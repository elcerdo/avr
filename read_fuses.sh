#!/bin/bash

MCU=atmega32
PROGRAMMER_MCU=m32
AVRDUDE_PROGRAMMERID=usbasp
AVRDUDE=avrdude

LFUSE=$(mktemp)
HFUSE=$(mktemp)
${AVRDUDE} -c ${AVRDUDE_PROGRAMMERID} -p ${PROGRAMMER_MCU} -U lfuse:r:${LFUSE}:b -q
${AVRDUDE} -c ${AVRDUDE_PROGRAMMERID} -p ${PROGRAMMER_MCU} -U hfuse:r:${HFUSE}:b -q
LFUSE=$(cat ${LFUSE})
HFUSE=$(cat ${HFUSE})

echo -e "low fuse bits:  ${LFUSE:2:4} ${LFUSE:6}\n\tBODLVL\t${LFUSE:2:1}\n\tBODEN\t${LFUSE:3:1}\n\tSUT1\t${LFUSE:4:1}\n\tSUT0\t${LFUSE:5:1}\n\tCKSEL3\t${LFUSE:6:1}\n\tCKSEL2\t${LFUSE:7:1}\n\tCKSEL1\t${LFUSE:8:1}\n\tCKSEL0\t${LFUSE:9:1}"
echo -e "high fuse bits: ${HFUSE:2:4} ${HFUSE:6}\n\tOCDEN\t${HFUSE:2:1}\n\tJTAGEN\t${HFUSE:3:1}\n\tSPIEN\t${HFUSE:4:1}\n\tCKOPT\t${HFUSE:5:1}\n\tEESAVE\t${HFUSE:6:1}\n\tBOOTSZ1\t${HFUSE:7:1}\n\tBOOTSZ0\t${HFUSE:8:1}\n\tBOOTRST\t${HFUSE:9:1}"

