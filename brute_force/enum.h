#ifndef ENUM_H
#define ENUM_H

#include "isa.h"

struct test_seq{
	vector<double>* input;
	vector<double>* output;
	double error_bound;
	vector<inst> code;
	test_seq(vector<double>* inp,vector<double>* out,double error,vector<inst> c){
		input = inp;
		output = out;
		error_bound = error;
		code = c;
	}
};

struct best_result{
	vector<inst> code;
	int nsat;
	best_result(){
		nsat = 0;
	}
};

//returns state of a system given a vector of inst
state exec_inst_seq(state s,vector<inst>* code);
state exec_inst(state S,inst I);
vector<inst> inst_generator();
int check_all_inst_seq(test_seq T,int len,best_result* b);
void print_instruction(inst I);
void print_instruction_seq(vector<inst> code);
int satisfiability(test_seq T);

#endif