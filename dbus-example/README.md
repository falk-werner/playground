# dbus-example

Example to show simple interprocess communication (*IPC*) using D-BUS.

## Build an Run

    mkdir build
    cd build
    cmake ..
    make
    ./greeter_service &
    ./greet
    killall greeter_service

## Basic approach

In this example, [glib](https://developer.gnome.org/glib/) is used for D-Bus communication.
it contains a tool named `gdbus-codegen` to generate proxy and stub code. Therefore an interface description file (XML) is used.

## Running with valgrind

I did not manage to remove all valgrind issues :-/.  
**Note:** glib is shipped with a valgrind suppression file located at `/usr/share/glib-2.0/valgrind/glib.supp`.

## Further Information

*    **[gdbus-codegen](https://developer.gnome.org/gio/stable/gdbus-codegen.html):** generate proxy and stub for D-BUS communication
*    **[D-Bus Introspection Format](https://dbus.freedesktop.org/doc/dbus-specification.html#introspection-format):** Define D-Bus Interface via XML
*    **[D-Bus Specification](https://dbus.freedesktop.org/doc/dbus-specification.html)**
*    **[g_bus_own_name](https://developer.gnome.org/gio/stable/gio-Owning-Bus-Names.html#g-bus-own-name)**