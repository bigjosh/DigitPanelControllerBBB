lockdir=/tmp/leds.locks
pipe=$lockdir/fifo
echo $1 >$pipe
