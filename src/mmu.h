#ifndef __MMU_H__
#define __MMU_H__

/**
  * This file contains interfaces of classes used in the
  * Memory Management System (mms) namespace.
  */

#include <cstdint>

#include "pte.h"

namespace mms
{

  typedef uint32_t frame_t;
  typedef uint32_t page_t;


  //==========//
  struct mmu  //
  //==========//
  {
    enum access_instruction { READ, WRITE };

    virtual frame_t access_page(access_instruction, page_t) = 0;
    virtual bool page_fault() = 0;
    virtual void clear_fault() = 0;
  }; // end: mmu


  //=====================//
  struct mmu_controller  //
  //=====================//
  {
    virtual void map_page(page_t, frame_t) = 0;
    virtual void unmap_page(page_t, frame_t) = 0;
  }; // end: mmu_controller


  //=================//
  struct page_table  //
  //=================//
  {
    virtual uint32_t get_page_properties(page_t page_indx) = 0;
    virtual uint32_t get_page_frame(page_t page_indx) = 0;
  }; // end: page_table


  //=========================//
  struct page_fault_handler  //
  //=========================//
  {
    virtual void page_fault(page_t page_indx) = 0;
  }; // end: page_fault_handler


  //============//
  struct pager  //
  //============//
  {

  }; // end: pager

};

#endif //__MMU_H__
