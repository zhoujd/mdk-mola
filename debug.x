file zzmediaxapp

set env DISPLAY=:0.0

set breakpoint pending on

##task 1001
#set args -task 1001

##task 2001
#set args -task 2001 -i input-stream/foreman_352x288_20.nv12 -o output/test.nv12
set args -task 2001 -sw 1920 -sh 1080 -dw 1920 -dh 1080 -scc p010 -dcc p010 -i input-stream/crowdrun-1920x1080.p010 -o output/output.p010



b main
r
