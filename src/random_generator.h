#ifndef __RANDOM_GENERAGOR_H__
#define __RANDOM_GENERAGOR_H__

#include <vector>
#include <iostream>

//======= RANDOM_GENERATOR ========//
struct random_generator {          //
//=================================//
  virtual int get_random(int cap) = 0;
}; //end: random_generator --------//



//===== LOOPING_RANDOM_GENERATOR ======//
struct looping_random_generator        //
  : public random_generator {          //
//=====================================//
  template<typename InputIterator>
  looping_random_generator(InputIterator begin, InputIterator end);

  virtual int get_random(int cap);

private:
  std::vector<int> nums;
  std::vector<int>::const_iterator current;
}; //end: loopgin_random_generator ----//



//============ TEMPLATE IMPLEMENTATIONS ===============//

template<typename InputIterator>
looping_random_generator::looping_random_generator(InputIterator begin, InputIterator end) 
{
  for( ; begin != end; begin++ )
    nums.push_back(*begin); 
  current = nums.begin();
}



#endif //__RANDOM_GENERAGOR_H__
