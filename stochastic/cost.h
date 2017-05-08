#ifndef COST_H
#define COST_H

#include "isa.h"
#include "exec.h"

double get_cost(double res,double val,double error);
pair<int,double> evaluate_cost(test_seq T);

#endif