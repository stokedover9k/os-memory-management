#include "__pager_aging.h"

//=============== pager_aging_v ========================//

mms::pager_aging_v::pager_aging_v(char32_t num_frames, mms::page_table *pt, char32_t num_pages)
  : pager_aging<pager_with_free_list>(num_frames, pt, num_pages)
{ }

mms::indx_t mms::pager_aging_v::index_to_page(indx_t index)
{
  return index;
}

mms::indx_t mms::pager_aging_v::page_to_index(indx_t page)
{
  return page;
}

//=============== pager_aging_p ========================//

mms::pager_aging_p::pager_aging_p(char32_t num_frames, mms::page_table *pt)
  : pager_aging<pager_with_frame_table>(num_frames, pt, num_frames)
{ }

mms::indx_t mms::pager_aging_p::index_to_page(indx_t index)
{
  return get_page(index);
}

mms::indx_t mms::pager_aging_p::page_to_index(indx_t page)
{
  return page_table_->get_page_frame(page);
}
