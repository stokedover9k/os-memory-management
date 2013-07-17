#include "mmu.h"

//================ vector_page_table ==================//

mms::vector_page_table::vector_page_table(unsigned int size) 
  : std::vector<indx_t>(size, 0)   // 0 = ~(PRESENT|MODIFIED|REFERENCED|PAGEDOUT) & PROPERTIES_MASK
{ }

void mms::vector_page_table::raise_properties(indx_t page_indx, pte_t bits)
{
  assert(bits == (bits & PROPERTIES_MASK));
  raise_bits( &at(page_indx), bits );
}

void mms::vector_page_table::unset_properties(indx_t page_indx, pte_t bits)
{
  assert(bits == (bits & PROPERTIES_MASK));
  unset_bits( &at(page_indx), bits );
}

char32_t mms::vector_page_table::get_page_properties(indx_t page_indx) const
{
  return at(page_indx) & PROPERTIES_MASK;
}

char32_t mms::vector_page_table::get_page_frame(indx_t page_indx) const
{
  return at(page_indx) & INDEX_MASK;
}

void mms::vector_page_table::set_page_frame(indx_t page_indx, indx_t frame_indx)
{
  assert(frame_indx == (frame_indx & INDEX_MASK));
  unset_bits(&at(page_indx), INDEX_MASK);
  raise_bits(&at(page_indx), frame_indx);
}

//============== mmu_with_vector_page_table ==============//

mms::mmu_with_vector_page_table::mmu_with_vector_page_table(unsigned int size)
  : vector_page_table(size), faulted(false)
{ }

mms::indx_t mms::mmu_with_vector_page_table::access_page(access_instruction instr, indx_t page)
{
  char32_t frame = get_page_frame(page);
  char32_t bits = get_page_properties(page);

  if( ! (bits & PRESENT) )
  {
    faulted = true;
  }
  else
  {
    if( instr == mmu::WRITE )
    {
      raise_properties(page, MODIFIED);
    }

    raise_properties(page, REFERENCED);
  }

  return frame;
}

bool mms::mmu_with_vector_page_table::page_fault()
{
  return faulted;
}

void mms::mmu_with_vector_page_table::clear_fault()
{
  faulted = false;
}
