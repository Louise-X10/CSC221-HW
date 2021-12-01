#include "chromosome.hh"
#include "cities.hh"
#include "deme.hh"
#include<cassert>
#include<fstream>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <numeric>


bool is_valid(){
    auto range =7;
    std::vector<int> permute({1,2,3,4,5,6,7});

    auto it1 = std::find_if(permute.begin(),permute.end(),[range](auto x){return x > range;}); // returns last if no elt exceeds range
    bool in_range = (it1 == permute.end());

    std::sort(permute.begin(),permute.end());
    auto it2 = std::unique(permute.begin(),permute.end()); // returns last if no duplicates are removed
    bool no_repeat = (it2 == permute.end());
    return (in_range and no_repeat);
}

int main() {    
    const auto cities = Cities("five.tsv");
    std::cout << cities.total_path_distance({ 0, 1, 2, 3, 4 }) << "\n"; // Should be 48.8699
    std::cout << cities.total_path_distance({ 3, 2, 4, 0, 1 }) << "\n"; // Should be 53.43
    
    Chromosome parent1(&cities);
    std::cout << "fitness" << parent1.get_fitness() << "\n";
    
    Chromosome parent2(&cities);
    //std::pair<Chromosome*, Chromosome*> children = parent1.recombine(&parent2);
    std::cout << "parent 2 order size is " << parent2.get_ordering()[1] << "\n"; 

    if (is_valid()){
	std::cout<< "is valid";
    } else {
	std::cout<< "is not valid";
    }

    return 0;
}

