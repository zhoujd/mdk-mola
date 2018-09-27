file zzmediaxapp

set env DISPLAY=:0.0

set breakpoint pending on

##task 1001
#set args -task 1001

##task 2001
set args -task 2001 -i input-stream/foreman_352x288_20.nv12 -o output/test.nv12

b main
r
