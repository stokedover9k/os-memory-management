#include "__pager_clock.h"

//================== pager_clock_p =============================//

mms::pager_clock_p::pager_clock_p(char32_t num_frames, page_table *pt)
      : pager_clock<pager_with_frame_table>(num_frames, pt, num_frames)
{ }

mms::indx_t mms::pager_clock_p::get_next_page()
{
  return get_page(get_hand());
}

//================== pager_clock_v =============================//

mms::pager_clock_v::pager_clock_v(char32_t num_frames, page_table *pt, char32_t num_pages)
  : pager_clock<pager_with_free_list>(num_frames, pt, num_pages)
{ }

mms::indx_t mms::pager_clock_v::get_next_page()
{
  return get_hand();
}

void mms::pager_clock_v::after_free_page( indx_t page, indx_t frame )
{ }

void mms::pager_clock_v::after_load_page( indx_t page, indx_t frame )
{ }
