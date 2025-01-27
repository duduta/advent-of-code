#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <numeric> 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

/*
 The trick to this problem is not to imply transitivity of the rules. If you do so and consider ALL the rules (including those implied by transitivity) when checking for the right order of pages, you will get cycles. 
 There is no mention of transitivity in the problem description.
 When checking if an update is in order, only use the explicit rules pertinent to that update. 
 */

//=====================================================

bool check_order(int x, std::vector<int>& update, std::map<int, std::set<int>>& rules) {
	auto end = std::prev(update.end()); 
	//iterate through all but last element, it's redundant to check for an edge from x to x.
	for(auto it = update.begin(); it!=end; it++) {
		//check if there is a rule x|(*it)
		if (rules[x].count(*it)) return false;
	}
return true;
}

//sorts the indexes corresponding to page numbers from an update
void topo(const std::size_t nr_of_nodes, const std::vector<std::pair<int,int>>& edges, std::vector<int>& topo_sorted)  {
		typedef boost::adjacency_list<> Graph;
		Graph g{edges.cbegin(), edges.cend(), nr_of_nodes};

		//should try catch IRL, in case there are cycles in the input data
		boost::topological_sort(g, std::back_inserter(topo_sorted));
}


void put_update_in_order(const std::vector<int>& update, const std::map<int,std::set<int>>& rules, std::vector<int>& ordered_update) {
	auto n(update.size()); //nr. of nodes
	std::vector<int> indexes(update.size());
	std::iota(indexes.begin(), indexes.end(),0);
	std::map<int, int> sorted_nodes_with_orig_index;

	//make a map from node to index (value from update vector => index)
	std::transform(update.begin(), update.end(), indexes.begin(), std::inserter(sorted_nodes_with_orig_index, sorted_nodes_with_orig_index.end()), [](int a, int b)
			{
				return std::make_pair(a,b);
			});
	std::set<int> sorted_nodes(update.begin(),update.end());

	//edges will contain all rules pertinent to the current update
	//it's important to consider only rules for the current update
	//don't imply transitivity, or you'll get cycles
	std::vector<std::pair<int,int>> edges;
	for(auto [node, i]: sorted_nodes_with_orig_index) {
		//if there is a rule "node|something"
		if(rules.count(node)) {
			std::vector<int> intersection;
			//filter the rules for "node|(only nodes from current update)"
			std::set_intersection(rules.at(node).begin(), rules.at(node).end(),
					sorted_nodes.begin(), sorted_nodes.end(),
					std::back_inserter(intersection));	
			//for all pertinent rules, add an edge node|elem 
			for(int elem: intersection) {
				edges.push_back(std::make_pair(i,sorted_nodes_with_orig_index[elem]));
			}	
		}

	}
 	//we now have all pertinent rules (they will be edges in our graph) we can make the graph

	//this will hold the indexes in the correct order
	std::vector<int> indexes_topo_sorted;
	topo(n, edges, indexes_topo_sorted);

	//populate the resulting vector with the values, in the correct order of the pages
	for(int i: indexes_topo_sorted) 
		ordered_update.push_back(update[i]);

}

//=============================================================

int main()
{
	int answer(0);
	std::string linie;
	std::ifstream fis ("example-input.txt");
	if(!fis.is_open()) return 1;
	std::map<int, std::set<int>> rules;
	
	//process input rules
	while(getline(fis, linie) && linie!="") {
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
			if(update_is_in_right_order && !check_order(x, update, rules)) 
				update_is_in_right_order = false;
		}	

		if(!update_is_in_right_order) {
			std::vector<int> ordered_update;	
			put_update_in_order(update, rules, ordered_update);
			//exit;
			answer+= ordered_update[ordered_update.size()/2];
		}
	}

	std::cout << "Answer = " << answer << '\n';
	std::ofstream f("answer5b.txt");
	if (!f) std::cerr << "Couldn't open the output file" << std::endl;
	f << answer;
	f.close();

	return EXIT_SUCCESS;	
}
