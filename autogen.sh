#!/bin/sh

#1: create hello.c source file
#2: run autoscan in current path
#3: edit Makefile.am

#4: run  aclocal aclocal.m4 will be create
aclocal

#5: run autoconf configure will be create
autoconf

#6: run autoheader config.h.in will be create
autoheader

#7: run automake --add-missing like this
automake --add-missing


