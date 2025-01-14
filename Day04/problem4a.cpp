#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int countMatchesPerLine(std::string& s) {
	int n(0), res(0);
	while ( (res=s.find("XMAS",  res)) != std::string::npos) {
		n++; res+=4;
	}
	res=0;
	while ( (res=s.find("SAMX",  res)) != std::string::npos) {
	 	n++; res+=4;
	}
	return n;
}

int countMatches(std::vector<std::string>& v) {
	int n(0);
	for(auto it=v.begin(); it<v.end(); it++) {
		n+=countMatchesPerLine(*it);
	}
	return n;
}

// m is the size of the source vector, n is the length of a string from source 
// may think of (m,n) in terms of (rows,cols) and of the source as of matrix of char
// read the source diagonally - all the "\" diagonals - into a vector of strings
void diagonalize_descending(std::vector<std::string>& source, int m, int n, std::vector<std::string>& diag) 
{
	auto d_line = diag.begin();
	int initial_i(0), initial_j(n-1);
	//at each step, find initial i,j for beginning of a diagonal
	while(initial_i<m) {
		int i = initial_i, j = initial_j;
		//complete a diagonal
		while(i<m && j<n) {
			d_line->append(1,source[i].at(j));
			i++; j++;
		}
		
		if(initial_j == 0) initial_i++; 
		else initial_j--;

		d_line++;
	}
}

// m is the size of the source vector, n is the length of a string from source m,n are rows,cols
// read all the "/" ascending diagonals from source into a vector of strings
void diagonalize(std::vector<std::string>& source, int m, int n, std::vector<std::string>& diag) {
	auto d_line = diag.begin();
	int initial_i(0), initial_j(0);
	while(initial_j<n) {
		int i = initial_i, j = initial_j;
		while(i>=0 && j<m) {
			d_line->append(1,source[i].at(j));
			i--; j++;
		}
		
		if(initial_i < m-1) initial_i++; 
		else initial_j++;

		d_line++;
	}
}


int main() {
	std::ifstream fis("example-input.txt");
	int answer(0), nr_rows(0);
	std::string linie;

	if(!fis.is_open()) {
		std::cerr << "Couldn't open input file \n";	
		return 1;
	}

	//take the first line. I need to know the nr of columns
	if(!getline(fis,linie)) return 1;
	int nr_cols = linie.size();
	//will be used to count vertical matches
	std::vector<std::string> transposed(nr_cols, "");
	
	do	{
		nr_rows++;
		for(int i=0; i<nr_cols; i++) {
			transposed[i].append(1,linie.at(i));	
		}
		//this counts the horizontal matches (both XMAS and SAMX)
		answer += countMatchesPerLine(linie);
	}
	while (getline(fis, linie));
	
	fis.close();		

	//diagonalize 
	std::vector<std::string> ascending_diag(nr_rows + nr_cols-1, "");
	//because it's transposed, nr_cols and nr_rows are passed swapped
	//didn't store the original source, but only its transposed variant
	//it will do just fine - the diagonals are the same, albeit reversed
	//it doesn't matter, since we be both count XMAS and its reverse SMAX
	diagonalize(transposed, nr_cols, nr_rows, ascending_diag);	
	
	//will hold all descending diagonals
	std::vector<std::string> descending_diag(nr_rows + nr_cols-1, "");
	//because it's transposed, nr_cols and nr_rows are passed swapped
	diagonalize_descending(transposed, nr_cols, nr_rows, descending_diag);

	//this counts vertical matches
	answer += countMatches(transposed);	
	//counts ascending diagonal matches
	answer += countMatches(ascending_diag);
	//count descending diagonal matches
	answer += countMatches(descending_diag);
	

	std::cout << "Answer = " << answer << '\n';
	std::ofstream f("answer4a.txt");
	if (!f) std::cerr << "Couldn't open the output file" << std::endl;
	f << answer;
	 	
	return 0;
}
