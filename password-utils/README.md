# password-utils

A small collection of tools dealing with linux passwords.

## dump_passwd

    dump_passwd

Prints the contents of /etc/passwd for all users that can login (have a valid shell set).

## is_password_expired

    sudo is_password_expired <user>

Checks whether the password of a given user is expired.

## create_password_hash

    create_password_hash <salt> <password>

Creates a hash for a given password. The tool is just a wrapper around the `crypt` function. When `<salt>` is provided in the format `$<n>$<salt>$`, the hash algorithm is specified by `<n>` (see [https://man7.org/linux/man-pages/man3/crypt.3.html](https://man7.org/linux/man-pages/man3/crypt.3.html).
