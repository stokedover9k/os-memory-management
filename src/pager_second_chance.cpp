#include "__pager_second_chance.h"

//=============== pager_second_chance =====================//

mms::pager_second_chance::pager_second_chance(uint32_t num_frames, page_table *pt)
  : pager_fifo(num_frames, pt)
{ }

mms::indx_t mms::pager_second_chance::next_to_evict()
{
  while(true)
  {
    indx_t frame = frames_list.front();
    indx_t page = get_page(frame);
    uint32_t bits = page_table_->get_page_properties(page);
    if( !(bits & REFERENCED) )
    {
      return page;
    }
    page_table_->unset_properties(page, REFERENCED);
    frames_list.pop_front();
    frames_list.push_back(frame);
  }
}
