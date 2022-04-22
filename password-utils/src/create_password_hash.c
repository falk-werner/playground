#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
    int exit_code = EXIT_SUCCESS;
    if (argc == 3)
    {
        char const * salt = argv[1];
        char const * password = argv[2];

        char const * hash = crypt(password, salt);
        puts(hash);
    }
    else
    {
        printf(
            "Usage: create_password_hash <salt> <password>\n"
            "\n"
            "Examples:\n"
            "\t create_password_hash foo bar\n"
            "\t create_password_hash \'$6$foo$\' bar\n"
        );
    }

    return exit_code;
}