#!/bin/bash

# trap ctrl-c and call ctrl_c()
trap ctrl_c INT

function ctrl_c() {
        echo "Trapped CTRL-C. Exiting. Merging per process logs to merged_u.log or merged_b.log"
        python log_merger.py "$mode"
        trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM EXIT
}

echo "Press 'b' for Broadcast and 'u' for Unicast (without the single quotes), followed by [ENTER]"
read mode

if [[ $mode = "u" ]]
then
	echo "Starting demo in Unicast mode"
else
	echo "Starting demo in Broadcast mode"
fi
echo "Wait for some time before exiting, after it says all messages have been sent to account for the delay factor. Exit with Ctrl-C."
for i in {0..4}
do
  if [[ $mode = "u" ]]
  then
  	./bin/main "Unicast" "$i" &
  else
  	./bin/main "Broadcast" "$i" &
  fi
done

wait