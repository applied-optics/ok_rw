# ok_rw

A simple tool that talks to Opal Kelly FPGA cards over USB. It will do the following in order:

* Send a complete file to endpoint 0x80 using a block throttled pipe.
* Read a specified amount of bytes (must be a multiple of 1024) from the block
  throttled pipe at endpoint 0xA0 and save it to a file.

Only one of the two steps is required.
