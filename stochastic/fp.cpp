#include "fp.h"

FixedPoint::FixedPoint(int i){n=i;}

FixedPoint::FixedPoint(double x){
    n = int(x * BASE);
}

void FixedPoint::print(){
    printf("n=%d , multiplier = %f\n",n,1/BASE);
}


FixedPoint add(FixedPoint x , FixedPoint y) {
    FixedPoint z (x.n + y.n);
    return z;
}

FixedPoint sub(FixedPoint x , FixedPoint y) {
    FixedPoint z (x.n - y.n) ;
    return z;
}

FixedPoint mult(FixedPoint x, FixedPoint y) {
    FixedPoint z(((double)(x.n)*(double)(y.n)));
    return z;
}

FixedPoint div(FixedPoint x , FixedPoint y){
    FixedPoint z (double (x.n)/ double(y.n));
    return z;
}

FixedPoint fplshift(FixedPoint x, int y){
    FixedPoint z(x.n << y);
}

FixedPoint fplshift(FixedPoint x, FixedPoint y){
    FixedPoint z(x.n << (y.n >> (int)LOGBASE));
    return z;
}

FixedPoint fprshift(FixedPoint x, int y){
    FixedPoint z(x.n >> y);
}

FixedPoint fprshift(FixedPoint x, FixedPoint y){
    FixedPoint z(x.n >> (y.n >> (int)LOGBASE));
    return z;
}

FixedPoint fpand(FixedPoint x, FixedPoint y){
    FixedPoint z((x.n & y.n) >> (int)LOGBASE);
    return z;
}

FixedPoint fpor(FixedPoint x, FixedPoint y){
    FixedPoint z((x.n | y.n) >> (int)LOGBASE);
    return z;
}

FixedPoint fpxor(FixedPoint x, FixedPoint y){
    FixedPoint z((x.n xor y.n) >> (int)LOGBASE);
    return z;
}

/*
int main(){
    FixedPoint x(2.2) , y(3.3);
    x.print(); y.print();
    (add(x,y)).print();
    (sub(x,y)).print();
    (mult(x,y)).print();
    (div(x,y)).print();
    (fpand(x,y)).print();
    (fpor(x,y)).print();
    (fpxor(x,y)).print();
    (fplshift(x,3)).print();

}
*/