#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

enum class Direction {
	up,
	right,
	down,
	left	
};

Direction& operator++(Direction &d) {
	return d = (d==Direction::left) ? Direction::up : static_cast<Direction>(static_cast<int>(d)+1);
}

std::tuple<int,int,bool> try_to_move_guardian(int from_i, int from_j, Direction d, int nr_rows, int nr_cols) {
	int i(from_i),j(from_j);
	bool still_inside(true);
	switch (d) {
		case Direction::up:
			i--; if(i==-1) still_inside=false;
			break;
		case Direction::right:
			j++; if(j==nr_cols) still_inside=false;
			break;
		case Direction::down:
			i++; if(i==nr_rows) still_inside=false;
			break;
		case Direction::left:
			j--; if(j==-1) still_inside=false;
			break;
	}
	return std::make_tuple(i, j, still_inside);
}

int main() {
	//answer starts at 1 to account for the first position of the guard (which he implicitly visited)
	int answer(1), nr_rows(0), nr_cols(0);
	int start_i(-1), start_j(-1);
	std::string linie;
	std::ifstream fis ("example-input.txt");
	if(!fis.is_open()) return 1;
	std::vector<std::string> input;

	int res(-1);
	while (getline(fis, linie))
	{	
		if ((res=linie.find('^', 0)) != std::string::npos) {
			start_i = nr_rows;
			start_j = res;
			linie.at(res)='X';
		}
		input.push_back(linie);
		nr_rows++;
	}
	fis.close();
	
	if(!input.size() || start_i == -1 || start_j == -1) return 1;
	nr_cols = input.at(0).size();

	int i(start_i), j(start_j);	
	Direction dir(Direction::up);

	while(1) {
		auto [new_i, new_j, still_inside] = try_to_move_guardian(i,j, dir, nr_rows, nr_cols);
		if(!still_inside) break;
		//there's a fence, need to change direction
		if(input[new_i].at(new_j) == '#') {
			++dir;
		}
		//there's no fence in front of the guardian
		else {
			//commit move
			i=new_i; j=new_j;
			//if haven't been there yet
			if(input[i].at(j) != 'X') {
				input[i].at(j) = 'X';
				answer++;
			}
		}
	}


	std::cout << "Answer = " << answer << '\n';
	std::ofstream f("answer6a.txt");
	if (!f) std::cerr << "Couldn't open the output file" << std::endl;
	f << answer;
	f.close();	 	

	return 0;
}

