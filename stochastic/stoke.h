#ifndef STOKE_H
#define STOKE_H

#include "isa.h"
#include "exec.h"
#include "cost.h"
#include "util_inst.h"
#include <cstdlib>

#define MAX_START_LENGTH 5
#define COST_WEIGHT 0.000005
#define MAX_CODE_LENGTH 10
#define OPCODE_M 0.17
#define OPERAND_M 0.49
#define SWAP_M 0.17
#define REPLACE_M 0.17
#define ITER 100000
#define N_RESTARTS 10

extern double prob_opcode;
extern double prob_operand;
extern double prob_swap;
extern double prob_replace;

void initialize_probabilities();
vector<inst> generate_initial_code(int length);
int get_next_code(vector<inst>* code);
void mutate_operand(vector<inst>* code);
void mutate_opcode(vector<inst>* code);
void swap_inst(vector<inst>* code);
void replace_inst(vector<inst>* code);
int stochastic_search(test_seq T,best_result* b);

#endif