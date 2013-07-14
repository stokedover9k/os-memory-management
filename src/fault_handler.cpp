#include "fault_handler.h"

mms::fault_handler_with_pager::fault_handler_with_pager(mmu * m, pager * p)
  : mmu_(m), pager_(p)
{
  if( m == NULL )
    throw std::invalid_argument("MMU passed to fault_handler_with_pager is NULL");
  if( p == NULL )
    throw std::invalid_argument("Pager passed to fault_handler_with_pager is NULL");
}

void mms::fault_handler_with_pager::page_fault(indx_t page_indx)
{
  //indx_t frame = pager_->get_next_frame(page_indx);
  pager_->get_next_frame(page_indx);
  mmu_->clear_fault();
}
