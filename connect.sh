#!/bin/sh

# connect.sh

# Usage:
# $ ./connect.sh <device> <port speed>
# Example: ./connect.sh /dev/ttyS0 9600
#

# tty settings
#stty -F "$1" "$2" raw -icanon min 1 time 0

# THESE WORK vvvv
#stty -F "$1" "$2" cs8 -cstopb -parenb -echo -icanon min 1 time 0 raw

# THESE WORK ALSO vvvv
# stty -F "$1" "$2" -icrnl -ixon -opost -isig -icanon -echo

# Let cat read the device $1 in the background
# cat "$1" &

# Capture the PID of the background process (cat)
# so it is possible to terminate it when done.
# bgPid=$!

# Read commands from user, send them to device $1
# while read cmd
# do
#    echo "$cmd" > "$1"
# done

# Terminate background read process
# kill "$bgPid"
putty -load arduino_uart
