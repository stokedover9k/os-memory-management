#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "loglib.h"
#include "random_gen.h"
#include "pte.h"
#include "mmu.h"

#define NUM_PAGES (64)

char const * const USAGE_STR = "Usage: ./mmu [-a<algo>] [-o<options>] [–f<num_frames>] inputfile randomfile";

namespace PARAMS
{
  enum {
    OPERATIONS   = 1 << 0,
    FINAL_PAGES  = 1 << 1,
    FINAL_FRAMES = 1 << 2,
    SUMMARY      = 1 << 3 };

  static char algo = 'l';
  static char output = OPERATIONS | FINAL_PAGES | FINAL_FRAMES | SUMMARY;
  static unsigned int num_frames = 32;
  static char const * inputfile = NULL;
  static char const * randomfile = NULL;
};

void parse_args(int argc, char const *argv[]);

//===================== MAIN ===========================//

int main(int argc, char const *argv[])
{
  using namespace std;

  //------------ parse arguments -------------//
  try { parse_args(argc, argv); }
  catch (std::invalid_argument const& ia) {
    std::cerr << "Error (INVALID ARGUMENT): " << ia.what() << "\n\t" << USAGE_STR << '\n';
    exit(1); }

  // open file for logging
  Output2FILE::Stream() = fopen( "trace.log", "w" );

  //----------- set up random generator ------//
  {
    ifstream inrandom(PARAMS::randomfile);
    int numbers;
    inrandom >> numbers;

    std::vector<int> rvec(numbers);
    rvec.assign(istream_iterator<int>(inrandom), istream_iterator<int>());
    rnd::looping_random_iterator<std::vector<int> >::set_container(std::move(rvec));
  }

  cout << "done" << endl;
}

// end: MAIN ===========================================//

void parse_args(int argc, char const *argv[]) {
  if( argc <= 2 )
    throw std::invalid_argument("must provide input and random files");
  PARAMS::inputfile  = argv[argc-2];
  PARAMS::randomfile = argv[argc-1];

  for (int i = 1; i < argc - 2; ++i)
  {
    std::string arg( argv[i] );

    std::cout << i << " = " << arg << std::endl;

    if( arg.length() < 3 || arg[0] != '-' )
      throw std::invalid_argument(arg);

    switch( arg[1] ) {
      case 'a': 
        if( arg.length() != 3 ) throw std::invalid_argument(arg);
        PARAMS::algo = arg[2];
        break;
      case 'o':
        PARAMS::output = 0;
        for (unsigned int j = 2; j < arg.length(); ++j)
          switch( arg[j] ) {
            case 'O':  PARAMS::output |= PARAMS::OPERATIONS;    break;
            case 'P':  PARAMS::output |= PARAMS::FINAL_PAGES;   break;
            case 'F':  PARAMS::output |= PARAMS::FINAL_FRAMES;  break;
            case 'S':  PARAMS::output |= PARAMS::SUMMARY;       break;
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
