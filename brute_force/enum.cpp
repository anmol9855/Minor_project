#include "enum.h"

//returns state of a system given a vector of inst
state exec_inst_seq(state S,vector<inst>* code){
	for (int i = 0; i < code->size(); ++i)
		S = exec_inst(S,code->at(i));

	return S;
}

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
		/*			
		// left shift
		case 5:
			if(I.regIndex1 == -1)
				S.regs[I.regIndex2] = (S.regs[I.regIndex2]) << (int)(I.op1);
			else
				S.regs[I.regIndex2] += (S.regs[I.regIndex2]) << (int)S.regs[I.regIndex1];
			break;
		//right shift
		case 6:
			if(I.regIndex1 == -1)
				S.regs[I.regIndex2] = (S.regs[I.regIndex2]) >> (int)I.op1;
			else
				S.regs[I.regIndex2] += (S.regs[I.regIndex2]) >> (int)S.regs[I.regIndex1];
			break;	
		*/
		default:
			break;						
	}
	return S;
}

//TODO- allow for symbolic constants
vector<inst> inst_generator(){
	vector<inst> Iset;
	
	//generate all instructions with both operands as registers
	for(int i = 0; i < REG_COUNT; ++i)
	{
		for (int j = 0; j < REG_COUNT; ++j)
		{
			for (int k = 0; k < INST_COUNT; ++k)
			{
				inst I(k,2,i,j,-1);
				Iset.push_back(I);				
			}
		
		}
	}

	//generate all instructions involving an immediate
	for(int i = 0; i < REG_COUNT; ++i)
	{
		for (int k = 0; k < INST_COUNT; ++k)
		{
			inst I1(k,2,-1,i,0);
			Iset.push_back(I1);
			inst I2(k,2,-1,i,1);
			Iset.push_back(I2);				
		}
	}
	return Iset;
}

int check_all_inst_seq(test_seq T,int len,best_result* b){
	if(len == 0){
		// if(b->nsat == T.input->size())
		// 	return 1;
		// int a = satisfiability(T);
		// if(b->nsat<a){
		// 	b->code = T.code;
		// 	b->nsat = a;
		// }
		return 1;	
	}
	else{
		int count = 0;
		vector<inst> a = inst_generator();
		for(int i = 0; i < a.size(); ++i)
		{
			// if(b->nsat == T.input->size())
			// 	break;
			vector<inst> tmp = T.code;
			tmp.push_back(a[i]);
			test_seq T1(T.input,T.output,T.error_bound,tmp);
			// if(len == 3)
			// 	cout<<"update"<<endl;
			count += check_all_inst_seq(T1,len-1,b);
		}
		return count;
	}	
}



void print_instruction(inst I){
	string s = "";
	switch(I.opcode){
		case 0:
			s+="mov ";
			break;
		case 1:
			s+="add ";
			break;
		case 2:
			s+="sub ";
			break;
		case 3:
			s+="mult ";
			break;
		case 4:
			s+="div ";
			break;
		case 5:
			s+="lshift ";
			break;
		case 6:
			s+="rshift ";
			break;
		default:
			break;							
	}
	if(I.regIndex1!= -1){
		s += "r"+to_string(I.regIndex1);
	}
	else{
		s += to_string(I.op1);
	}
	s += " r"+to_string(I.regIndex2);
	cout<<s<<endl;
}

void print_instruction_seq(vector<inst> code){
	for(int i = 0; i < code.size(); ++i)
	{
		print_instruction(code[i]);
	}
}

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

