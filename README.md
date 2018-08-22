# qupt
8-bit to quoted-print conversion utility

This utility, designed to be simple, stupid, is provided under the MIT license.

Read the LICENSE file for further information.

## Building

``gcc -c=ansi qupt.c -o qupt``

## Invoking

Qupt can take a filename as a parameter. The command reads stdin if no argument is given, or if the character `-` is put instead of a filename.

``qupt filename``

Please note that qupt is byte-buffered. Thus, when using stdin, the output will begin to output data on stdout as soon as a byte is given, and until `<EOF>` is reached. 
