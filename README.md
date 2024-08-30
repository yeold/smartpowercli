## CLI Program for Odroid Smart Power V1

This is a command line interface program for controlling the [Odroid Smart Power V1](https://wiki.odroid.com/old_product/accessory/odroidsmartpower)

This is a discontunued product, but I've had one laying around for a decade and I wanted an easy way to interface with it through a linux terminal

### Usage
The program takes two arguments:

Path to the device descriptor

And a command

You need to find out what device file that corresponds to your PSU when you connect it, usually something like `/dev/hidrawX`

#### Commands

onoff : Toggles power

startstop : Toggles power logging

status : Returns the status of the PSU

version : Returns the FW version


### Building

As for now, just build the binary with this command:

`gcc -g src/smartpowercli.c -o build/smartpowercli`
