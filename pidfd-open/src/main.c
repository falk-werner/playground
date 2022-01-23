#include "pidfd_open.h"

#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
    int exit_code = EXIT_SUCCESS;

    pid_t const pid = fork();
    if (pid > 0)
    {
        // parent
        printf("child started\n");
        int fd = pidfd_open(pid, 0);
        
        struct pollfd fds[1];
        fds[0].fd = fd;
        fds[0].events = POLLIN;

        bool finished = false;
        while (!finished)
        {
            printf("poll...\n");
            int rc = poll(fds, 1, 1000);
            if (rc > 0)
            {
                if (fds[0].revents != 0)
                {
                    close(fd);
                    int status = 0;
                    pid_t p = waitpid(pid, &status, 0);
                    if (p == pid)
                    {
                        printf("child terminated with exit_code: %d\n", WEXITSTATUS(status));
                        finished = true;
                    }
                }
            }
        }

    }
    else if (0 == pid)
    {
        // child
        sleep(5);
    }
    else
    {
        perror("fork");
        exit_code = EXIT_FAILURE;
    }
    

    return exit_code;
}