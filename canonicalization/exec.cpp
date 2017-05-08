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

//check if instruction is valid
bool isValid(int r1,int r2, int opcode,int len){
	//case of move instruction(eg. mov r0 r0)
	if(opcode == 0 && r1 == r2)
		return false;
	//last instruction not related to output register
	if(len == 1 && r2!=OUTPUT_INDEX)
		return false;
	return true;
}

//TODO- allow for symbolic constants
vector<inst> inst_generator(int active_regs,int len){
	vector<inst> Iset;
	int index1 = min(active_regs,REG_COUNT-1);
	int index2 = min(index1+1,REG_COUNT-1);
	
	//generate all instructions with both operands as registers
	for(int i = 0; i < index1; ++i)
	{
		for (int j = 0; j < index2 ; ++j)
		{
			for (int k = 0; k < INST_COUNT; ++k)
			{
				if(isValid(i,j,k,len)){
					inst I(k,2,i,j,-1);
					Iset.push_back(I);				
				}
			}
		}
	}

	//generate all instructions involving an immediate
	for(int i = 0; i < index1; ++i)
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

int check_all_inst_seq(test_seq T,int len,best_result* b,bool start){
	if(len == 0){
		if(b->nsat == T.input->size())
			return 1;
		int a = satisfiability(T);
		if(b->nsat<a){
			b->code = T.code;
			b->nsat = a;
		}
		return 1;	
	}

	else{
		int count = 0;
		vector<inst> a = inst_generator(T.active_regs,len);
		
		if(start){
			#pragma omp parallel
			{
				best_result b1;
				b1.code = b->code;
				b1.nsat = b->nsat;
				int count1 = 0;
				#pragma omp for
				for(int i = 0; i < a.size(); ++i)
				{
					// if(b->nsat == T.input->size())
					// 	break;
					vector<inst> tmp = T.code;
					tmp.push_back(a[i]);
					int max_index = max(a[i].regIndex1,a[i].regIndex2);
					int active_regs = T.active_regs;
					if(max_index>=T.active_regs) 
						active_regs = max_index+1; 
					test_seq T1(T.input,T.output,T.error_bound,tmp,active_regs);
					count1 += check_all_inst_seq(T1,len-1,&b1,false);
				}

				#pragma omp critical
				{
					count += count1;
					if(b1.nsat > b->nsat){
						b->nsat = b1.nsat;
						b->code = b1.code;
					}
				}	
			}
		}
			
		else{
			for(int i = 0; i < a.size(); ++i)
			{
				if(b->nsat == T.input->size())
					break;
				vector<inst> tmp = T.code;
				tmp.push_back(a[i]);
				int max_index = max(a[i].regIndex1,a[i].regIndex2);
				int active_regs = T.active_regs;
				if(max_index>=T.active_regs) 
					active_regs = max_index+1; 
				test_seq T1(T.input,T.output,T.error_bound,tmp,active_regs);
				count += check_all_inst_seq(T1,len-1,b,false);
			}
		}	

		return count;
	}	
}

int find_best_fit(test_seq T,best_result* b){
	for (int i = 1; i <= MAX_LENGTH; ++i)
	{
		int r = check_all_inst_seq(T,i,b,false);
		cout << r <<"," << i << endl;
		if(b->nsat == T.input->size())
			return i;
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
	//first do a loose check by choosing random indices
	int n_cases = 0;
	int sz = T.input->size();
	for (int i = 0; i < LOOSE_COUNT; ++i)
	{
		int index = rand()%sz;
		state S;
		S.regs[INPUT_INDEX] = T.input->at(index);
		S = exec_inst_seq(S,&T.code);
		double val = S.regs[OUTPUT_INDEX];
		if(val>T.output->at(index)-T.error_bound && val<T.output->at(index)+T.error_bound)
			n_cases++;
	}
	if(n_cases != LOOSE_COUNT){
		return -1;
	}
	cout << "check " <<endl;
	int count = 0;
	for(int i = 0; i < sz; ++i)
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