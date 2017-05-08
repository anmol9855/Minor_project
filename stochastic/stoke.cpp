#include "stoke.h"
#include <map> 
#include <assert.h>

double prob_opcode,prob_operand,prob_swap,prob_replace;

void initialize_probabilities(){
	prob_opcode = OPCODE_M;
	prob_operand = prob_opcode+OPERAND_M;
	prob_swap = prob_operand+SWAP_M;
	prob_replace = 1.0;
}

vector<inst> generate_initial_code(){
	int length = rand()%MAX_START_LENGTH+1;
	vector<inst> res;
	res = generate_random_inst_seq(length);
	return res;
}

int get_next_code(vector<inst>* code){
	//choose operation to perform
	float prob = (float)rand()/(float) RAND_MAX;
	if(prob < prob_opcode){
		mutate_opcode(code);
		// canonicalize(code);
		return 0;
	}
	else if(prob < prob_operand){
		mutate_operand(code);
		// canonicalize(code);
		return 1;
	}
	else if(prob < prob_swap){
		swap_inst(code);
		// canonicalize(code);
		return 2;
	}
	else{
		replace_inst(code);
		// canonicalize(code);
		return 3;
	}
}

void mutate_operand(vector<inst>* code){
	int len = code->size();
	assert(len!=0);
	int i = rand()%len;
	int opIndex = rand()%2;
	int regIndex;

	if(opIndex == 1){
		regIndex = (code->at(i).regIndex2+1+rand()%(REG_COUNT-1))%REG_COUNT;
		code->at(i).regIndex2 = regIndex;	
	}
	else if(opIndex == 0 && code->at(i).noperands == 2){
		regIndex = (code->at(i).regIndex1+1+rand()%(REG_COUNT-1))%REG_COUNT;
		code->at(i).regIndex1 = regIndex;	
	}
	else{
		if(code->at(i).op1 < 0.5)
			code->at(i).op1 = 1.0;
		else
			code->at(i).op1 = 0.0;
	}
}

void mutate_opcode(vector<inst>* code){
	int len = code->size();
	int i = rand()%len;
	int opcode = (code->at(i).opcode+1+rand()%(INST_COUNT-1))%INST_COUNT;
	code->at(i).opcode = opcode;
}

void swap_inst(vector<inst>* code){
	int len = code->size();
	if(len == 1)
		return;
	int index1 = rand()%len;
	int index2 = (index1+1+rand()%(len-1))%len;
	inst tmp = code->at(index1);
	code->at(index1) = code->at(index2);
	code->at(index2) = tmp;
}

//allow for insertions and delete
void replace_inst(vector<inst>* code){
	//0 for replace, 1 for add, 2 for delete
	int mode = rand()%3;
	int len = code->size();

	float ratio1 = (float)(MAX_CODE_LENGTH-len)/MAX_CODE_LENGTH;
	float ratio2 = (float)(len-1)/MAX_CODE_LENGTH;
	// cout << "Ratios" << ratio1 << "," << ratio2 << endl;
	float prob_add_inst =  1 - exp(-ratio1);
	float prob_del_inst = 1-exp(-ratio2);
	//replace	
	if(mode == 0){
		//generate a random instruction
		inst I = generate_random_inst();
		//choose instruction to replace
		int index = rand()%code->size();
		code->at(index) = I;
		return;
	}

	//insert
	else if(mode == 1){
		float select_sample = (float)rand()/(float)RAND_MAX;
		if(prob_add_inst<select_sample && len < MAX_CODE_LENGTH){
			inst I = generate_random_inst();
			code->push_back(I);
		}
		return;
	}	

	//delete
	else{
		float select_sample = (float)rand()/(float)RAND_MAX;
		if(prob_del_inst<select_sample && len>1){ 
			int index = rand()%(code->size());
			code->erase(code->begin()+index);
		}
	}			
}


int stochastic_search(test_seq T,best_result* b){
	vector<inst> code,tmp;
	double cost,new_cost;
	pair<int,double> result;
	initialize_probabilities();
	int count = 0;
	int no_update_limit = 1000;

	for (int rep = 0; rep < N_RESTARTS; ++rep)
	{
		int no_cost_updates = 0;		//used for deciding when random restart should be done
		cout << "Restarting" << endl;
		code = generate_initial_code();
		
		int it = 0;
		T.code = code;
		
		result = evaluate_cost(T);
		cost = result.second;
		if(rep==0 || cost < b->cost){
			b->code = code;
			b->cost = cost;
			b->nsat = result.first;
		}	
		if(b->nsat == T.input->size())
			return count;
		/*
		cout << "Initial code sequence:- " << endl;
		print_instruction_seq(code);
		cout << "Initial Cost: " << cost <<endl;
		cout << "Initial number of inputs satisfied: "<< result.first <<endl;  	
		*/	

		while(it < ITER && no_cost_updates<no_update_limit){
			tmp = code;
			int move = get_next_code(&code);
			T.code = code;

			result = evaluate_cost(T);
			new_cost = result.second;

			float r = exp(-COST_WEIGHT*(new_cost-cost));
			float prob = min((float)1,r);
			// cout << move <<"," << code.size()<<"," << cost << "," << new_cost << "," << r << "," << prob << endl;

			float select_sample = (float)rand()/(float)RAND_MAX;

			// no update
			if(prob < select_sample)
				code = tmp;
			else{
				// cout << "accepted" << endl;
				if(cost > new_cost){
					cout << cost << "," << new_cost << "," << prob << endl;
					
				}
				no_cost_updates = (cost>new_cost)? 0:(no_cost_updates+1);
			
				cost = new_cost;
				if(cost < b->cost){
					b->code = code;
					b->nsat = result.first;
					b->cost = cost;
					if(b->nsat == T.input->size())
						return count;
				}
			}
			it++;
			count++;
		}
		cout << "Instructions checked:- " << it << endl;
	}		
	return count;
}