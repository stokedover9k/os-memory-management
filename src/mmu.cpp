#include "mmu.h"

//==================== mms::page_table ======================//

mms::page_table::page_table(size_t num_pages) :
  std::vector<mms::pte_t>(num_pages, 0)
{ }

mms::pte_t mms::page_table::get_page_bits(size_t page_indx) const {
  return at(page_indx) & BITS_MASK; }

mms::pte_t mms::page_table::get_frame_index(size_t page_indx) const {
  return at(page_indx) & INDEX_MASK; }

void mms::page_table::set_page_bits(size_t page_indx, pte_t bits) {
  assert(bits == (bits & BITS_MASK));
  at(page_indx) = get_frame_index(page_indx) | bits; }

void mms::page_table::raise_page_bits(size_t page_indx, pte_t bits) {
  assert(bits == (bits & BITS_MASK));
  raise_bits( &at(page_indx), bits); }

void mms::page_table::unset_page_bits(size_t page_indx, pte_t bits) {
  assert(bits == (bits & BITS_MASK));
  unset_bits( &at(page_indx), bits & BITS_MASK ); }

void mms::page_table::set_frame_index(size_t page_indx, size_t frame_indx) {
  assert(frame_indx == (frame_indx & INDEX_MASK));
  at(page_indx) = get_page_bits(page_indx) | frame_indx; }

//==================== mms::mmu ==============================//

mms::mmu::mmu(size_t num_pages) : 
  page_table(num_pages)
{ }

/*
mms::frame_t mms::mmu::fetch_read(size_t page_indx) {
  return get_page_bits(page_indx) & PRESENT
    ? get_frame_index(page_indx)
    : page_fault(page_indx); }  // TODO

mms::frame_t mms::mmu::fetch_write(size_t page_indx) { 
  // TODO
}
*/

//==================== mms::fault_handler ====================//

mms::frame_t mms::fault_handler::page_fault(size_t page_indx) {
  static frame_t next_free_frame = 0;
  if( next_free_frame < get_num_frames() )
    return next_free_frame++;

  frame_t evict_page = get_pager()->next_evict(); 
  // TODO

  return get_page_table()->get_frame_index(evict_page);
}

mms::pager *& mms::fault_handler::get_pager() {
  static pager * _pager = NULL;
  return _pager; }

mms::page_table *& mms::fault_handler::get_page_table() {
  static page_table * _page_table = NULL;
  return _page_table; }

size_t & mms::fault_handler::get_num_frames() {
  static size_t num_frames = 1;
  return num_frames; }
