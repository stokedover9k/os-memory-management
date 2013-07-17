#include "__pager_random.h"

//=============== pager_random ========================//

mms::pager_random::pager_random(uint32_t num_frames, page_table * pt)
  : pager_with_frame_table(num_frames, pt)
{ }

mms::indx_t mms::pager_random::next_to_evict()
{
  auto itr = used_frames.begin();
  for( int i = VRAND(used_frames.size()); i > 0; --i )
  {
    itr++;
  }
  return get_page(*itr);
}

void mms::pager_random::after_free_page( indx_t page, indx_t frame )
{
  used_frames.erase(frame);
  pager_with_frame_table::after_free_page(page, frame);
}

void mms::pager_random::after_load_page( indx_t page, indx_t frame )
{
  used_frames.insert(frame);
  pager_with_frame_table::after_load_page(page, frame);
}
