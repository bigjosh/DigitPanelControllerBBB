#!/bin/bash
lockdir=/tmp/leds.locks
if mkdir "$lockdir"; then
	if [ -f "$lockdir/lockdir.pid"]; then
		kill -SIGTERM $(cat "$lockdir/lockdir.pid")
	fi
    echo $$ >"$lockdir/lock.pid"
	rm "$lockdir/lock.dir"
	./leds $1 >/dev/tcp/localhost/7890 
	rm "$lockdir/lockdir.pid"
fi
