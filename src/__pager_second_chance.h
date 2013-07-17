#ifndef __PAGER_SECOND_CHANCE_H__
#define __PAGER_SECOND_CHANCE_H__

#include "__pager_fifo.h"

namespace mms
{

  //=================================//
  struct pager_second_chance         //
  //---------------------------------//
    : public pager_fifo              //
  //=================================//
  {
    pager_second_chance(uint32_t num_frames, page_table *);

  protected:
    virtual indx_t next_to_evict();
  }; //--pager_second_chance---------//

};

#endif //__PAGER_SECOND_CHANCE_H__