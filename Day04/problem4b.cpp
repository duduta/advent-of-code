#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

/*						we consider the central position, that of letter A	
									
	 M S					count the number of intersections between matches 
	 	A						found on asc diagonal and on desc diagonal
	 M S   			
*/ 


//needed in case one wants to print a set from the map of intersection_candidates
template<typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& set)
{
    if (set.empty())
        return out << "{}";
    out << "{ " << *set.begin();
    std::for_each(std::next(set.begin()), set.end(), [&out](const T& element)
    {
        out << ", " << element;
    });
    return out << " }";
}

//(i,j) is the position in the original input coordinate space (row,col)
//it represents the central position of a match (that of the letter A)
//will capture (i,j) in a map where i becomes key and j part of a set
//this will help to faster check for intersections later
void captureMatchesPerOneAscendingDiagonal(std::string& s, int initial_i, int initial_j, std::map<int, std::set<int>>& intersection_candidates) {
	int pos(0);
	while ( (pos=s.find("MAS",  pos)) != std::string::npos) {
		int i(initial_i - pos-1); 
		int j(initial_j + pos+1);
		intersection_candidates[i].insert(j);
	 	pos+=3;
	}
	pos=0;
	while ( (pos=s.find("SAM",  pos)) != std::string::npos) {
		int i(initial_i - pos-1); 
		int j(initial_j + pos+1);
		intersection_candidates[i].insert(j);
	 	pos+=3;
	}
}

int check_intersection(int i, int j, std::map<int, std::set<int>>& intersection_candidates) {
	if(intersection_candidates[i].count(j)) return 1;
	return 0;
}


int findMatchesPerOneDescendingDiagonal_andIntersect(std::string& s, int initial_i, int initial_j, std::map<int, std::set<int>>& intersection_candidates) {
	int pos(0), nr_of_intersections(0);
	while ( (pos=s.find("MAS",  pos)) != std::string::npos) {
		int i(initial_i + pos+1); 
		int j(initial_j + pos+1);
		nr_of_intersections+=	check_intersection(i,j, intersection_candidates);	
		pos+=3;
	}
	pos=0;
	while ( (pos=s.find("SAM",  pos)) != std::string::npos) {
		int i(initial_i + pos+1); 
		int j(initial_j + pos+1);
		nr_of_intersections+=	check_intersection(i,j, intersection_candidates);	
	 	pos+=3;
	}
	return nr_of_intersections;
}


// m is the size of the source vector, n is the length of a string from source m,n are rows,cols
// intersection_candidates contains coordinates of matches found on the ascending diagonal
int diagonalize_descending(std::vector<std::string>& source, int m, int n, std::map<int,std::set<int>>& intersection_candidates) 
{
	int nr_of_intersections(0);
	int initial_i(0), initial_j(n-1);
	//at each step, find initial i,j for beginning of a diagonal
	while(initial_i<m) {
		std::string diagonal_line("");
		int i = initial_i, j = initial_j;
		//complete a diagonal
		while(i<m && j<n) {
			diagonal_line.append(1,source[i].at(j));
			i++; j++;
		}
	
		//we have a complete desc diagonal, we have initial_i, initial_j
		//we may find matches, their corresponding position in the
		//original matrix, and check to see if they intersect with any of the 
		//matches already found on the asc diagonals
		nr_of_intersections += findMatchesPerOneDescendingDiagonal_andIntersect(diagonal_line, initial_i, initial_j, intersection_candidates);

		if(initial_j == 0) initial_i++; 
		else initial_j--;

		//next diagonal line
	}
	return nr_of_intersections;
}

// m is the size of the source vector, n is the length of a string from source m,n are rows,cols
void findAscendingDiagonalMatches(std::vector<std::string>& source, int m, int n, std::map<int,std::set<int>>& intersection_candidates) {
	int initial_i(0), initial_j(0);
	while(initial_j<n) {
		std::string diagonal_line("");
		int i = initial_i, j = initial_j;
		while(i>=0 && j<n) {
			diagonal_line.append(1,source[i].at(j));
			i--; j++;
		}

		//I have initial_i, initial_j and the complete diagonal_line
		//so it's a good place to check for matches and capture 
		//the original (i,j)s = the positions of each matched letter A from 
		//a successful match
		captureMatchesPerOneAscendingDiagonal(diagonal_line, initial_i, initial_j, intersection_candidates);

		if(initial_i < m-1) initial_i++; 
		else initial_j++;

		//next diagonal
	}
}

int main() {
	std::ifstream fis("example-input.txt");
	int answer(0), nr_rows(0), nr_cols(0);
	std::string linie;
	std::vector<std::string> original; //think of it as of a matrix of chars

	if(!fis.is_open()) {
		std::cerr << "Couldn't open input file \n";	
		return 1;
	}

	while (getline(fis, linie)) 
		original.push_back(linie);
	fis.close();		

	nr_rows = original.size();	
	nr_cols = original.at(0).size();
	
	//(i,j) coordinates of matches in the ascending diagonal
	//in the original (not rotated) matrix space
	//i => j
	std::map<int, std::set<int>> intersection_candidates;
	//diagonalize ascending (read the data diagonally - all the acending diagonals)
	//find and capture matches positions (of the letter A from the MAS or SAM)
	findAscendingDiagonalMatches(original, nr_rows, nr_cols, intersection_candidates);	

/*
 	for(auto [k, vals]: intersection_candidates) {
		std::cout << "[" << k << "]=> " << vals << std::endl;
	}
*/
	//diagonalize descending, find matches 
	//and count their intersections with asc diag matches
	// 	that were already found and stored in the intersection_candidates map
	answer = diagonalize_descending(original, nr_rows, nr_cols, intersection_candidates);	

	std::cout << "Answer = " << answer << '\n';
	std::ofstream f("answer4b.txt");
	if (!f) std::cerr << "Couldn't open the output file" << std::endl;
	f << answer;
	 	
	return 0;
}
