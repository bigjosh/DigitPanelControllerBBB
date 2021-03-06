#!/bin/bash

#this background process waits for new commands on a pipe
#it sends new commands to a new instance of the leds program, 
#killing any old instance that is runnning

#start by running start-ledsd.sh

lockdir=/tmp/leds.locks
pipe=$lockdir/fifo
pidfile=$lockdir/pid

mkdir "$lockdir"

echo "Creating initial pipe"
mkfifo "$pipe"

#give permisisons to everyone
chmod a+w "$pipe"

#stuff initiall Call into the pipe, but background it
#becuase it will block
		
while true
do	

	lpid=0

	if read line <$pipe; then
		if [[ "$line" == 'quit' ]]; then
			break
		fi
		
		echo "LPID=" $lpid
		
		if [ -e "$pidfile"  ]; then
		
			#kill last background process, even if it is allready dead
			#will error first pass, but thats ok
			echo "Killing" $(cat $pidfile)
			kill -SIGKILL  $(cat $pidfile)
			
			if [ $? -eq 0 ]; then
				echo Kill worked
			else
				echo Kill FAIL
			fi
		fi			
					
		#Run the actual leds command in the background
		./leds $line >/dev/tcp/localhost/7890 &
		
		echo $! >$pidfile
		echo "started " $!
		
	fi
done	

rm $pipe

#remove lock
rm -r "$lockdir"
	
exit

