#ifndef __MMU_H__
#define __MMU_H__

#include "pte.h"

#include <cassert>
#include <cstddef>
#include <vector>

namespace mms
{

  struct page_table;
  struct mmu;
  struct page_fault_handler;
  struct pager;

  typedef uint32_t frame_t;

  typedef std::vector<pte_t> __page_table_base;



  //=========== PAGE_TABLE =========//
  struct page_table :               //
    private __page_table_base {     //
  //================================//
    page_table(size_t num_pages);

    pte_t get_page_bits(size_t page_indx) const;
    pte_t get_frame_index(size_t page_indx) const;

    void   set_page_bits(size_t page_indx, pte_t bits);
    void raise_page_bits(size_t page_indx, pte_t bits);
    void unset_page_bits(size_t page_indx, pte_t bits);
    void set_frame_index(size_t page_indx, size_t frame_indx);
  }; // end: page_table ------------//



  //============== MMU ==============//
  struct mmu : private page_table {  //
  //=================================//
    mmu(size_t num_pages);
    frame_t fetch_read(size_t page_indx);
    frame_t fetch_write(size_t page_indx);
  }; // end: mmu --------------------//



  //=========== FAULT_HANDLER ===========//
  struct fault_handler {                 //
  //=====================================//
    static frame_t page_fault(size_t page_indx);
    static pager *& get_pager();
    static page_table *& get_page_table();
    static size_t & get_num_frames();
  }; // end: fault_handler --------------//



  //============ PAGER ============//
  struct pager {                   //
  //===============================//
    virtual frame_t next_evict() = 0;
  }; // end: pager ----------------//

};

#endif //__MMU_H__
