#include "enum.h"
#include <time.h>

struct numbers{
	double a1;
	double a2;
};

numbers get_num(string line){
	numbers res;
	char * pch;
	char * str = const_cast<char*>(line.c_str());
	pch=strchr(str,',');
	int pos = (int) (pch-line.c_str());
	string str1 = line.substr(0,pos);
	string str2 = line.substr(pos+1,line.size());
	res.a1 = atof(str1.c_str());
	res.a2 = atof(str2.c_str());
	return res;
}

int main(){
	string line;
	clock_t start, end;
	vector<double> input;
	vector<double> output;
	double error_bound;
	cout<<"Enter error bound"<<endl;
	cin >> error_bound;
	input.resize(1001);
	output.resize(1001);
	ifstream myfile ("../functions/poly1.txt");
	int index = 0;
	if (myfile.is_open())
	{
		while (getline (myfile,line))
		{
			numbers N = get_num(line);
			input[index] = N.a1;
			output[index] = N.a2;
			index++;
		}
		myfile.close();
	}

	vector<inst> code;
	best_result b;
	test_seq T(&input,&output,error_bound,code,0);

	start = clock();
	
	int res = check_all_inst_seq(T,3,&b);
	
	end = clock();
	double totalTime = (double)(end-start)/CLOCKS_PER_SEC;
	
	cout<<"Total instruction sequences checked - "<<res<<endl;
	cout<<"Number of inputs satisfied - "<<b.nsat<<endl;
	cout<<"Instruction Sequence :- "<<endl;
	print_instruction_seq(b.code);
	cout<<"Time taken - "<<totalTime<<endl;
	return 0;
}