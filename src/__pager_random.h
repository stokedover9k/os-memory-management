#ifndef __PAGER_RANDOM_H__
#define __PAGER_RANDOM_H__

#include <set>

#include "__pager_with_frame_table.h"

namespace mms
{
  //=================================//
  struct pager_random                //
  //---------------------------------//
    : public pager_with_frame_table  //
  //=================================//
  {
    pager_random(char32_t num_frames, page_table *);

  protected:
    virtual indx_t next_to_evict();
    virtual void after_free_page( indx_t page, indx_t frame );
    virtual void after_load_page( indx_t page, indx_t frame );

  private:
    std::set<indx_t> used_frames;
  }; //--pager_random----------------//
};

#endif //__PAGER_RANDOM_H__