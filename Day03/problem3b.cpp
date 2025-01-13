#include <iostream>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>


int main() {
	std::ifstream fis("example-input.txt");
	int answer(0);
	std::string linie;
	bool enabled(true);
	boost::regex do_pat("do\\(\\)");	
	boost::regex dont_pat("don\'t\\(\\)");	
	boost::regex mul_pat("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)");

	if(!fis.is_open()) {
		std::cerr << "Couldn't open the input file \n";	
		return 1;
	}
	
	while (getline(fis, linie))
	{
		boost::smatch results;
		std::string::const_iterator begin = linie.begin();
		std::string::const_iterator end_of_line = linie.end();
		std::string::const_iterator end = end_of_line; 
			
		while(begin != end_of_line) {	

			if(!enabled) {
				//search for next "do()"
				if(boost::regex_search(begin, end_of_line, results, do_pat)) {
					enabled = true;
					begin = results[0].second;
				} else end = end_of_line; //nothing to be done on this line
			}
				
			if(enabled) {
				//search for next "dont()"
				if(boost::regex_search(begin, end_of_line, results, dont_pat))
				{
					enabled = false;
					end = results[0].second;
				} else end = end_of_line; //have work to do till the end of this line, will go on next line with enabled=true
																		
				//search for all "mul()"s from the just found [begin,end) range
				while (boost::regex_search(begin, end, results, mul_pat)) {
					int x = boost::lexical_cast<int>(results[1]);
					int y = boost::lexical_cast<int>(results[2]);
					//std::cout << "expr = " << results[0] << " x = " << x << " y = " << y << std::endl;
					answer+= x*y; 
					begin = results[0].second; 
				}
	
			}
				
			begin = end; 
		} 
	}
	fis.close();
		
	std::cout << "Answer = " << answer << '\n';
	std::ofstream f("answer4b.txt");
	if (!f) {
		std::cerr << "Couldn't open the output file" << std::endl;
		return 1;
	}
	f << answer;
	f.close();

	return 0;
}

