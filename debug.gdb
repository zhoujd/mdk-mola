
#cd /ssh:root@10.239.156.124:/root/build_hdr/mdk-mola

file zzmediaxapp

set env DISPLAY=:0.0
set env LIBVA_DRIVER_NAME=iHD
set env LIBVA_DRIVERS_PATH=/opt/zach/lib/dri
set env LD_LIBRARY_PATH=/opt/zach/lib:/opt/zach/ffmpeg/lib

set breakpoint pending on
set solib-search-path /opt/zach/lib:/opt/zach/lib/dri:/opt/zach/ffmpeg/lib

##task 1001
define task1001
  #set args -task 1001 -sw 352 -sh 288 -scc nv12 -i input-stream/foreman_352x288_20.nv12 -o output/output-d.nv12 -n 10
  set args -task 1001 -sw 127 -sh 127 -scc a2rgb10 -i input-stream/centering_scaling_test_pattern_127x127_5frames.a2r10g10b10 -o output/test.a2rgb10
  
  b main

  r
end

##task 2001
define task2001
  #set args -task 2001 -i input-stream/foreman_352x288_20.nv12 -o output/test.nv12
  #set args -task 2001 -sw 1920 -sh 1080 -dw 1920 -dh 1080 -scc p010 -dcc p010 -i input-stream/crowdrun-1920x1080.p010 -o output/output.p010
  #set args -task 2001 -sw 1920 -sh 1080 -dw 1920 -dh 1080 -scc p010 -dcc argb -i input-stream/crowdrun-1920x1080.p010 -o output/output.argb -n 5
  #set args -task 2001 -sw 3840 -sh 2160 -dw 3840 -dh 2160 -scc p010 -dcc argb -i input-stream/4K-3840x2160-10.p010 -o output/output.argb -hdr

  #h2h
  set args -task 2001 -sw 3840 -sh 2160 -dw 3840 -dh 2160 -scc p010 -dcc a2rgb10 -i input-stream/4K-3840x2160-10.p010 -o output/output-h2h.a2rgb10 -h2h --hdr-input x0=13250:x1=7500:x2=34000:y0=34500:y1=3000:y2=16000:wp_x=15635:wp_y=16450:max_dl=10000:min_dl=1000:max_cl=2000:max_pl=2000 --hdr-output x0=13250:x1=7500:x2=34000:y0=34500:y1=3000:y2=16000:wp_x=15635:wp_y=16450:max_dl=10000:min_dl=1000:max_cl=2000:max_pl=2000
  
  #h2s
  #set args -task 2001 -sw 3840 -sh 2160 -dw 3840 -dh 2160 -scc p010 -dcc a2rgb10 -i input-stream/4K-3840x2160-10.p010 -o output/output.a2rgb10 --hdr-input x0=13250:x1=7500:x2=34000:y0=34500:y1=3000:y2=16000:wp_x=15635:wp_y=16450:max_dl=10000:min_dl=1000:max_cl=2000:max_pl=2000
  
  b main
  b ZZVA_InitVA
  b DdiMedia_RenderPicture
  b DdiVp_RenderPicture
  b DdiVp_SetProcPipelineParams
  b DdiVp_GetColorSpace
  b ZZMatrix2002_ProcNextFrame
  b render_picture_vp_hdr

  r
end

define task2003
  set args -task 2003 -sw 720 -sh 480 -dw 720 -dh 480 -scc nv12 -dcc nv12 -i input-stream/water.h264 -o output/water.yuv -n 2
  b main
  #b ffmpeg_decode_init
  #b ZZSurface_LockFrame
  b DdiMedia_DeriveImage
  r
end

task2003
#task1001
