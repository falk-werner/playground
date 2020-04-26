/*
 * MIT License
 *
 * Copyright (c) 2019 Falk Werner
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define BUFFER_SIZE 1024

static void print_usage()
{
    printf(
        "popen, example of popen\n"
        "executes a command using popen\n"
        "\n"
        "Usage: popen <command>\n"
        "\n"
        "Arguments:\n"
        "\tcommand - command to execute\n"
        "\n"
        "Example:\n"
        "\tpopen \"ls -lah\"\n"
    );
}

static void sigpipe_handler(int signal_number)
{
    (void) signal_number;
    fprintf(stderr, "signal: SIGPIPE received\n");
}

int main(int argc, char * argv[])
{
    int result = EXIT_FAILURE;

    if (1 < argc)
    {
        signal(SIGPIPE, sigpipe_handler);

        char const * command = argv[1]; 
        FILE * file = popen(command, "r");
        if (NULL != file)
        {
            char buffer[BUFFER_SIZE + 1];
            size_t len = fread(buffer, 1, BUFFER_SIZE, file);
            while (0 < len)
            {
                buffer[len] = '\0';
                printf("%s", buffer);
                len = fread(buffer, 1, BUFFER_SIZE, file);
            }

            int status = pclose(file);
            result = WEXITSTATUS(status);
        }
        else
        {
            fprintf(stderr, "error: unable to execute command\n");
        }
        
    }
    else
    {
        print_usage();
    }
    

    return result;
}
