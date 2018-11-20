#!/bin/sh
#set -x

export DISPLAY=:0.0
#export LIBVA_DRIVERS_PATH=/usr/lib/x86_64-linux-gnu/dri/
export LIBVA_DRIVER_NAME=iHD  

mkdir -p output
rm -rf output/*

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

    #./zzmediaxapp -task 2001 -sw 3840 -sh 2160 -dw 3840 -dh 2160 -scc p010 -dcc argb -i input-stream/4K-3840x2160-10.p010 -o output/output-hdr.argb -hdr
    #./zzmediaxapp -task 2001 -sw 3840 -sh 2160 -dw 3840 -dh 2160 -scc p010 -dcc argb -i input-stream/4K-3840x2160-10.p010 -o output/output.argb
    #md5sum output/output-hdr.argb output/output.argb

    h2h_input="--hdr-input x0=13250:x1=7500:x2=34000:y0=34500:y1=3000:y2=16000:wp_x=15635:wp_y=16450:max_dl=10000:min_dl=1000:max_cl=2000:max_pl=2000"
    h2h_output="--hdr-output x0=13250:x1=7500:x2=34000:y0=34500:y1=3000:y2=16000:wp_x=15635:wp_y=16450:max_dl=10000:min_dl=1000:max_cl=2000:max_pl=2000"
    h2s_input="--hdr-input x0=13250:x1=7500:x2=34000:y0=34500:y1=3000:y2=16000:wp_x=15635:wp_y=16450:max_dl=10000:min_dl=1000:max_cl=2000:max_pl=2000"

    ## H2H
    ./zzmediaxapp -task 2001 -sw 3840 -sh 2160 -dw 3840 -dh 2160 -scc p010 -dcc a2rgb10 -i input-stream/4K-3840x2160-10.p010 -o output/output-h2h.a2rgb10 -h2h $h2h_input $h2h_output

    ## H2S
    ./zzmediaxapp -task 2001 -sw 3840 -sh 2160 -dw 3840 -dh 2160 -scc p010 -dcc argb -i input-stream/4K-3840x2160-10.p010 -o output/output.argb -h2s $h2s_input
    
    md5sum output/*
       
    echo "run task 2001 end ..."
}

run_task_1001()
{
    echo "run task 1001 start ..."
    #./zzmediaxapp -task 1001 -sw 352 -sh 288 -scc nv12 -i input-stream/foreman_352x288_20.nv12 -o output/output-d.nv12
    #./zzmediaxapp -task 1001 -sw 3840 -sh 2160 -scc p010 -i input-stream/4K_3840x2160.P010 -o output/4K-3840x2160-10.p010 -n 10

    ./zzmediaxapp -task 1001 -sw 127 -sh 127 -scc a2rgb10 -i input-stream/centering_scaling_test_pattern_127x127_5frames.a2r10g10b10 -o output/test.a2rgb10
    md5sum input-stream/centering_scaling_test_pattern_127x127_5frames.a2r10g10b10 output/test.a2rgb10
    
    echo "run task 1001 end ..."
}

run_task_2002()
{
    echo "run task 2002 start ..."
    #./zzmediaxapp -task 2002 -sw 352 -sh 288 -dw 720 -dh 576 -scc nv12 -dcc argb -i input-stream/foreman_352x288_20.nv12 -df 30.0 -angle 1 -n 10
    #./zzmediaxapp -task 2002 -sw 352 -sh 288 -dw 720 -dh 576 -scc nv12 -dcc argb -i input-stream/foreman_352x288_20.nv12 -df 30.0 -angle 1
    ./zzmediaxapp -task 2002 -sw 352 -sh 288 -dw 720 -dh 576 -scc nv12 -dcc argb -i input-stream/foreman_352x288_20.nv12 -df 30.0 -angle 2 -demo
    echo "run task 2002 end ..."    
}

#run_task_1001
run_task_2001
#run_task_2002
