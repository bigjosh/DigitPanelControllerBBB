#!/bin/bash
echo "Content-type: text/html"
echo ''
echo 'CGI Color set<br>'
./leds $QUERY_STRING >/dev/tcp/localhost/7890
echo "Finished"
