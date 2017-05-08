#ifndef ISA_H
#define ISA_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include "fp.h"

#define REG_COUNT 4
#define INPUT_INDEX 0
#define OUTPUT_INDEX 1
#define INST_COUNT 7

using namespace std;

struct state{
	vector<double> regs;
	state(){
		regs.resize(REG_COUNT,1);
	}
};

/*
mov,add,sub,mult,div,lshift,rshift*/
struct inst{
	int opcode;
	int noperands;
	int regIndex1;		//register index of first operand(-1 if immediate)
	int regIndex2;		//register index of second operand
	double op1;
	inst(int c,int n,int i1,int i2,double i){
		opcode = c;
		noperands = n;
		regIndex1 = i1;
		regIndex2 = i2;
		op1 = i;
	}
	inst(){};
};

#endif


