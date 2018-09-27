#ifndef ZEVENTBASE_H
#define ZEVENTBASE_H

#include "cmn.h"

enum zzEventEN
{
    //global event
    ZZ_EVENT_START  = 0,
    ZZ_EVENT_END,

    ZZ_EVENT_PART_START,
    ZZ_EVENT_PART_END,


    ZZ_EVENT_PIPE_EXIT,

    //custom event
    ZZ_EVENT_USER   = 0x1000,

    //unknow event
    ZZ_EVENT_UNKNOW = 0xFFFF,


};

typedef zzU32 zzEventID; 

#endif //ZEVENTBASE_H
