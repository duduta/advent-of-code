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
		int x(0),y(0); 
		st>>x; st>>y; 
		a.push_back(x); b.push_back(y);	
	}
	fis.close();
		
	sort(a.begin(), a.end());	
	sort(b.begin(), b.end());	

	int n(a.size());
	for(int i=0; i<n; i++) 
		answer += abs(a[i]-b[i]);
	
	std::cout << "Answer = " << answer << '\n';
	std::ofstream f("answer1a.txt");
	if (!f) {
		std::cerr << "Couldn't open the output file" << std::endl;
		return 1;
	}
	f << answer;
	f.close();

	return 0;
}
