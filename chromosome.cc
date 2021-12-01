/*
 * Implementation for Chromosome class
 */

#include <algorithm>
#include <cassert>
#include <random>

#include "chromosome.hh"

//////////////////////////////////////////////////////////////////////////////
// Generate a completely random permutation from a list of cities
Chromosome::Chromosome(const Cities* cities_ptr)
  : cities_ptr_(cities_ptr),
    order_(random_permutation(cities_ptr->size())),
    generator_(std::chrono::system_clock::now().time_since_epoch().count()),
    distribution_(0, order_.size()-1)  // range 0 to length of permutation
{
    assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Clean up as necessary
Chromosome::~Chromosome()
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Perform a single mutation on this chromosome
void
Chromosome::mutate()
{
    auto index1 = distribution_(generator_);
    auto index2 = distribution_(generator_);
    std::swap(order_[index1], order_[index2]);

    assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Return a pair of offsprings by recombining with another chromosome
// Note: this method allocates memory for the new offsprings
std::pair<Chromosome*, Chromosome*>
Chromosome::recombine(const Chromosome* other)
{
  assert(is_valid());
  assert(other->is_valid());
  
  unsigned num1 = distribution_(generator_); // random int between 0 and permutation length
  unsigned num2 = distribution_(generator_);
  unsigned b = std::min(num1, num2);
  unsigned e = std::max(num1, num2);
  assert (b < order_.size());
  assert (0 <= b);

  auto child1 = create_crossover_child(this, other, b, e);
  auto child2 = create_crossover_child(other, this, b, e);
  std::pair<Chromosome*, Chromosome*> pair (child1, child2);
  return pair;
}

//////////////////////////////////////////////////////////////////////////////
// For an ordered set of parents, return a child using the ordered crossover.
// The child will have the same values as p1 in the range [b,e),
// and all the other values in the same order as in p2.
Chromosome*
Chromosome::create_crossover_child(const Chromosome* p1, const Chromosome* p2,
                                   unsigned b, unsigned e) const
{
  Chromosome* child = p1->clone();

  // We iterate over both parents separately, copying from parent1 if the
  // value is within [b,e) and from parent2 otherwise
  unsigned i = 0, j = 0;

  for ( ; i < p1->order_.size() && j < p2->order_.size(); ++i) {
    if (i >= b and i < e) {
      child->order_[i] = p1->order_[i];
    }
    else { // Increment j as long as its value is in the [b,e) range of p1
      while (p1->is_in_range(p2->order_[j], b, e)) {
        ++j;
        assert(j < p2->order_.size());
      }
      child->order_[i] = p2->order_[j]; // if p2 jth value is not in [b,e) range of p1, copy into child
      j++;
    }
  }

  assert(child->is_valid());
  return child;
}

// Return a positive fitness value, with higher numbers representing
// fitter solutions (shorter total-city traversal path).
double
Chromosome::get_fitness() const
{
    auto distance = calculate_total_distance();
    return (1/distance);
}

// A chromsome is valid if it has no repeated values in its permutation,
// as well as no indices above the range (length) of the chromosome.
// We implement this check with a sort, which is a bit inefficient, but simple
bool
Chromosome::is_valid() const
{
    auto range = order_.size();
    auto permute = order_;

    auto it1 = std::find_if(permute.begin(),permute.end(),[range](auto x){return x > range;}); // returns last if no elt exceeds range
    bool in_range = (it1 == permute.end());

    std::sort(permute.begin(),permute.end());
    std::unique(permute.begin(),permute.end()); // returns last if no duplicates are removed
    bool no_repeat = (range == permute.size());
    return (in_range and no_repeat);
}

// Find whether a certain value appears in a given range of the chromosome.
// Returns true if value is within the specified the range specified
// [begin, end) and false otherwise.
bool
Chromosome::is_in_range(unsigned value, unsigned begin, unsigned end) const
{
    auto it = std::find(order_.cbegin()+begin, order_.cbegin()+end,value); // return last if can't find value
    return (it!= order_.cbegin()+end);
}
