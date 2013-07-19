#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "loglib.h"
#include "output.h"
#include "random_gen.h"

#include "mms.h"
#include "mmu.h"
#include "pagers.h"
#include "fault_handler.h"

#define NUM_PAGES (64)

#ifndef PRINT_INFO
#define PRINT_INFO (false)
#endif

#ifndef PRINT_DEBUG
#define PRINT_DEBUG (false)
#endif

char const * const USAGE_STR = "Usage: ./mmu [-a<algo:aArfscClN>] [-o<options:OPpFfS>] [–f<num_frames>] inputfile randomfile";

namespace PARAMS
{
  static char algo = 'l';
  static unsigned int num_frames = 32;
  static char const * inputfile = NULL;
  static char const * randomfile = NULL;
};

void parse_args(int argc, char const *argv[]);
std::string page_table_to_string(mms::page_table *);
std::string frame_table_to_string(mms::page_table *);
std::string summary();

//===================== MAIN ===========================//

int main(int argc, char const *argv[])
{
  using namespace std;

  OutputToFile::Stream() = stdout;
  OutFilePrinter::ReportingMode() = OPERATIONS|INSTRUCTION|FINAL_PAGES|FINAL_FRAMES|SUMMARY;
  if( PRINT_INFO )  OutFilePrinter::ReportingMode() |= INFO;
  if( PRINT_DEBUG ) OutFilePrinter::ReportingMode() |= DEBUG;

  /* Use like this:
  OUT(OPERATIONS) << "O";
  OUT(FINAL_PAGES) << "P";
  OUT(FINAL_FRAMES) << "F";
  OUT(SUMMARY) << "S";
  OUT(INFO) << "I";
  OUT(DEBUG) << "D";
  */

  //------------ parse arguments -------------//
  try { parse_args(argc, argv); }
  catch (std::invalid_argument const& ia) {
    cerr << "Error (INVALID ARGUMENT): " << ia.what() << "\n\t" << USAGE_STR << '\n';
    exit(1); }

  // open file for logging
  Output2FILE::Stream() = fopen( "trace.log", "w" );

  //----------- set up random generator ------//
  {
    ifstream inrandom(PARAMS::randomfile);
    if( !inrandom ) {
      cerr << "Error: could not open random file" << "\n\t" << USAGE_STR << '\n';
      exit(2);
    }
    int numbers;
    inrandom >> numbers;

    std::vector<int> rvec(numbers);
    rvec.assign(istream_iterator<int>(inrandom), istream_iterator<int>());
    rnd::looping_random_iterator<std::vector<int> >::set_container(std::move(rvec));
  }

  //----- build MMU, mem-table, fault-handler, and pager -----//
  mms::mmu *mmu;
  mms::page_table *pt;
  mms::page_fault_handler *fault_handler;
  mms::pager *pager;
  {
    mms::mmu_with_vector_page_table * mmu_pt = new mms::mmu_with_vector_page_table(NUM_PAGES);

    switch(PARAMS::algo)
    {
      case 'r':  pager = new mms::pager_random(PARAMS::num_frames, mmu_pt);              break;
      case 'f':  pager = new mms::pager_fifo(PARAMS::num_frames, mmu_pt);                break;
      case 's':  pager = new mms::pager_second_chance(PARAMS::num_frames, mmu_pt);       break;
      case 'c':  pager = new mms::pager_clock_p(PARAMS::num_frames, mmu_pt);             break;
      case 'C':  pager = new mms::pager_clock_v(PARAMS::num_frames, mmu_pt, NUM_PAGES);  break;
      case 'l':
      {
        mms::pager_lru * plru = new mms::pager_lru(PARAMS::num_frames, mmu_pt);
        mmu_pt->set_access_signal( plru->get_access_signal_object() );
        pager = plru;
      }
        break;
      case 'N':  pager = new mms::pager_nru(PARAMS::num_frames, mmu_pt, NUM_PAGES);      break;
      default:  cerr << "Error: invalid pager algorithm \"" << PARAMS::algo << '"' << endl;  exit(5);
    }

    fault_handler = new mms::fault_handler_with_pager( mmu_pt, pager );

    mmu = mmu_pt;
    pt = mmu_pt;
  }

  //------------ set up input stream ---------//
  {
    ifstream infile(PARAMS::inputfile);
    if( !infile ) {
      cerr << "Error: could not open input file" << "\n\t" << USAGE_STR << '\n';
      exit(2);
    }
    string line;
    while( std::getline(infile, line) ) {
      if( line[0] == '#')
        continue;
      stringstream linestream(line);
      int access_type, page_num;
      linestream >> access_type >> page_num;

      OUT(INSTRUCTION) << access_type << ' ' << page_num;

      mms::mmu::access_instruction instr = (access_type == 0) 
        ? mms::mmu::READ
        : mms::mmu::WRITE;
      mms::indx_t page = page_num;

      mmu->access_page(instr, page);

      if( mmu->page_fault() )
      {
        OUT(INFO) << "faulted";
        mmu->clear_fault();

        fault_handler->page_fault(page);
        mmu->clear_fault();
        mmu->access_page(instr, page);

        if( mmu->page_fault() )
        {
          // kill process?
          cerr << "Error: failed to bring in page " << page << '\n';
          exit(4);
        }
      }

      OUT(INSTR_PAGES) << page_table_to_string(pt);
      OUT(INSTR_FRAMES) << frame_table_to_string(pt);
    }
  }

  OUT(FINAL_PAGES) << page_table_to_string(pt);
  OUT(FINAL_FRAMES) << frame_table_to_string(pt);
  OUT(SUMMARY) << summary();
}

