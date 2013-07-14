#ifndef __FAULT_HANDLER_H__
#define __FAULT_HANDLER_H__

#include <stdexcept>
#include "mms.h"

namespace mms
{

  //===============================//
  struct fault_handler_with_pager  //
  //-------------------------------//
    : public page_fault_handler    //
  //===============================//
  {
    fault_handler_with_pager( mmu *, pager * );

    void page_fault(indx_t page_indx);

  private:
    mmu * mmu_;
    pager * pager_;
  }; //--fault_handler_with_pager--//

};

#endif //__FAULT_HANDLER_H__
