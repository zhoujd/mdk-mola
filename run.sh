#!/bin/sh

export DISPLAY=:0.0

if [ ! -e ./zzmediaxapp ]; then
    echo "no ./zzmediaxapp, please run build script"
    exit 1
fi

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
run_task_1101
