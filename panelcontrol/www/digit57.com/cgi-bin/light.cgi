#!/bin/bash
echo "Content-type: text/html"
echo ''
echo 'CGI Bash example<br>'
./a.out 1 100 $QUERY_STRING >/dev/udp/localhost/7890
echo $QUERY_STRING
echo "Blue! " 


