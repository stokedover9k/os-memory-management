#ifndef __PTE_H__
#define __PTE_H__ 

#include <cstdint>
#include <string>

namespace mms
{
  typedef uint32_t pte_t;

  enum pte_bits {
    PRESENT    = 1 << 31,
    MODIFIED   = 1 << 30,
    REFERENCED = 1 << 29,
    PAGEDOUT   = 1 << 28 };

  const pte_t PROPERTIES_MASK = PRESENT|MODIFIED|REFERENCED|PAGEDOUT;
  const pte_t INDEX_MASK = ~(PROPERTIES_MASK);

  inline void  raise_bits( pte_t *pte, pte_t pbits ) { *pte |=  pbits; }
  inline void  unset_bits( pte_t *pte, pte_t pbits ) { *pte &= ~pbits; }
  inline void toggle_bits( pte_t *pte, pte_t pbits ) { *pte ^=  pbits; }
  inline bool  check_bits( pte_t  pte, pte_t pbits ) { return pte & pbits; }

  inline std::string bits_to_string(pte_t bits) {
    if( !(bits & (PRESENT|PAGEDOUT)) ) return "*";
    if( !(bits & PRESENT) ) return "#";
    std::string s = "RMS";
    if( !(bits & REFERENCED) ) s[0] = '-';
    if( !(bits & MODIFIED) )   s[1] = '-';
    if( !(bits & PAGEDOUT) )   s[2] = '-';
    return s;
  }
};

#endif //__PTE_H__
