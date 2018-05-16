#!/bin/bash

USADA=$(free -m| grep Mem | awk '{print $3/$2 * 100.0}');
LIVRE=$(free -m| grep Mem | awk '{print ($7)/$2 * 100.0}');
./Exe10 $LIVRE $USADA;