// end: MAIN -------------------------------------------//

void parse_args(int argc, char const *argv[]) {
  if( argc <= 2 )
    throw std::invalid_argument("must provide input and random files");
  PARAMS::inputfile  = argv[argc-2];
  PARAMS::randomfile = argv[argc-1];

  for (int i = 1; i < argc - 2; ++i)
  {
    std::string arg( argv[i] );

    OUT(DEBUG) << i << " = " << arg;

    if( arg.length() < 3 || arg[0] != '-' )
      throw std::invalid_argument(arg);

    switch( arg[1] ) {
      case 'a': 
        if( arg.length() != 3 ) throw std::invalid_argument(arg);
        PARAMS::algo = arg[2];
        break;
      case 'o':
        OutFilePrinter::ReportingMode() &= ~(OPERATIONS|INSTRUCTION|FINAL_PAGES|FINAL_FRAMES|SUMMARY);
        for (unsigned int j = 2; j < arg.length(); ++j)
          switch( arg[j] ) {
            case 'O':  OutFilePrinter::ReportingMode() |= OPERATIONS|INSTRUCTION;    break;
            case 'P':  OutFilePrinter::ReportingMode() |= FINAL_PAGES;   break;
            case 'p':  OutFilePrinter::ReportingMode() |= INSTR_PAGES;   break;
            case 'F':  OutFilePrinter::ReportingMode() |= FINAL_FRAMES;  break;
            case 'f':  OutFilePrinter::ReportingMode() |= INSTR_FRAMES;  break;
            case 'S':  OutFilePrinter::ReportingMode() |= SUMMARY;       break;
            default: throw std::invalid_argument(arg + " unknown option " + arg[j]);
          }
        break;
      case 'f':
        PARAMS::num_frames = atoi( arg.substr(2).c_str() );
        if( PARAMS::num_frames == 0 ) throw std::invalid_argument(arg);
        break;
      default: throw std::invalid_argument(arg);
    }
  }
}

std::string page_table_to_string(mms::page_table *pt)
{
  std::ostringstream os;
  for (int i = 0; i < NUM_PAGES; ++i)
  {
    std::string page = mms::bits_to_string(pt->get_page_properties(i));
    if( page.length() > 1 )
      os << i << ':' << page << ' ';
    else
      os << page << ' ';
  }
  return os.str();
}

std::string frame_table_to_string(mms::page_table *pt)
{
  std::vector<int> frames_to_pages(PARAMS::num_frames, -1);
  for (int i = 0; i < NUM_PAGES; ++i)
  {
    if( pt->get_page_properties(i) & mms::PRESENT )
    {
      mms::indx_t frame = pt->get_page_frame(i);
      frames_to_pages[frame] = i;
    }
  }
  std::ostringstream os;
  for (std::vector<int>::iterator i = frames_to_pages.begin(); i != frames_to_pages.end(); ++i)
  {
    if( *i == -1 )
      os << "* ";
    else
      os << *i << ' ';
  }
  return os.str();
}

std::string summary()
{
  std::ostringstream os;
  uint64_t total
    = 400  * (mms::pager::stat_map() + mms::pager::stat_unmap())
    + 3000 * (mms::pager::stat_in() + mms::pager::stat_out())
    + 150  * (mms::pager::stat_zero())
    + 1    * (OutFilePrinter::InstructionCount() + 1);
  os << "SUM "
     << OutFilePrinter::InstructionCount() + 1
     << " U=" << mms::pager::stat_unmap()
     << " M=" << mms::pager::stat_map()
     << " I=" << mms::pager::stat_in()
     << " O=" << mms::pager::stat_out()
     << " Z=" << mms::pager::stat_zero()
     << " ===> " << total;
  return os.str();
}