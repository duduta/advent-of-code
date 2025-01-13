#include <iostream>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>


int main() {
	std::ifstream fis("example-input.txt");
	int answer(0);
	std::string linie;

	if(!fis.is_open()) {
		std::cerr << "Couldn't open the input file \n";	
		return 1;
	}
	
	while (getline(fis, linie))
		{	
			boost::regex re("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)");
			boost::smatch results;
			std::string::const_iterator begin = linie.begin();
			std::string::const_iterator end = linie.end();

			while (boost::regex_search(begin, end, results, re)) {
				int x = boost::lexical_cast<int>(results[1]);
				int y = boost::lexical_cast<int>(results[2]);
				std::cout << "expr = " << results[0] << " x = " << x << " y = " << y << std::endl;
				answer+= x*y; 
				begin = results[0].second;
			}

		}
	fis.close();
		
	std::cout << "Answer = " << answer << '\n';
	std::ofstream f("answer3a.txt");
	if (!f) {
		std::cerr << "Couldn't open the output file" << std::endl;
		return 1;
	}
	f << answer;
	f.close();

	return 0;
}

