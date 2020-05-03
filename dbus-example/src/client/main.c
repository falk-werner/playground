#include "greeter.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int result = EXIT_FAILURE;

    Greeter * greeter = greeter_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION, G_DBUS_PROXY_FLAGS_NONE, "org.example.Greeter", "/org/example/greeter", NULL, NULL);
    gchar * greeting = NULL;
    gboolean success = greeter_call_say_hello_sync(greeter, "Bob", &greeting, NULL, NULL);
    if (success)
    {
        puts(greeting);
        free(greeting);
        result = EXIT_SUCCESS;        
    }
    else
    {
        fprintf(stderr, "error: faild to say_hello\n");
    }
    
    g_object_unref(greeter);
    return result;
}