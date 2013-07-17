#ifndef __MMU_H__
#define __MMU_H__

#include <vector>
#include <cassert>

#include "mms.h"

namespace mms
{

  //==============================//
  struct vector_page_table        //
  //------------------------------//
    : public page_table,          //
      private std::vector<indx_t> //
  //==============================//
  {
    vector_page_table(unsigned int num_pages);

    void raise_properties(indx_t page_indx, pte_t property_bits);
    void unset_properties(indx_t page_indx, pte_t property_bits);
    char32_t get_page_properties(indx_t page_indx) const;
    char32_t get_page_frame(indx_t page_indx) const;
    void     set_page_frame(indx_t page_indx, indx_t frame_indx);

  }; //--vector_page_table--------//



  //=================================//
  struct mmu_with_vector_page_table  //
  //---------------------------------//
    : public mmu,                    //
      public vector_page_table       //
  //=================================//
  {
    mmu_with_vector_page_table(unsigned int num_pages);

    // mmu interface
    indx_t access_page(access_instruction, indx_t);
    bool page_fault();
    void clear_fault();

  private:
    bool faulted;
  }; //--mmu_with_vector_page_table--//
};

#endif //__MMU_H__
