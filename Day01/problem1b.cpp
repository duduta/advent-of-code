#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>


int main() {
	std::ifstream fis ("example-input.txt");
	int answer(0);
	std::vector<int> a, b;
	std::string linie;

	if(!fis.is_open()) {
		std::cerr << "Couldn't open input.txt file \n";	
		return 1;
	}
	
	while (getline(fis, linie))
	{	
		std::stringstream st(linie);
		int x,y; 
		st>>x; st>>y; 
		a.push_back(x); b.push_back(y);	
	}
	fis.close();
		
	sort(a.begin(), a.end());	
	sort(b.begin(), b.end());	

	auto it_a = a.begin();
	auto it_b = b.begin();
	int count(0);
	while(it_a != a.end() && it_b != b.end()) {

		while(*it_a > *it_b && it_b != b.end()) it_b++;
		while(it_b != b.end() && *it_a == *it_b) {count++; it_b++;}	
		if (count) answer+= *it_a * count;
		int old_a = *it_a;
		it_a++;
		if (it_a != a.end() && old_a != *it_a) count = 0;
	}

	std::cout << "Answer = " << answer << '\n';
	std::ofstream f("answer1b.txt");
	if (!f) {
		std::cerr << "Couldn't open the output file" << std::endl;
		return 1;
	}
	f << answer;
	f.close();

	return 0;
}
