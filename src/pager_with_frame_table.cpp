#include "__pager_with_frame_table.h"

//=============== pager_with_frame_table ==============//

mms::pager_with_frame_table::pager_with_frame_table(char32_t num_frames, page_table * pt)
  : pager_with_free_list(num_frames, pt)
{ }

void mms::pager_with_frame_table::after_free_page( indx_t page, indx_t frame )
{
  frame_table.erase(frame);
}

void mms::pager_with_frame_table::after_load_page( indx_t page, indx_t frame )
{
  frame_table[frame] = page;
}

mms::indx_t mms::pager_with_frame_table::get_page(indx_t frame)
{
  return frame_table[frame];
}
