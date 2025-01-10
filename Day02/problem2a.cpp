#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>


bool safeLevels(int a, int b, bool semn) {
	int dif = a-b;
	//and a==b is not considered a safe level
	//important because signbit returns 0 both for a==b and for a>b
	if (dif==0) return false;
	if (semn != signbit(dif) || abs(dif) > 3) return false;
	return true;
}

int main() {
	std::ifstream fis("example-input.txt");
	int answer(0);
	std::string linie;

	if(!fis.is_open()) {
		std::cerr << "Couldn't open the input file \n";	
		return 1;
	}
		
	readLine: while (getline(fis, linie))
	{	
		std::stringstream st(linie);
		int x(0), y(0);
		bool semn(false);	
		st>>x; st>>y;
		//semn will be true for x<y or it will be false for x>=y
		semn = signbit(x-y);
		if (!safeLevels(x,y,semn)) continue;
		x=y;

		while(st>>y) {
			if (!safeLevels(x,y,semn)) goto readLine;
			x=y;
		}		

		answer++;
	}
	fis.close();
		
	std::cout << "Answer = " << answer << '\n';
	std::ofstream f("answer2a.txt");
	if (!f) {
		std::cerr << "Couldn't open the output file" << std::endl;
		return 1;
	}
	f << answer;
	f.close();

	return 0;
}
