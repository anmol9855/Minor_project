#ifndef FP_H
#define FP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// multiplier = 1/BASE 
#define BASE 32.0    //***give BASE as double
#define LOGBASE 5   //log of the base

class FixedPoint
{
public:
    int n;
    FixedPoint (int i);
    FixedPoint (double x);
    void print();
};

FixedPoint add (FixedPoint x , FixedPoint y);
FixedPoint sub (FixedPoint x , FixedPoint y);
FixedPoint mult (FixedPoint x , FixedPoint y);
FixedPoint div (FixedPoint x , FixedPoint y);
FixedPoint fplshift(FixedPoint x, int y);
FixedPoint fplshift(FixedPoint x, FixedPoint y);
FixedPoint fprshift(FixedPoint x, int y);
FixedPoint fprshift(FixedPoint x, FixedPoint y);
FixedPoint fpand (FixedPoint x , FixedPoint y);
FixedPoint fpor (FixedPoint x , FixedPoint y);
FixedPoint fpxor(FixedPoint x, FixedPoint y);

#endif