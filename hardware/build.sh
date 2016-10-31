#!/bin/bash

make clean && make
avr-objcopy -j .text -j .data -O ihex remotelab.elf remotelab.hex

