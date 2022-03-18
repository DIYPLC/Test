#include <stdint.h>
#include <stdbool.h>
#include <iso646.h>
#include "FbFilterNaN.h"

#define In    p->In
#define Out   p->Out
#define Alarm p->Alarm

void FbFilterNaN(struct DbFilterNaN *p)
{

//Если In==(NaN или Inf или -Inf) то Out=0 иначе Out=In.
// hex 7F800000 = bin 01111111_10000000_00000000_00000000
//Стандарт IEEE 754.
if ( 0x7F800000 == ( ((uint32_t)In) bitand 0x7F800000) )
{
Out   = 0.0;
Alarm = true;
}
else
{
Out   = In;
Alarm = false;
}

  return;
}

//  +---------+
//  | GNU GPL |
//  +---------+
//  |
//  |
//  .= .-_-. =.
// ((_/)o o(\_))
//  `-'(. .)`-'
//  |/| \_/ |\
//  ( |     | )
//  /"\_____/"\
//  \__)   (__/