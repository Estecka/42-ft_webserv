#!/bin/zsh

plist=$(ps -e | grep webserv);
pcount=$(echo $plist | grep webserv -c);

if [ $pcount -lt 1 ]
then
	echo "Webserv not found";
elif [ $pcount -gt 1 ]
then
	echo "Mutilple instances of webserv are running: ";
	ps -e | grep webserv;
fi;

pid=$(echo $plist | grep '^ *[0-9]*' --only-matching | tr -d ' ')
unset plist pcount

while sleep 2;
do \
	clear;
	sudo ls /proc/$pid/fd/ -l --color=tty || exit;
done;