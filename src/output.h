#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdarg>

enum OutputMode
{
  OPERATIONS   = 1 << 0,
  FINAL_PAGES  = 1 << 1,
  FINAL_FRAMES = 1 << 2,
  SUMMARY      = 1 << 3,

  INFO         = 1 << 4,
  DEBUG        = 1 << 5
};

////////////////////////////////////////////

template <typename OutputPolicy>
struct Printer
{
  ~Printer();

  std::ostringstream& Get(OutputMode mode);

  static unsigned int& ReportingMode();

protected:
  std::ostringstream os;
};

//------------------------------------------

template <typename OutputPolicy>
Printer<OutputPolicy>::~Printer() {
  OutputPolicy::Output( os.str() );
}

template <typename OutputPolicy>
std::ostringstream& Printer<OutputPolicy>::Get(OutputMode mode) {
  return os;
}

template <typename OutputPolicy>
unsigned int & Printer<OutputPolicy>::ReportingMode() {
  static unsigned int _reporting_mode = 0;
  return _reporting_mode;
}

////////////////////////////////////////////

struct OutputToFile
{
  inline static void Output( const std::string& msg ) {
    FILE* pstream = Stream();
    if( pstream ) {
      fprintf(pstream, "%s\n", msg.c_str());
      fflush( pstream );
    }
  }

  inline static FILE*& Stream() {
    static FILE* pstream = stderr;
    return pstream;
  }
};

/////////////////////////////////////////////

#define OUT(mode) \
  if ( !(mode & Printer<OutputToFile>::ReportingMode()) || !OutputToFile::Stream()) ; \
  else Printer<OutputToFile>().Get(mode)

#endif //__OUTPUT_H__
