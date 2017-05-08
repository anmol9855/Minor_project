#include "util_inst.h"
#include <map> 
#include <assert.h>

void canonicalize(vector<inst>* code){
	map<int,int> reg_map;
	map<int,int>::iterator it;

	int active_regs = 0;
	for(int i = 0; i < code->size(); ++i)
	{
		int reg1 = code->at(i).regIndex1;
		int reg2 = code->at(i).regIndex2;
		it = reg_map.find(reg1);
		if(reg1 != -1){
			if(it!= reg_map.end())
				code->at(i).regIndex1 = it->second;
			//find renaming and insert
			else{
				assert(active_regs<REG_COUNT);
				reg_map.insert(pair<int,int>(reg1,active_regs));
				code->at(i).regIndex1 = active_regs;
				active_regs++;
			}
		}
		it = reg_map.find(reg2);
		if(it!= reg_map.end())
			code->at(i).regIndex2 = it->second;
		//find renaming and insert
		else{
			assert(active_regs<REG_COUNT);
			reg_map.insert(pair<int,int>(reg2,active_regs));
			code->at(i).regIndex2 = active_regs;
			active_regs++;
		}	
	}
}

inst generate_random_inst(){
	int opcode = rand()%INST_COUNT;
	int noperands = rand()%2+1;
	int regIndex1;
	double op1;
	int regIndex2 = rand()%REG_COUNT;
	if(noperands == 1){
		op1 = rand()%2;
		regIndex1 = -1;
	}
	else{
		op1 = -1;
		regIndex1 = rand()%REG_COUNT;
	}
	inst I(opcode,noperands,regIndex1,regIndex2,op1);
	return I;
}

vector<inst> generate_random_inst_seq(int length)
{
	vector<inst> res;
	for(int i = 0; i < length; ++i)
	{
		inst I = generate_random_inst();
		res.push_back(I);
	}
	canonicalize(&res);
	return res;
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