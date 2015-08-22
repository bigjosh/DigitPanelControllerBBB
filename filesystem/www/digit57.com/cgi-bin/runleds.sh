#!/bin/bash

#this script sends a message to the pipe that was created by a 
#background ledsd

lockdir=/tmp/leds.locks
pipe=$lockdir/fifo
echo $1 >$pipe
