#include "cost.h"

//basic function comparing 2 floats
double get_cost(double res,double val,double error){
	double c;
	if((c > val - error) && (c < val+error))
		c = 0.0;
	else
		c = abs(val-res)/error;
	return c;
}

pair<int,double> evaluate_cost(test_seq T){
	int count = 0;
	double cost = 0.0;
	for(int i = 0; i < T.input->size(); ++i)
	{
		state S;
		S.regs[INPUT_INDEX] = T.input->at(i);
		S = exec_inst_seq(S,&T.code);
		double res = S.regs[OUTPUT_INDEX];
		cost+=get_cost(res,T.output->at(i),T.error_bound);
		if(res>T.output->at(i)-T.error_bound && res<T.output->at(i)+T.error_bound)
			count++; 
	}
	return make_pair(count,cost);
}