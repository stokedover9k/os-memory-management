#ifndef __PAGER_NRU_H__
#define __PAGER_NRU_H__

#include <vector>
#include <cassert>
#include <stdexcept>

#include "__pager_with_free_list.h"

namespace mms
{

  //===============================//
  struct pager_nru                 //
  //-------------------------------//
    : public pager_with_free_list  //
  //===============================//
  {
    pager_nru(char32_t num_frames, page_table * pt, char32_t pages);

  protected:
    static unsigned int bits_to_class(char32_t bits);

    virtual indx_t next_to_evict();

    virtual void after_free_page( indx_t page, indx_t frame );
    virtual void after_load_page( indx_t page, indx_t frame );

  private:
    char32_t num_pages;
    char fault_count;
    enum { REFERENCED_REFRESH_RATE = 10 };
  }; //--pager_nru-----------------//

};

#endif //__PAGER_NRU_H__
