#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <list>
#include <cctype>
#include <boost/optional/optional.hpp>
#include <boost/optional/optional_io.hpp>


struct position {
	int i{0};
	int j{0};
	position() {}
	position(int x, int y) : i(x), j(y) {}
};
	
position operator-(const position &a, const position &b) {
	position result;
	result.i = a.i-b.i;
	result.j = a.j-b.j;
	return result;
}

position operator*(int v, const position &a) {
	position result;
	result.i = v * a.i;
	result.j = v * a.j;
	return result;
}

std::ostream& operator<<(std::ostream& out, const position& pos)
{
	  return out << "(" << pos.i << "," << pos.j << ")" << std::endl;
}

boost::optional<position> find_antinode(const position &a, const position &b, const int m, const int n) {
	position result;
	result = 2*a - b;
	if(result.i >= 0 && result.j >=0 && result.i < m && result.j <n) return boost::optional<position>(result);
	return boost::optional<position>();
}

//=============================================================

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::list<T>& lista)
{
    if (lista.empty())
        return out << "{}";
    out << "{ " << *lista.begin();
    std::for_each(std::next(lista.begin()), lista.end(), [&out](const T& element)
    {
        out << ", " << element;
    });
    return out << " }";
}
//===============================================================


void find_antinodes(std::list<position>& positions, std::vector<std::vector<bool>> &marked, int m, int n) {
	//will consider all pairs
	for(auto it=positions.begin(); it!=std::prev(positions.end()); it++) {
		std::for_each(std::next(it), positions.end(),
				[it, &marked, m, n] (position &second) {
					boost::optional<position> antinode_pos = find_antinode(*it, second, m, n);
					if(antinode_pos) marked.at(antinode_pos->i)[antinode_pos->j]=true;
					boost::optional<position> antinode_pos_mirror = find_antinode(second, *it, m, n);
					if(antinode_pos_mirror) marked.at(antinode_pos_mirror->i)[antinode_pos_mirror->j]=true;
				}
				);
	} 
}

int count_antinodes(std::vector<std::vector<bool>> &marked) {
	int res(0);
	for(auto row: marked) {
		for(auto elem: row) {
			if(elem) res++;
		}
	}
	return res;
}

//=============================================

int main() {
	int answer(0);
	char c;
	std::string linie;
	std::ifstream fis ("example-input.txt");
	if(!fis.is_open()) return 1;

	if (!getline(fis,linie)) return 1;
	int nr_cols = linie.size();
	int nr_rows = 0;
	std::map<char, std::list<position>> antenas;
	
	do {
		std::stringstream st(linie);
		int current_col=0;
		while(st.get(c)) {
			//if digit or upper or lower case letter
			if(isalnum(c)) {
				position pos{nr_rows, current_col};
				antenas[c].push_back(pos);
			}
			current_col++;
		}	
		nr_rows++;
	} while (getline(fis,linie));
	fis.close();

	//a matrix to mark found antinodes. 	
	std::vector<std::vector<bool>> marked_antinodes {static_cast<size_t>(nr_rows), std::vector<bool>(nr_cols, false)};

	for(auto [antena_name, positions]: antenas) {
		find_antinodes(positions, marked_antinodes, nr_rows, nr_cols);
	}

	answer = count_antinodes(marked_antinodes);
	

	std::cout << "Answer = " << answer << '\n';
	std::ofstream f("answer8a.txt");
	if (!f) std::cerr << "Couldn't open the output file" << std::endl;
	f << answer;
	f.close();	 	

	return 0;
}

