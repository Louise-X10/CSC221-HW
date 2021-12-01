/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */

#include "chromosome.hh"
#include "deme.hh"
#include<cassert>

// Generate a Deme of the specified size with all-random chromosomes.
// Also receives a mutation rate in the range [0-1].
Deme::Deme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
    : mut_rate_(mut_rate),
     generator_(std::chrono::system_clock::now().time_since_epoch().count())
   {    
    for (unsigned i = 0; i< pop_size; i++) {
	pop_.push_back(  new Chromosome(cities_ptr)); // random chromosome from given cities
    }
        
}

// Clean up as necessary
Deme::~Deme()
{    
    int size = pop_.size();
    for (int i = 0; i<size; i++) {
	delete pop_[i];
    } 
}

// Evolve a single generation of new chromosomes, as follows:
// We select pop_size/2 pairs of chromosomes (using the select() method below).
// Each chromosome in the pair can be randomly selected for mutation, with
// probability mut_rate, in which case it calls the chromosome mutate() method.
// Then, the pair is recombined once (using the recombine() method) to generate
// a new pair of chromosomes, which are stored in the Deme.
// After we've generated (pop_size) new chromosomes, we delete all the old ones.
void Deme::compute_next_generation()
{
    std::vector<Chromosome*> pop;
    int iter = pop_.size()/2; // rounds down for odd
    for (auto i=0; i < iter ; i++) {
	Chromosome* parent1 = select_parent();
	Chromosome* parent2 = select_parent();
	std::vector<Chromosome*> parents;
	parents.push_back(parent1);
	parents.push_back(parent2);

	std::uniform_real_distribution<double> distribution(0.0, 1.0); // random double generator from [0,1]

	
	// mutate parents by mut rate
	for (auto j=0; j<2; j++) {
	    int random = distribution(generator_); // a random number between [0,1.0]
            if (random < mut_rate_) {
	    parents[j]->mutate();
	    }
	}
	
	// generate pair of new chromosomes, store in pop
	std::pair<Chromosome*, Chromosome*> children = parent1->recombine(parent2);
	pop.push_back(children.first);
	pop.push_back(children.second);
    }

    // destroy old chromosomes
    //int size = pop_.size();
    for (auto child : pop_) {
	delete child;
    }
    // replace with new chromosomes
    std::swap(pop_, pop);
}

bool compare(Chromosome* a, Chromosome* b) {
    return a->get_fitness() < b->get_fitness();
}

// Return a copy of the chromosome with the highest fitness.
const Chromosome* Deme::get_best() const
{
    assert(pop_.size() > 1);
    return *std::max_element(pop_.begin(), pop_.end(), compare);
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.
Chromosome* Deme::select_parent()
{
    //int size = pop_.size();
    double total_fitness = std::accumulate(pop_.begin(),pop_.end(),0,[](double sum, Chromosome* a){return sum + a->get_fitness();});

    std::uniform_real_distribution<double> distribution(0, total_fitness); // random double num generator from [0,total_fit]

    double random = distribution(generator_); // a random number between [0,total_fit]
    double psum = 0; // partial sum

    // add chromosome's fit to psum until psum exceeds random, then return that chromosome
    return *std::find_if(pop_.begin(), pop_.end(), [&psum,random](Chromosome* a){psum += a->get_fitness(); return psum>random;});
}
