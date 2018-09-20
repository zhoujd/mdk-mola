#!/bin/bash


BUILD_ROOT=`pwd`
DUMP_ROOT=$BUILD_ROOT/build

echo "build zzmedixapp with cmake in $DUMP_ROOT folder start ..."

case $1 in
    "clean")
        rm -f  $BUILD_ROOT/zzmediaxapp
        rm -rf $DUMP_ROOT
        ;;
    *)
        mkdir -p build
        pushd build
        
        cmake ..
        make $1
        
        popd

        rm -f $BUILD_ROOT/zzmediaxapp
        ln -s $DUMP_ROOT/zzmediaxapp .
	    ;;
esac

echo "build zzmedixapp with cmake in $DUMP_ROOT folder start ..."
