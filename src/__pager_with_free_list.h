#ifndef __PAGER_WITH_FREE_LIST_H__
#define __PAGER_WITH_FREE_LIST_H__

#include <sstream>
#include <iomanip>
#include <list>

#include "output.h"
#include "random_gen.h"
#include "mmu.h"
#include "pte.h"


namespace mms
{
  //===========================//
  struct pager_with_free_list  //
  //---------------------------//
    : public pager             //
  //===========================//
  {
    pager_with_free_list(char32_t num_frames, page_table *);

    indx_t get_next_frame(indx_t page_indx);

  protected:
    virtual indx_t next_to_evict() = 0;
    virtual void after_free_page( indx_t page, indx_t frame ) { };
    virtual void after_load_page( indx_t page, indx_t frame ) { };

    page_table * const page_table_;

  private:
    void free_page( indx_t page, indx_t frame );
    void load_page( indx_t page, indx_t frame );

    void pageout( indx_t page, indx_t frame );
    void pagein( indx_t page, indx_t frame );
    void zero( indx_t page, indx_t frame );

    std::list<indx_t> free_list;
  }; //--pager_with_free_list--//
};

#endif //__PAGER_WITH_FREE_LIST_H__