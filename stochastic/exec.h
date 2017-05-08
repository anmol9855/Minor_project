#ifndef EXEC_H
#define EXEC_H

#include "isa.h"
#include "math.h"
#include <limits>
#include <algorithm>

struct test_seq{
	vector<double>* input;
	vector<double>* output;
	double error_bound;
	vector<inst> code;
	int active_regs;
	test_seq(vector<double>* inp,vector<double>* out,double error,vector<inst> c,int active){
		input = inp;
		output = out;
		error_bound = error;
		code = c;
		active_regs = active;
	}
};

struct best_result{
	vector<inst> code;
	int nsat;
	double cost;
	best_result(){
		nsat = 0;
		cost = numeric_limits<double>::max();
	}
};

//returns state of a system given a vector of inst
state exec_inst_seq(state s,vector<inst>* code);
state exec_inst(state S,inst I);
int satisfiability(test_seq T);

#endif