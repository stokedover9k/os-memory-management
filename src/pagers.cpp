#include "pagers.h"

void mms::pager_base::set_page_table( mms::page_table const * const pt ) {
  ptable = pt; }

//================== random_pager ====================//

mms::random_pager::random_pager(unsigned int _num_frames) 
  : num_frames(_num_frames)
{ }

mms::frame_t mms::random_pager::next_evict() {
  frame_t f = VRAND.next(num_frames);
  return f; }