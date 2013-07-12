#ifndef __PAGERS_H__
#define __PAGERS_H__ 

#include <vector>
#include <set>

#include "mmu.h"
#include "random_gen.h"

namespace mms
{

  //============ PAGER_BASE =========//
  struct pager_base {                //
  //=================================//
    void set_page_table( page_table const * const );
    virtual frame_t next_evict() = 0;

  private:
    page_table const * ptable;
  }; // end: pager_base -------------//



  //============ RANDOM_PAGER ===============//
  struct random_pager : public pager_base  { //
  //=========================================//
    random_pager(unsigned int num_frames);

    virtual frame_t next_evict();

  private:
    unsigned int num_frames;
  }; // end: random_pager -------------------//
};

#endif //__PAGERS_H__
