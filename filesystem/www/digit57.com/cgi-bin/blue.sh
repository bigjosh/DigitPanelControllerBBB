#!/bin/bash
echo "Content-type: text/html"
echo ''
echo 'CGI Bash example<br>'
printf "\x00\x00\x00\x03\x00\x00\xff" >/dev/udp/localhost/7890
echo "Blue!"
