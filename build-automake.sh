#!/bin/bash

BUILD_ROOT=`pwd`
DUMP_ROOT=$BUILD_ROOT/dump

echo "build zzmedixapp with automake in $DUMP_ROOT folder start ..."

case $1 in
    "clean")
        rm -f  $BUILD_ROOT/zzmediaxapp
        rm -rf $DUMP_ROOT
        ;;
    *)
        ./autogen.sh
        
        mkdir -p $DUMP_ROOT
        
        pushd $DUMP_ROOT
        
        ../configure CFLAGS=-g O0 --enable-libudev_use=no
        make $1

        popd

        ls -l  $DUMP_ROOT/zzmediaxapp $DUMP_ROOT/libzzmediax.a 
        
        rm -f $BUILD_ROOT/zzmediaxapp
        ln -s $DUMP_ROOT/zzmediaxapp .
	;;
esac

echo "build zzmedixapp with automake in $DUMP_ROOT folder end ..."
