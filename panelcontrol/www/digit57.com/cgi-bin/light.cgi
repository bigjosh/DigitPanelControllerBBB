#!/bin/bash
echo "Content-type: text/html"
echo ''
echo 'CGI Bash example<br>'
./a.out 57 30 5 $QUERY_STRING >/dev/tcp/localhost/7890
echo $QUERY_STRING
echo "Blue! " 


