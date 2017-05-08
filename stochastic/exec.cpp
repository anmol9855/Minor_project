#include "exec.h"

//returns state of a system given a vector of inst
state exec_inst_seq(state S,vector<inst>* code){
	for (int i = 0; i < code->size(); ++i)
		S = exec_inst(S,code->at(i));
	return S;
}

//executes a single instruction
state exec_inst(state S,inst I){
	int opcode = I.opcode;
	switch(opcode){
		//mov
		case 0:
			//immediate
			if(I.regIndex1 == -1)
				S.regs[I.regIndex2] = I.op1;
			else
				S.regs[I.regIndex2] = S.regs[I.regIndex1];		
			break;
		//add
		case 1:
			if(I.regIndex1 == -1)
				S.regs[I.regIndex2] += I.op1;
			else
				S.regs[I.regIndex2] += S.regs[I.regIndex1];
			break;
		//sub
		case 2:
			if(I.regIndex1 == -1)
				S.regs[I.regIndex2] -= I.op1;
			else
				S.regs[I.regIndex2] -= S.regs[I.regIndex1];
			break;
		//mult
		case 3:
			if(I.regIndex1 == -1)
				S.regs[I.regIndex2] *= I.op1;
			else
				S.regs[I.regIndex2] *= S.regs[I.regIndex1];
			break;
		//div
		case 4:
			if(I.regIndex1 == -1)
				S.regs[I.regIndex2] /= I.op1;
			else
				S.regs[I.regIndex2] /= S.regs[I.regIndex1];
			break;	
					
		// left shift
		case 5:
			if(I.regIndex1 == -1)
				S.regs[I.regIndex2] = (S.regs[I.regIndex2]) * pow(2,(int)(I.op1));
			else
				S.regs[I.regIndex2] = (S.regs[I.regIndex2]) * pow(2,(int)S.regs[I.regIndex1]);
			break;
		//right shift
		case 6:
			if(I.regIndex1 == -1)
				S.regs[I.regIndex2] = (S.regs[I.regIndex2]) / pow(2,(int)(I.op1));
			else
				S.regs[I.regIndex2] = (S.regs[I.regIndex2]) / pow(2,(int)S.regs[I.regIndex1]);
			break;	
		
		default:
			break;						
	}
	return S;
}

//evaluates performance of a code given a input output sequence and error bound
int satisfiability(test_seq T){
	int count = 0;
	for(int i = 0; i < T.input->size(); ++i)
	{
		state S;
		S.regs[INPUT_INDEX] = T.input->at(i);
		S = exec_inst_seq(S,&T.code);
		double val = S.regs[OUTPUT_INDEX];
		if(val>T.output->at(i)-T.error_bound && val<T.output->at(i)+T.error_bound)
			count++;
	}
	return count;
}




