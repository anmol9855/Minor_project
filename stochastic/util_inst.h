#ifndef INST_H
#define INST_H

#include "isa.h"
#include <cstdlib>

void canonicalize(vector<inst>* code);
inst generate_random_inst();
vector<inst> generate_random_inst_seq(int length);
void print_instruction(inst I);
void print_instruction_seq(vector<inst> code);

#endif