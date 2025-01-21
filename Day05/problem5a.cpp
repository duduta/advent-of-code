#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <vector>

//print a set of T, used it for debugging
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

    

bool check_order(int x, std::vector<int>& update, std::map<int, std::set<int>>& rules) {
	auto end = std::prev(update.end()); 
	//iterate through all but last element, it's redundant to check for an edge from x to x.
	for(auto it = update.begin(); it!=end; it++) {
		//check if there is a rule x|v 
		if (rules[x].count(*it)) return false;
	}
	return true;
}

//=======================================

int main() {
	int answer(0);
	std::string linie;
	std::ifstream fis ("example-input.txt");
	if(!fis.is_open()) return 1;
	std::map<int, std::set<int>> rules;
	
	//process rules first. read input till the first blank line	
	while (getline(fis, linie) && linie!="")  {
		int v(0), w(0);
		std::stringstream st(linie);
		st >> v; st.get(); st >> w; 
		rules[v].insert(w);
	}

	//process updates lines
	while(getline(fis, linie) && linie!="") {
		std::vector<int> update;
		std::string page_number;
		bool update_is_in_right_order(true);
		std::stringstream st(linie);

		//gather page numbers from a line
		while (getline(st, page_number, ',')) {
			int x(stoi(page_number));
			update.push_back(x);
			if(!check_order(x, update, rules)) {
				update_is_in_right_order = false;
				break;
			}  
		}
	

		if(update_is_in_right_order) 
			answer+= update[update.size()/2];

	}


	std::cout << "Answer = " << answer << '\n';
	std::ofstream f("answer5a.txt");
	if (!f) std::cerr << "Couldn't open the output file" << std::endl;
	f << answer;
	f.close();

}
