#ifndef __PAGER_WITH_FRAME_TABLE_H__
#define __PAGER_WITH_FRAME_TABLE_H__

#include <unordered_map>

#include "__pager_with_free_list.h"

namespace mms
{
  //===============================//
  struct pager_with_frame_table    //
  //-------------------------------//
    : public pager_with_free_list  //
  //===============================//
  {
    pager_with_frame_table(uint32_t num_frames, page_table *);

    virtual void after_free_page( indx_t page, indx_t frame );
    virtual void after_load_page( indx_t page, indx_t frame );

  protected:
    indx_t get_page(indx_t frame);

  private:
    std::unordered_map<indx_t, indx_t> frame_table;
  }; //--pager_with_frame_table----//
};

#endif //__PAGER_WITH_FRAME_TABLE_H__