#!/bin/bash
echo "Content-type: text/html"
echo ''
echo 'CGI Color set<br>'
./a.out 57 30 5 $QUERY_STRING >/dev/tcp/localhost/7890
sleep 1
./a.out 57 30 5 C000000 >/dev/tcp/localhost/7890
ech "Finished"

