#ifndef __MMS_H__
#define __MMS_H__

/**
  * This file contains interfaces of classes used in the
  * Memory Management System (mms) namespace.
  */

#include <cstdint>

#include "pte.h"

namespace mms
{
  typedef char32_t indx_t;

  //==========//
  struct mmu  //
  //==========//
  {
    enum access_instruction { READ, WRITE };

    virtual indx_t access_page(access_instruction, indx_t) = 0;
    virtual bool page_fault() = 0;
    virtual void clear_fault() = 0;
  }; // end: mmu

  //=================//
  struct page_table  //
  //=================//
  {
    virtual void raise_properties(indx_t page_indx, pte_t property_bits) = 0;
    virtual void unset_properties(indx_t page_indx, pte_t property_bits) = 0;
    virtual char32_t get_page_properties(indx_t page_indx) const = 0;
    virtual char32_t get_page_frame(indx_t page_indx) const = 0;
    virtual void     set_page_frame(indx_t page_indx, indx_t frame_indx) = 0;
  }; // end: page_table


  //=========================//
  struct page_fault_handler  //
  //=========================//
  {
    virtual void page_fault(indx_t page_indx) = 0;
  }; // end: page_fault_handler


  //============//
  struct pager  //
  //============//
  {
    virtual indx_t get_next_frame(indx_t page_indx) = 0;

    inline static char32_t & stat_map()   { static char32_t  __map__  = 0;  return  __map__;  }
    inline static char32_t & stat_unmap() { static char32_t __unmap__ = 0;  return __unmap__; }
    inline static char32_t & stat_out()   { static char32_t  __out__  = 0;  return  __out__;  }
    inline static char32_t & stat_in()    { static char32_t   __in__  = 0;  return   __in__;  }
    inline static char32_t & stat_zero()  { static char32_t  __zero__ = 0;  return  __zero__; }
  }; // end: pager

};

#endif //__MMS_H__
