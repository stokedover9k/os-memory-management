#include "__pager_lru.h"

mms::pager_lru::pager_lru(char32_t num_frames, page_table * pt)
  : pager_with_frame_table(num_frames, pt)
{ }

void mms::pager_lru::access_signal(indx_t page, mmu::access_instruction)
{
  indx_t frame = page_table_->get_page_frame(page);
  auto itr = std::find(frame_use_queue.begin(), frame_use_queue.end(), frame);
  if( itr != frame_use_queue.end() )
    frame_use_queue.erase(itr);
  frame_use_queue.push_front(frame);
}

mms::indx_t mms::pager_lru::next_to_evict()
{
  return get_page(frame_use_queue.back());
}

std::function<void(mms::indx_t page, mms::mmu::access_instruction)> 
mms::pager_lru::get_access_signal_object()
{
  lru_alert_at_signal s;
  s.pager = this;
  return std::function<void(indx_t page, mmu::access_instruction)>(s);
}
