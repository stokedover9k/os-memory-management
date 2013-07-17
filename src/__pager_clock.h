#ifndef __PAGER_CLOCK_H__
#define __PAGER_CLOCK_H__

#include "__pager_with_free_list.h"
#include "__pager_with_frame_table.h"

namespace mms
{

  //===============================//
  template <typename ParentPager>  //
  struct pager_clock               //
  //-------------------------------//
    : public ParentPager           //
  //===============================//
  {
    pager_clock(char32_t num_frames, page_table *pt, indx_t index_count)
      : ParentPager(num_frames, pt), indx_count(index_count), hand(0)
    { }

  protected:
    virtual indx_t next_to_evict()
    {
      while(true)
      {
        // Hand points to either pages or frames, but get_next_page()
        // must return a page index (possibly using the frame table)
        // so that page properties can be retrieved.
        indx_t next_page = get_next_page();
        hand = (++hand < indx_count) ? hand : 0;  // advance hand
        indx_t bits = ParentPager::page_table_->get_page_properties(next_page);
        if( bits & PRESENT && !(bits & REFERENCED) )
          return next_page;
        ParentPager::page_table_->unset_properties(next_page, REFERENCED);
      }
    }

    virtual indx_t get_next_page() = 0;
    indx_t get_hand() const { return hand; }

  private:
    indx_t indx_count;
    indx_t hand;
  }; //--pager_clock---------------//



  //==============================================//
  struct pager_clock_p                            //
  //----------------------------------------------//
    : public pager_clock<pager_with_frame_table>  //
  //==============================================//
  {
    pager_clock_p(char32_t num_frames, page_table *pt);

  protected:
    indx_t get_next_page();
  }; //--pager_clock_p----------------------------//



  //============================================//
  struct pager_clock_v                          //
  //--------------------------------------------//
    : public pager_clock<pager_with_free_list>  //
  //============================================//
  {
    pager_clock_v(char32_t num_frames, page_table *pt, char32_t num_pages);

  protected:
    indx_t get_next_page();
    void after_free_page( indx_t page, indx_t frame );
    void after_load_page( indx_t page, indx_t frame );
  }; //--pager_clock_v--------------------------//

};

#endif //__PAGER_CLOCK_H__
