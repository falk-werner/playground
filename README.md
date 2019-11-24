[![Build Status](https://travis-ci.org/falk-werner/openssl-examples.svg?branch=master)](https://travis-ci.org/falk-werner/openssl-examples)

# openssl-examples

This project is a collection of examples using [Open SSL library](https://www.openssl.org/).

## Create self-signed certifcae and private key

Some examples require a certifacte and private key. To generate them, the following
command can be used:

    openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes

Note that this is a very basic examples. Please refer to advanded documentation to create
full featured PKI. One stating point might be the [OpenSSL PKI Tutorual](https://pki-tutorial.readthedocs.io/).

## Signing Files

To sign files, OpenSSL provides CMS ([Cryptographic Message Syntax](https://en.wikipedia.org/wiki/Cryptographic_Message_Syntax). CMS provides

*   integrity check (make sure the file was not tempered)
*   include intermediate certificates (allowing intermediate users to sign file)
*   revokation lists  (e.g. to prematurely revoke a certificate when the key was leaked)
*   multi-sign a file (e.g. to verify integrity even if one signature can not be checked)
*   message encryption (which is not discussed here yet)

A very basic example how to use CMS with OpenSSL is shown by cms_sign, cms_verify, cms_info applications.

### Sign a file

    echo "some text" > message.txt
    cms_sign -f message.txt -p key.pem -c cert.pem -o message.cms
    
### Show signature info

The signature created by cms_sign is soted in [DER format](https://wiki.openssl.org/index.php/DER). To print it's contents in human readable form, the following command can be used:

    cms_info -s message.cms

###  Verify a signature

    cms_verify -f message.txt -s message.cms -c cert.pem

