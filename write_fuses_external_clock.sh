#!/bin/bash

MCU=atmega32
PROGRAMMER_MCU=m32
AVRDUDE_PROGRAMMERID=usbasp
AVRDUDE=avrdude

${AVRDUDE} -c ${AVRDUDE_PROGRAMMERID} -p ${PROGRAMMER_MCU} -U lfuse:w:0b11101111:m -q