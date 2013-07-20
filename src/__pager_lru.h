#ifndef __PAGER_LRU_H__
#define __PAGER_LRU_H__

#include <list>
#include <algorithm>
#include <functional>

#include "__pager_with_frame_table.h"
#include "mmu.h"

namespace mms
{

  //=================================//
  struct pager_lru                   //
  //---------------------------------//
    : public pager_with_frame_table  //
  //=================================//
  {
    pager_lru(char32_t num_frames, page_table *);

    virtual indx_t next_to_evict();

    void access_signal(indx_t page, mmu::access_instruction);
    std::function<void(indx_t page, mmu::access_instruction)> get_access_signal_object();

  private:
    std::list<indx_t> frame_use_queue;

    struct lru_alert_at_signal
    {
      pager_lru * pager;
      inline void operator()(indx_t page, mmu::access_instruction instr)
      {
        pager->access_signal(page, instr);
      }
    };

  }; //--pager_lru-------------------//

};

#endif //__PAGER_LRU_H__
