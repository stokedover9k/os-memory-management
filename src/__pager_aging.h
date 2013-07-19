#ifndef __PAGER_AGING_H__
#define __PAGER_AGING_H__

#include <vector>

#include "__pager_with_frame_table.h"

namespace mms
{

  template <typename ParentPager>
  struct pager_aging
    : public ParentPager
  {
    pager_aging(char32_t num_frames, page_table *pt, indx_t index_count)
      : ParentPager(num_frames, pt), age_vector(index_count, 0)
    { }

  protected:
    virtual indx_t next_to_evict()
    {
      char32_t min_age = ~0;
      int min_page = 0;

      for(unsigned int index = 0; index < age_vector.size(); ++index)
      {
        // index can refer to either physical frame or virtual page;
        // index_to_page(...) converts that index to page value.
        indx_t page = index_to_page(index);
        char32_t bits = ParentPager::page_table_->get_page_properties(page);

        if( bits & PRESENT )
        {
          // shift counter
          age_vector[index] >>= 1;
  
          // if present and referenced, set leftmost bit to 1
          if( bits & REFERENCED )
          {
            age_vector[index] |= (1 << 31);
            ParentPager::page_table_->unset_properties(page, REFERENCED);
          }

          if( age_vector[index] < min_age )
          {
            min_age = age_vector[index];
            min_page = page;
          }
        }
      }

      std::ostringstream os;
      for(unsigned int i = 0; i < age_vector.size(); ++i )
      {
        os << ' ' << i << ':' << std::hex << age_vector[i] << std::dec;
      }
      OUT(INSTR_FRAMES) << "||" << os.str();

      return min_page;
    }

    void after_load_page( indx_t page, indx_t frame )
    {
      ParentPager::after_load_page(page, frame);
      age_vector[page_to_index(page)] = 0;
    }

    virtual indx_t index_to_page(indx_t index) = 0;
    virtual indx_t page_to_index(indx_t page) = 0;

  private:
    std::vector<char32_t> age_vector;
  };



  //============================================//
  struct pager_aging_v                          //
  //--------------------------------------------//
    : public pager_aging<pager_with_free_list>  //
  //============================================//
  {
    pager_aging_v(char32_t num_frames, page_table *pt, char32_t num_pages)
      : pager_aging<pager_with_free_list>(num_frames, pt, num_pages)
    { }

  protected:
    indx_t index_to_page(indx_t index)
    {
      return index;
    }

    indx_t page_to_index(indx_t page)
    {
      return page;
    }

  }; //--pager_aging_v--------------------------//



  //==============================================//
  struct pager_aging_p                            //
  //----------------------------------------------//
    : public pager_aging<pager_with_frame_table>  //
  //==============================================//
  {
    pager_aging_p(char32_t num_frames, page_table *pt)
      : pager_aging<pager_with_frame_table>(num_frames, pt, num_frames)
    { }

  protected:

    indx_t index_to_page(indx_t index)
    {
      return get_page(index);
    }

    indx_t page_to_index(indx_t page)
    {
      return page_table_->get_page_frame(page);
    }

  }; //--pager_aging_v----------------------------//

};

#endif //__PAGER_AGING_H__
