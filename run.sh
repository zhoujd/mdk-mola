#!/bin/sh

export DISPLAY=:0.0

mkdir -p output

if [ ! -e ./zzmediaxapp ]; then
    echo "no ./zzmediaxapp, please run build script"
    exit 1
fi

run_task_2001()
{
    echo "run task 2001 start ..."
    #./zzmediaxapp -task 2001 -i input-stream/foreman_352x288_20.nv12 -o output/test.nv12
    ./zzmediaxapp -task 2001 -sw 352 -sh 288 -dw 720 -dh 576 -scc nv12 -dcc nv12 -i input-stream/foreman_352x288_20.nv12 -o output/output.nv12 -sf 30 -angle 1
    echo "run task 2001 end ..."
}

run_task_1001()
{
    echo "run task 1001 start ..."
    ./zzmediaxapp -task 1001
    echo "run task 1001 end ..."
}

#run_task_1001
run_task_2001
