#include "__pager_fifo.h"

//=============== pager_fifo ===========================//

mms::pager_fifo::pager_fifo(uint32_t num_frames, page_table * pt)
  : pager_with_frame_table(num_frames, pt)
{ }

mms::indx_t mms::pager_fifo::next_to_evict()
{
  return get_page(frames_list.front());
}

void mms::pager_fifo::after_free_page( indx_t page, indx_t frame )
{
  assert( frame == frames_list.front() );
  frames_list.pop_front();
  pager_with_frame_table::after_free_page(page, frame);
}

void mms::pager_fifo::after_load_page( indx_t page, indx_t frame )
{
  frames_list.push_back(frame);

  std::ostringstream ss;
  for (auto i = frames_list.begin(); i != frames_list.end(); ++i)
    ss << *i << ' ';
  OUT(DEBUG) << ss.str();

  pager_with_frame_table::after_load_page(page, frame);
}
