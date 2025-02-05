#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>


struct Combination {
	std::vector<int> data; //the size of this is the k from (n choose k)
	int level; //needed for generating next combination
	const int n; //n from (n choose k)  
							 //ex: (2 choose 7) choses from {0,1} 7 times

	//first combination from the (n choose k) = a vector of k zeros
	Combination(int n, int k): data(std::vector<int>(k,0)), level(k-1), n(n)  {}
	Combination()=delete;

	void printCombination() {
		for(int elem: this->data) {
			std::cout << elem << " ";	
		}	
		std::cout << std::endl;
	}
};

int next_combination(Combination& comb) {
	int k(comb.data.size());
	if (comb.level == -1) return 0;
	if (comb.data[comb.level]< comb.n-1) {
		comb.data[comb.level]++; 
		if(comb.level<k-1) comb.level++;
		return 1;
	}
	comb.data[comb.level] = 0;
	comb.level--;
	while (comb.level>=0 && comb.data[comb.level]==comb.n-1) {
		comb.data[comb.level] = 0;
		comb.level--;
	}
	if (comb.level >=0) {
		comb.data[comb.level]++;
		comb.level=k-1;	
		return 1;
	}
	return 0;
}

long concatenate(const long x,const long y) {
	long pow(10);
	while(y >= pow)
		pow *= 10;
	return x*pow + y;
}

long perform_calculations(const std::vector<long>& numbers, const std::vector<int>& operations_combination) {	
	long result(numbers[0]);
	std::tuple<std::plus<long>, std::multiplies<long>, std::function< long(long,long)> > ops{std::plus<long>(), std::multiplies<long>(), concatenate};
	for (int i=0; i<operations_combination.size(); i++) {
		switch (operations_combination[i]) {
			case 0: 
				result = std::get<0>(ops)(result, numbers[i+1]);
				break;
			case 1: 
				result = std::get<1>(ops)(result, numbers[i+1]);
				break;
			case 2: 
				result = std::get<2>(ops)(result, numbers[i+1]);
				break;
		}
	}
	return result;
}

int check_equation(const std::vector<long>& numbers, const long target) {
	//the number of operations for this equation
 	const	int k = numbers.size() - 1;
	
	//generate combinations of the three operations (+ is 0, * is 1, || is 2)
	//from three operations choose k times
	Combination comb(3, k);
	do {
		if (perform_calculations(numbers, comb.data) == target) return 1;
	} while (next_combination(comb));

	return 0;
}

//================================
int main() {
	long answer(0);
	std::string linie;
	std::ifstream fis ("example-input.txt");
	if(!fis.is_open()) return 1;

	while (getline(fis, linie))
	{	
		std::stringstream st(linie);
		std::vector<long> operands;
		long target(0); long x(0);
		st>>target;
		st.get(); // consume the ':'
		while(st>>x) {
			operands.push_back(x);
		}

		if(check_equation(operands, target)) {
			//there exists a combination of operantors that will produce the target value if applied to the given operands
			answer+= target;
		} 
	}
	fis.close();
	
	std::cout << "Answer = " << answer << '\n';
	std::ofstream f("answer7b.txt");
	if (!f) std::cerr << "Couldn't open the output file" << std::endl;
	f << answer;
	f.close();
	 	

	return 0;
}

