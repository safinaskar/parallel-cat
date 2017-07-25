#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>
#include <unistd.h>

#include <err.h>

int main(int argc, char *argv[])
{
  if (argc != 3)
    {
      fprintf(stderr, "Usage: %s FILE-1 FILE-2\n", argv[0]);
      exit (EXIT_FAILURE);
    }

  int fd_1 = open(argv[1], O_RDONLY);

  if (fd_1 == -1)
    {
      err (EXIT_FAILURE, "%s", argv[1]);
    }

  int fd_2 = open(argv[2], O_RDONLY);

  if (fd_2 == -1)
    {
      err (EXIT_FAILURE, "%s", argv[2]);
    }

  fd_set set;

  FD_ZERO(&set);
  FD_SET(fd_1, &set);
  FD_SET(fd_2, &set);

  for (;;){ //
    if (!FD_ISSET(fd_1, &set) && !FD_ISSET(fd_2, &set)){
      // Two EOFs
      break;
    }

    fd_set set_work = set;

    if (select(100, &set_work, NULL, NULL, NULL) == -1){
      if (errno == EINTR){
        continue;
      }else{
        err (EXIT_FAILURE, "select");
      }
    }

    if (FD_ISSET(fd_1, &set_work)){
      char c;
      int read_returned = read(fd_1, &c, 1);

      if (read_returned == -1){
        err (EXIT_FAILURE, NULL);
      }else if (read_returned == 0){
        FD_CLR(fd_1, &set);
        close(fd_1);
      }else{
        switch (write(1, &c, 1)){
          case -1:
            err (EXIT_FAILURE, NULL);
          case 0:
            errx (EXIT_FAILURE, "write returned 0");
        }
      }
    }

    if (FD_ISSET(fd_2, &set_work)){
      char c;
      int read_returned = read(fd_2, &c, 1);

      if (read_returned == -1){
        err (EXIT_FAILURE, NULL);
      }else if (read_returned == 0){
        FD_CLR(fd_2, &set);
        close(fd_2);
      }else{
        switch (write(1, &c, 1)){
          case -1:
            err (EXIT_FAILURE, NULL);
          case 0:
            errx (EXIT_FAILURE, "write returned 0");
        }
      }
    }
  }

  exit (EXIT_SUCCESS);
}
