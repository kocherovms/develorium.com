#!/bin/sh
set -e

if [ "$#" -ne 1 ]; then
	echo "Usage: $0 PID\nUnmasks socket information from ls -l /proc/$PID/fd"
	exit 1
fi

PID=$1
TCP_INFO=$(ss -ntuae)
UNIX_INFO=$(ss -xap | grep $PID)

ls -l /proc/$PID/fd |
while read fd; do
	if $(echo $fd | grep -q 'socket:'); then
		inode=$(echo $fd | sed 's/\(.*socket\):\[\(.*\)\].*/\2/g')
		tcp_socket_info=$(echo "$TCP_INFO" | grep "ino:$inode" | cat)
		if [ -n "$tcp_socket_info" ]; then
			echo "$fd -> $tcp_socket_info"
			continue
		fi

		unix_socket_info=$(echo "$UNIX_INFO" | grep "$inode" | cat)
		if [ -n "$unix_socket_info" ]; then
			echo "$fd -> $unix_socket_info"
			continue
		fi

		echo "$fd -> ???"
		continue
	fi

	echo $fd
done

