#include "pagers.h"

//================= pager_with_free_list ==================//

mms::pager_with_free_list::pager_with_free_list(uint32_t num_frames, mms::page_table * pt)
  : page_table_(pt)
{
  for( unsigned int i = 0; i < num_frames; ++i )
    free_list.push_back(i);
}

mms::indx_t mms::pager_with_free_list::get_next_frame(mms::indx_t page) {
  if( free_list.empty() )
  {
    indx_t evict_page = next_to_evict();
    OUT(INFO) << "evict " << evict_page << " (" << bits_to_string(page_table_->get_page_properties(evict_page)) << ')';
    uint32_t frame = page_table_->get_page_frame(evict_page);

    free_page( evict_page, frame );
    after_free_page( evict_page, frame );

    free_list.push_back(frame);
  }
  
  indx_t frame = free_list.front();
  free_list.pop_front();

  // load page
  load_page(page, frame);
  after_load_page(page, frame);

  return frame;
}

void mms::pager_with_free_list::free_page( mms::indx_t page, mms::indx_t frame )
{
  OUT(OPERATIONS) << "UNMAP \t" << std::setw(4) << page << std::setw(4) << frame;
  page_table_->unset_properties( page, PRESENT );
  uint32_t bits = page_table_->get_page_properties( page );

  if( bits & MODIFIED )
  {
    pageout( page, frame ); 
  }
}

void mms::pager_with_free_list::load_page( mms::indx_t page, mms::indx_t frame )
{
  uint32_t bits = page_table_->get_page_properties( page );

  if( bits & PAGEDOUT )
  {
    pagein( page, frame ); 
  }
  else
  {
    zero( page, frame );
  }

  page_table_->set_page_frame( page, frame );
  OUT(OPERATIONS) << "MAP \t" << std::setw(4) << page << std::setw(4) << frame;
  page_table_->raise_properties( page, PRESENT );
}

void mms::pager_with_free_list::pageout( mms::indx_t page, mms::indx_t frame )
{
  OUT(OPERATIONS) << "OUT \t" << std::setw(4) << page << std::setw(4) << frame;
  page_table_->unset_properties( page, MODIFIED );
  page_table_->raise_properties( page, PAGEDOUT );
}

void mms::pager_with_free_list::pagein( mms::indx_t page, mms::indx_t frame )
{
  OUT(OPERATIONS) << "IN \t" << std::setw(4) << page << std::setw(4) << frame;
  page_table_->unset_properties( page, MODIFIED );
}

void mms::pager_with_free_list::zero( mms::indx_t page, mms::indx_t frame )
{
  OUT(OPERATIONS) << "ZERO \t" << std::setw(4) << frame;
  page_table_->unset_properties( page, MODIFIED );
}

//=============== pager_with_frame_table ==============//

mms::pager_with_frame_table::pager_with_frame_table(uint32_t num_frames, page_table * pt)
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
  return *itr;
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

