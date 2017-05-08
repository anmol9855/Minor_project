#include "exec.h"
#include "stoke.h"
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

int main(int argc, char *argv[]){
	string line;
	srand(time(NULL));
	clock_t start, end;
	vector<double> input;
	vector<double> output;
	double error_bound;
	input.resize(1001);
	output.resize(1001);

	if(argc != 3){
		cout << "usage: "<<argv[0] << " <input file> <error_bound>" <<endl;
		return -1;
	}

	ifstream myfile (argv[1]);
	error_bound = atof(argv[2]);
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
	test_seq T(&input,&output,error_bound,code,1);

	start = clock();
	/*
	int count = stochastic_search(T,&b);
	
	end = clock();
	double totalTime = (double)(end-start)/CLOCKS_PER_SEC;
	
	cout<<"Total instruction sequences checked - "<<count<<endl;
	cout<<"Number of inputs satisfied - "<<b.nsat<<endl;
	cout<<"Instruction Sequence :- "<<endl;
	print_instruction_seq(b.code);
	cout<<"Time taken - "<<totalTime<<endl;
	*/
	FixedPoint x(2.2) , y(3.3);
    x.print(); y.print();
    (add(x,y)).print();
    (sub(x,y)).print();
    (mult(x,y)).print();
    (div(x,y)).print();
    (fpand(x,y)).print();
    (fpor(x,y)).print();
    (fpxor(x,y)).print();
    (fplshift(x,3)).print();
	return 0;
}