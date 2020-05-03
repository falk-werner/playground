#include "greeter.h"
#include <glib-unix.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 80

struct context
{
    GMainLoop * loop;
    guint bus_id;
};

static void
on_name_lost(
    GDBusConnection * connection, 
    gchar const * name,
    gpointer user_data)
{
    (void) connection;

    struct context * context = user_data;
    fprintf(stderr, "error: failed to acquire bus: service already running?\n");
    g_bus_unown_name(context->bus_id);
}


static gboolean
on_say_hello(
    Greeter * greeter,
    GDBusMethodInvocation * invocation,
    gchar const * whom,
    gpointer * user_data)
{
    (void*) user_data;
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "Hello, %s\n", whom);

    greeter_complete_say_hello(greeter, invocation, buffer);
    return TRUE;
}

static void
on_name_acquired(
    GDBusConnection * connection, 
    gchar const * name,
    gpointer user_data)
{
    Greeter * greeter;
    greeter = greeter_skeleton_new();
    gboolean success = TRUE;

    int id = g_signal_connect(greeter, "handle-say_hello", G_CALLBACK(on_say_hello), user_data);
    if (0 >= id)
    {
        fprintf(stderr, "error: failed to connect handler: say_hello\n");
        success = FALSE;
    }

    if (success)
    {
        success = g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(greeter),
            connection, "/org/example/greeter", NULL);
        if (!success)
        {
            fprintf(stderr, "failed to export interface\n");
        }
    }

    if (!success)
    {
        struct context * context = user_data;
        g_bus_unown_name(context->bus_id);
    }
}


static gboolean
on_interrupt(gpointer user_data)
{
    struct context * context = user_data;
    g_bus_unown_name(context->bus_id);
}

static void
free_user_data(
    void * user_data)
{
    struct context * context = user_data;
    g_main_loop_quit(context->loop);
}

int main(int argc, char * argv[])
{
    struct context context;

    context.loop = g_main_loop_new(NULL, FALSE);
    context.bus_id = g_bus_own_name(G_BUS_TYPE_SESSION, "org.example.Greeter", 
        G_BUS_NAME_OWNER_FLAGS_NONE, 
        NULL, &on_name_acquired, &on_name_lost,
        &context, &free_user_data);
    
    if (0 == context.bus_id)
    {
        fprintf(stderr, "error: failed to own bus\n");
    }

    g_unix_signal_add(SIGINT, &on_interrupt, &context);

    printf("running.... \n");
    g_main_loop_run(context.loop);

    printf("shutdown...\n");
    g_main_loop_unref(context.loop);

    return EXIT_SUCCESS;
}