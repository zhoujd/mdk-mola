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
    #./zzmediaxapp -task 2001 -sw 352 -sh 288 -dw 720 -dh 576 -scc nv12 -dcc nv12 -i input-stream/foreman_352x288_20.nv12 -o output/output.nv12 -sf 30 -angle 1 -n 10

    #./zzmediaxapp -task 2001 -sw 1920 -sh 1080 -dw 1920 -dh 1080 -scc p010 -dcc p010 -i input-stream/crowdrun-1920x1080.p010 -o output/output.p010 -n 5

    #./zzmediaxapp -task 2001 -sw 1920 -sh 1080 -dw 1920 -dh 1080 -scc p010 -dcc argb -i input-stream/crowdrun-1920x1080.p010 -o output/output.argb -n 5

    ./zzmediaxapp -task 2001 -sw 3840 -sh 2160 -dw 3840 -dh 2160 -scc p010 -dcc argb -i input-stream/4K-3840x2160-10.p010 -o output/output-hdr.argb -hdr
    ./zzmediaxapp -task 2001 -sw 3840 -sh 2160 -dw 3840 -dh 2160 -scc p010 -dcc argb -i input-stream/4K-3840x2160-10.p010 -o output/output.argb

    md5sum output/output-hdr.argb output/output.argb
    ## dump surface
    #./zzmediaxapp -task 2001 -sw 3840 -sh 2160 -dw 3840 -dh 2160 -scc p010 -dcc p010 -i input-stream/4K_3840x2160.P010 -o output/4K-3840x2160-10.p010 -n 10 

    echo "run task 2001 end ..."
}

run_task_1001()
{
    echo "run task 1001 start ..."
    ./zzmediaxapp -task 1001
    echo "run task 1001 end ..."
}

run_task_1001
#run_task_2001
