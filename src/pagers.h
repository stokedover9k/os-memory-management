#ifndef __PAGERS_H__
#define __PAGERS_H__

#include <iomanip>
#include <list>
#include <set>
#include <map>
#include <unordered_map>

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
    pager_with_free_list(uint32_t num_frames, page_table *);

    indx_t get_next_frame(indx_t page_indx);

  protected:
    virtual indx_t next_to_evict() = 0;
    virtual void after_free_page( indx_t page, indx_t frame ) = 0;
    virtual void after_load_page( indx_t page, indx_t frame ) = 0;

  private:
    void free_page( indx_t page, indx_t frame );
    void load_page( indx_t page, indx_t frame );

    void pageout( indx_t page, indx_t frame );
    void pagein( indx_t page, indx_t frame );
    void zero( indx_t page, indx_t frame );

    page_table * const page_table_;
    std::list<indx_t> free_list;
  }; //--pager_with_free_list--//



  //===============================//
  struct pager_with_frame_table    //
  //-------------------------------//
    : public pager_with_free_list  //
  //===============================//
  {
    pager_with_frame_table(uint32_t num_frames, page_table *);

    virtual void after_free_page( indx_t page, indx_t frame );
    virtual void after_load_page( indx_t page, indx_t frame );

  private:
    std::unordered_map<indx_t, indx_t> frame_table;
  }; //--pager_with_frame_table----//



  //=================================//
  struct pager_random                //
  //---------------------------------//
    : public pager_with_frame_table  //
  //=================================//
  {
    pager_random(uint32_t num_frames, page_table *);

  protected:
    virtual indx_t next_to_evict();
    virtual void after_free_page( indx_t page, indx_t frame );
    virtual void after_load_page( indx_t page, indx_t frame );

  private:
    std::set<indx_t> used_frames;
  }; //--pager_random----------------//



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

  private:
    std::map<indx_t, indx_t> used_frames;
  }; //--pager_fifo------------------//

};

#endif //__PAGERS_H__
