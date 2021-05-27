#!/bin/bash

SERIAL_DEV=`fd --glob *cu.usb* /dev/`

if [ ! -z $SERIAL_DEV ]; then
    make upload
    if [ $? -eq 0 ]; then
        echo "Found Serial dev: $SERIAL_DEV"
        echo "Serial Output:"
        cat $SERIAL_DEV
    fi
else 
    echo "No board plugged in"
fi
