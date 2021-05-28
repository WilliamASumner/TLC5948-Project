# TLC5948A Debug
This is my current debug setup for the TLC5948A library. The reason this is a bit convoluted is I wanted to use some "newer" C++ features like enum classes to help make the bit-flicking a little more ergonomic.

## Requirements
This file requires a `python3` installation and `pyserial`, as well as `make`. Check the main [README](https://github.com/WilliamASumner/TLC5948-Project) of this project (from the lovely Arduino Makefile project) for more information. As far as hardware goes, I'm testing with an Arduino Nano and eventually a Teensy 4.1.

## Running it
Running it is pretty easy, simply use either `./run.sh` with a board connected, or `make upload` and run your favorite serial monitor, I like to just use `cat`.
