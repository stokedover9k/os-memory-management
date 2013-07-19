#include "__pager_nru.h"

mms::pager_nru::pager_nru(char32_t num_frames, mms::page_table * pt, char32_t pages)
  : pager_with_free_list(num_frames, pt), num_pages(pages), fault_count(0)
{ }

unsigned int mms::pager_nru::bits_to_class(char32_t bits)
{
  if( bits == 0 )               return 0;
  if( bits == MODIFIED )        return 1;
  if( bits == REFERENCED )      return 2;
  assert( bits == (MODIFIED|REFERENCED) );
  return 3;
}

mms::indx_t mms::pager_nru::next_to_evict()
{
  fault_count++;

  std::vector<indx_t> page_classes[4];

  for(unsigned int page = 0; page < num_pages; ++page)
  {
    char32_t bits = page_table_->get_page_properties(page);
    if( !(bits & PRESENT) )  continue;
    unsigned int page_class = bits_to_class(bits & (MODIFIED|REFERENCED));
    page_classes[page_class].push_back(page);

    if( fault_count == REFERENCED_REFRESH_RATE )
      page_table_->unset_properties(page, REFERENCED);
  }

  if( fault_count == REFERENCED_REFRESH_RATE )
    fault_count = 0;

  for (int i = 0; i < 4; ++i)
  {
    if (page_classes[i].size() > 0)
      return page_classes[i][VRAND(page_classes[i].size())];
  }
  throw std::logic_error("NRU pager did not classify any pages");
}

void mms::pager_nru::after_free_page( indx_t page, indx_t frame )
{ }

void mms::pager_nru::after_load_page( indx_t page, indx_t frame )
{ }

