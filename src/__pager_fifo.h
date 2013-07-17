#ifndef __PAGER_FIFO_H__
#define __PAGER_FIFO_H__

#include "__pager_with_frame_table.h"

namespace mms
{
  //=================================//
  struct pager_fifo                  //
  //---------------------------------//
    : public pager_with_frame_table  //
  //=================================//
  {
    pager_fifo(uint32_t num_frames, page_table *);

  protected:
    virtual indx_t next_to_evict();
    virtual void after_free_page( indx_t page, indx_t frame );
    virtual void after_load_page( indx_t page, indx_t frame );

    std::list<indx_t> frames_list;
  }; //--pager_fifo------------------//
};

#endif //__PAGER_FIFO_H__