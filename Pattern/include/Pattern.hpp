#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include "DEPFET.hpp"
/// Class Pattern describes 768x64 pattern which could be uploadet into DHPT
///memory
class Pattern
{
  private:
    std::vector < std::vector < uint8_t > > pattern;
    void copyMem (const uint8_t (&Array) [ROWS][COLUMNS]);
    void copyVector(const std::vector < std::vector < uint8_t > >& Array);
    void readFile (std::string filename);
    void writeFile (std::string filename);
    void emptyPattern ();
    unsigned int lastRow; // maximum: 767 (physical).
  public:
    /// Zero pattern
    Pattern ();
    ///Read pattern from the file
    Pattern (std::string filename);
    ///Read pattern from the PC memory
    Pattern (const uint8_t (&Array )[ROWS][COLUMNS]);
    /// Read pattern from the PC memory
    Pattern (const std::vector < std::vector < uint8_t > >& Array);
    ///The constructor opens file in written mode, the pattern is read from the
    /// PC memory
    Pattern (const uint8_t (&Array) [ROWS][COLUMNS], std::string filename);
    /// Read pattern from the PC memory and write into file
    Pattern (const std::vector < std::vector < uint8_t > >& Array, std::string
      filename);
    /// Return Amplitude with corresponding coordinates
    uint8_t getAmplitude (uint16_t row, uint8_t column) const;
    void pushHit (uint16_t row, uint8_t column, uint8_t amplitude);
    void fillArray (unsigned int (&Array) [ROWS][COLUMNS]);
    void dump () const;
    void setLastRow (unsigned int f_lastRow) {if (f_lastRow < MAX_LAST_ROW)
      lastRow = f_lastRow; else lastRow = MAX_LAST_ROW;};
    unsigned int getLastRow () const {return lastRow;};
};

bool operator== (const Pattern& left, const Pattern& right);
bool operator!= (const Pattern& left, const Pattern& right);
