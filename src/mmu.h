#ifndef __MMU_H__
#define __MMU_H__

#include <vector>
#include <cassert>
#include <functional>

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
    typedef std::function<void(indx_t, access_instruction)> access_signal_type;
  private:
    struct do_nothing_at_signal
    {
      inline void operator()(indx_t page, access_instruction i) { ; }
    };

  public:

    mmu_with_vector_page_table(
      unsigned int num_pages, 
      access_signal_type
        = do_nothing_at_signal());

    // mmu interface
    indx_t access_page(access_instruction, indx_t);
    bool page_fault();
    void clear_fault();
    void set_access_signal( access_signal_type s );

  private:
    bool faulted;
    access_signal_type access_signal;
  }; //--mmu_with_vector_page_table--//
};

#endif //__MMU_H__
