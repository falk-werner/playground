#include <shadow.h>

#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SECONDS_PER_DAY (24 * 60 * 60)

static bool is_password_expired(char const * username)
{
    bool expired = false;

    struct spwd const * entry = getspnam(username);
    if (NULL != entry)
    {
        if (0 == entry->sp_lstchg)
        { 
            expired = true;
        }
        else
        {
            time_t const now = time(NULL);
            long const days_since_epoch = now / SECONDS_PER_DAY;

            long const expires_at = entry->sp_lstchg + entry->sp_max;
            expired = (expires_at < days_since_epoch);
        }
    }
    else
    {
        fprintf(stderr, "error: unknown user\n");
        expired = true; // tread unknown as expired
    }

    return expired;
}

int main(int argc, char * argv[])
{
    int exit_code = EXIT_SUCCESS;

    if (argc > 1)
    {
        for(int i = 1; i < argc; i++)
        {
            char const * username = argv[i];
            bool expired = is_password_expired(username);
            printf("%s: %s\n", username, expired ? "expired" : "valid");

            if (expired)
            {
                exit_code = EXIT_FAILURE;
            }
        }

    }
    else
    {
        printf("usage: is_password_expired <username>...\n");
    }


    return exit_code;
}