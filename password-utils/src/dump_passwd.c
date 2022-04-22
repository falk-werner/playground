#include <sys/types.h>
#include <pwd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool can_login(char const * shell)
{
    static char const * fake_shells[] = {
        "/bin/false",
        "/sbin/nologin",
        "/usr/sbin/nologin"
    };

    for(size_t i = 0; NULL != fake_shells[i]; i++)
    {
        char const * fake_shell = fake_shells[i];
        if (0 == strcmp(shell, fake_shell))
        {
            return false;
        }
    }

    return true;
}

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;
    int exit_code = EXIT_SUCCESS;

    FILE * file = fopen("/etc/passwd", "rb");
    if (NULL != file)
    {
        struct passwd const * entry = fgetpwent(file);
        while (NULL != entry)
        {
            if (can_login(entry->pw_shell))
            {
                printf("name          : %s\n", entry->pw_name);
                printf("password      : %s\n", entry->pw_passwd);
                printf("user id       : %d\n", entry->pw_uid);
                printf("group id      : %d\n", entry->pw_gid);
                printf("info (gecos)  : %s\n", entry->pw_gecos);
                printf("home directory: %s\n", entry->pw_dir);
                printf("shell         : %s\n", entry->pw_shell);
                printf("\n");
            }

            entry = fgetpwent(file);
        }

        fclose(file);
    }
    else
    {
        fprintf(stderr, "error: failed to open /etc/passwd\n");
        exit_code = EXIT_FAILURE;
    }

    return exit_code;
}