#!/bin/sh

export DISPLAY=:0.0

run_task_1101()
{
    echo "run task 1101 start ..."
    ./zzmediaxapp -task 1101
    echo "run task 1101 end ..."
}

run_task_1001()
{
    echo "run task 1001 start ..."
    ./zzmediaxapp -task 1001
    echo "run task 1001 end ..."
}

run_task_1001
