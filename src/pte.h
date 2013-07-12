#ifndef __PTE_H__
#define __PTE_H__ 

#include <cstdint>

namespace mms
{
  typedef uint32_t pte_t;

  enum pte_bits {
    PRESENT    = 1 << 31,
    MODIFIED   = 1 << 30,
    REFERENCED = 1 << 29,
    PAGEDOUT   = 1 << 28 };

  pte_t const BITS_MASK = PRESENT|MODIFIED|REFERENCED|PAGEDOUT;
  pte_t const INDEX_MASK = ~BITS_MASK;

  inline void  raise_bits( pte_t *pte, pte_t pbits ) { *pte |=  pbits; }
  inline void  unset_bits( pte_t *pte, pte_t pbits ) { *pte &= ~pbits; }
  inline void toggle_bits( pte_t *pte, pte_t pbits ) { *pte ^=  pbits; }
  inline bool  check_bits( pte_t  pte, pte_t pbits ) { return pte & pbits; }
};

#endif //__PTE_H__
