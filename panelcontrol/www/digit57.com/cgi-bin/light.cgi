#!/bin/bash
echo "Content-type: text/html"
echo ''
echo 'CGI Bash example<br>'
#Must use TCP becuase this is too big for a UDP packet
./a.out 1 100 $QUERY_STRING >/dev/tcp/localhost/7890
echo $QUERY_STRING
echo "Blue! " 


