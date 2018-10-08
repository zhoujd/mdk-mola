file zzmediaxapp

set env DISPLAY=:0.0

set breakpoint pending on

##task 1001
define task1001
  set args -task 1001 -sw 352 -sh 288 -scc nv12 -i input-stream/foreman_352x288_20.nv12 -o output/output-d.nv12 -n 10
  b main

  r
end

##task 2001
define task2001
  #set args -task 2001 -i input-stream/foreman_352x288_20.nv12 -o output/test.nv12
  #set args -task 2001 -sw 1920 -sh 1080 -dw 1920 -dh 1080 -scc p010 -dcc p010 -i input-stream/crowdrun-1920x1080.p010 -o output/output.p010
  #set args -task 2001 -sw 1920 -sh 1080 -dw 1920 -dh 1080 -scc p010 -dcc argb -i input-stream/crowdrun-1920x1080.p010 -o output/output.argb -n 5

  set args -task 2001 -sw 3840 -sh 2160 -dw 3840 -dh 2160 -scc p010 -dcc argb -i input-stream/4K-3840x2160-10.p010 -o output/output.argb -hdr
  
  b main
  b DdiMedia_RenderPicture
  b DdiVp_RenderPicture
  b DdiVp_SetProcPipelineParams
  b DdiVp_GetColorSpace

  r
end

task2001
