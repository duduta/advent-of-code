#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>


//perform calculation for a given combination of operators
long perform_calculations(const std::vector<long>& numbers, const std::bitset<32>& combination) {
	int nr_of_operations(numbers.size() - 1);
	long result(numbers[0]);
	if (!nr_of_operations) return result; //if there's only one number

	std::tuple<std::plus<long>, std::multiplies<long> > ops{std::plus<long>(), std::multiplies<long>()};

	for(int t= nr_of_operations-1; t>=0; t--) {
		switch (combination[t]) {
			case 0:
				result = std::get<0>(ops)(result,numbers[nr_of_operations-t]);
				break;
			case 1:
				result = std::get<1>(ops)(result,numbers[nr_of_operations-t]);
				break;
			}
	}
	return result;
}

//does it have a combination of operators for which we obtain the given result?
int check_equation(const std::vector<long>& numbers, const long target) {
	
	//the number of operations for this equation
 	const	int n = numbers.size() - 1;
	//generate combinations of the two operations (+ is 0, * is 1)
	//count to n^6-1, the binary representation of each i will give us one combination
	for(int i=0; i<std::pow(2,n); i++) {
		//binary representation will encode my succession of operations
		const auto combination = std::bitset<32>(i);
		if (perform_calculations(numbers, combination) == target) return 1;
	}

	return 0;

}

//====================================================

int main() {
	long answer(0);
	int count_not_working(0);
	std::string linie;
	std::ifstream fis ("example-input.txt");
	if(!fis.is_open()) return 1;

	while (getline(fis, linie))
	{	
		std::stringstream st(linie);
		std::vector<long> operands;
		long target(0); long x(0);
		st>>target;
		st.get(); // this consumes the ':'
		while(st>>x) {
			operands.push_back(x);
		}

		if(check_equation(operands, target)) {
			//found a combination of operations that work
			answer+= target;
		} 
		else {
			count_not_working++;
		}	
	}
	fis.close();

	std::cout << "Answer = " << answer << '\n';
	std::ofstream f("answer7a.txt");
	if (!f) std::cerr << "Couldn't open the output file" << std::endl;
	f << answer;
	f.close(); 	

	return 0;
}

