#!/bin/bash

# trap ctrl-c and call ctrl_c()
trap ctrl_c INT

function ctrl_c() {
        echo "** Trapped CTRL-C"
        python log_merger.py
        trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM EXIT


}

for file in /home/rahul/git/distributed/a1/p3/bin/*
do
  $file &
done

wait