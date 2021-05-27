#!/bin/bash

# Find the port the arduino is on
SERIAL_DEV=`find /dev/ -name "*cu.usbserial*" | head -n1`

if [ ! -z $SERIAL_DEV ]; then
    make upload
    if [ $? -eq 0 ]; then
        echo "Found Serial dev: $SERIAL_DEV"
        echo "Serial Output:"
        cat $SERIAL_DEV
    fi
else 
    make
    echo "No board plugged in"
fi
