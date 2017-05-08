#ifndef EXEC_H
#define EXEC_H

#include "isa.h"
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <algorithm>

#define MAX_LENGTH 10
#define LOOSE_COUNT 10
#define EPSILON 0.00001

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
	best_result(){
		nsat = 0;
	}
};

//returns state of a system given a vector of inst
state exec_inst_seq(state s,vector<inst>* code);
state exec_inst(state S,inst I);
bool isValid(int r1,int r2, int opcode,int len);
vector<inst> inst_generator(int active_regs,int len);
int check_all_inst_seq(test_seq T,int len,best_result* b, bool start);
int find_best_fit(test_seq T,best_result* b);
void print_instruction(inst I);
void print_instruction_seq(vector<inst> code);
int satisfiability(test_seq T);

#endif