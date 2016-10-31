#!/bin/bash

avrdude -p atmega328p -c usbasp -e -U flash:w:remotelab.hex:i

