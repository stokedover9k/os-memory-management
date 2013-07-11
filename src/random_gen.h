#ifndef __RGEN_H__
#define __RGEN_H__

/**********************************************************************
  * CONTENTS:
  * - random_generator: a policy-based random number generator template
  * - looping_random_iterator: a random_generatory policy template
  * - typedefs and macros:
  *    - random_vector_looping_iterator
  *    - VRAND macro: constructs random_generator with vector looping policy
  *********************************************************************/

namespace rnd
{
  //================== random_generator =====================//
  template <typename RandomNumberIteratorPolicy>             //
  struct random_generator {                                  //
  //=========================================================//
    inline int next(int cap) { 
      return RandomNumberIteratorPolicy::next() % cap; }
  }; // end: random_generator -------------------------------//

  //============== looping_random_iterator ==================//
  template <typename Container>                              //
  struct looping_random_iterator {                           //
  //=========================================================//
    typedef Container container_type;

    inline static typename Container::value_type next() {
      if( get_iterator() == get_container().end() )
        get_iterator() = get_container().begin();
      return *get_iterator()++; }

    inline static void set_container(Container c) {
      get_container().swap(c);
      get_iterator() = get_container().begin(); }

  private:
    inline static Container & get_container() {
      static Container _container;
      return _container; }

    inline static typename Container::const_iterator & get_iterator() {
      static typename Container::const_iterator _itr = get_container().begin();
      return _itr; }
  }; // end: looping_random_iterator ------------------------//

};

#ifndef SKIP_RANDOM_GENERATOR_DEFS
  
#include <vector>
namespace rnd
{
  typedef looping_random_iterator<std::vector<int> > random_vector_looping_iterator;
};
#define VRAND rnd::random_generator<typename rnd::random_vector_looping_iterator>()

#endif //SKIP_RANDOM_GENERATOR_DEFS

#endif //__RGEN_H__