#include "./Pattern/include/Pattern.hpp"
#include <string>
#include <vector>
#include "./Pattern/include/DEPFET.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <exception>
#include <stdexcept>

void Pattern::emptyPattern ()
{
  pattern.clear();
  for (size_t i=0; i<ROWS; i++)
  {
    std::vector < uint8_t> row (COLUMNS);
    std::fill (row.begin(),row.end(), 0);
    pattern.push_back (row);
  }
  lastRow = MAX_LAST_ROW;
}

Pattern::Pattern ()
{
  emptyPattern ();
}

void Pattern::readFile (std::string filename)
{
  pattern.clear();
  std::ifstream file;
  if (filename == "")
  {
    emptyPattern();
  }
  try
  {
    file.open (filename, std::ios::binary);
    file.seekg (0, std::ios::beg);
    while (not file.eof () and (pattern.size() < ROWS))
    {
      std::vector <uint8_t> buffer (COLUMNS);
      file.read (reinterpret_cast<char *> (&(buffer[0])), COLUMNS);
      if (buffer.size() != COLUMNS)
      {
        buffer.resize (COLUMNS);
      }
      pattern.push_back (buffer);
    }
    file.close();
    while (pattern.size () < ROWS)
    {
      std::vector <uint8_t> buffer (COLUMNS, 0);
      pattern.push_back (buffer);
    }
  }
  catch (std::ios_base::failure& e)
  {
    std::cout << "Pattern file: IO exception: " << e.what () << std::endl;
  }
}

void Pattern::writeFile (std::string filename)
{
  std::ofstream file;
  if (pattern.size () != ROWS)
  {
    std::stringstream ss;
    ss << "Internal error in "<<__FILE__<<", line No."<<__LINE__<<",\
    Pattern.size() == "<<pattern.size();
    std::cerr << ss.str();
    throw std::logic_error (ss.str());
  }
  try
  {
    file.open (filename, std::ios::binary | std::ios::out);
    for (auto&row: pattern)
    {
      if (row.size() != COLUMNS)
      {
        std::stringstream ss;
        ss << "Internal error in "<<__FILE__<<", line No."<<__LINE__;
        std::cerr << ss.str();
        throw std::logic_error (ss.str());
      }
      std::vector <char> tmp_buf;
      for (auto &it: row)
      {
        tmp_buf.push_back (static_cast <char> (it));
      }
      file.write (&tmp_buf.front(), tmp_buf.size());
    }
    file.close();
  }
  catch (std::ios_base::failure& e)
  {
    std::cout << "Pattern file: IO exception: " << e.what () << std::endl;
  }
}

Pattern::Pattern (std::string filename)
{
  emptyPattern();
  readFile (filename);
}

void Pattern::copyMem (const uint8_t (&Array) [ROWS][COLUMNS])
{
  pattern.clear();
  for (size_t i = 0; i < ROWS; i++)
  {
    std::vector < uint8_t > buffer;
    for (size_t j = 0; j < COLUMNS; j++)
    {
      buffer.push_back (Array[i][j]);
    }
    pattern.push_back (buffer);
  }
}

void Pattern::copyVector(const std::vector < std::vector < uint8_t > >& Array)
{
  pattern.clear();
  for (size_t i = 0; i < ROWS; i++)
  {
    std::vector < uint8_t > buffer;
    for (size_t j = 0; j < COLUMNS; j++)
    {
      if (i<Array.size() and j < Array[i].size())
      {
        buffer.push_back (Array[i][j]);
      }
      else
      {
        buffer.push_back (0);
      }
      pattern.push_back (buffer);
    }
  }
}

Pattern::Pattern (const uint8_t (&Array )[ROWS][COLUMNS])
{
  emptyPattern();
  copyMem (Array);
}

Pattern::Pattern (const std::vector < std::vector < uint8_t > >& Array)
{
  emptyPattern();
  copyVector (Array);
}

Pattern::Pattern (const uint8_t (&Array )[ROWS][COLUMNS], std::string filename)
{
  copyMem (Array);
  writeFile (filename);
} 

Pattern::Pattern (const std::vector < std::vector < uint8_t > >& Array,
    std::string filename)
{
  emptyPattern();
  copyVector (Array);
  writeFile (filename);
}

uint8_t Pattern::getAmplitude (uint16_t row, uint8_t column) const
{
  if (pattern.size () != ROWS)
  {
    std::stringstream ss;
    ss << "Internal error in "<<__FILE__<<", line No."<<__LINE__
    <<"Pattern.size() == "<<pattern.size();
    throw std::logic_error (ss.str());
  }
  if ((row >= ROWS) or (column >= COLUMNS))
    return 0;
  if (pattern[row].size() != COLUMNS)
  {
    std::stringstream ss;
    ss << "Internal error in "<<__FILE__<<", line No."<<__LINE__;
    throw std::logic_error (ss.str());
  }
  return pattern [row][column];
}

void Pattern::dump () const
{
  std::cout << "Pattern: " << std::endl;
  for (auto &row: pattern)
  {
    for (auto &it: row)
    {
      std::cout << std::hex << (unsigned int)it << " "<<std::dec;
    }
    std::cout << ";" <<std::endl;
  }
}
void Pattern::pushHit (uint16_t row, uint8_t column, uint8_t amplitude)
{
  if ((row>=ROWS) or (column >= COLUMNS))
    return;
  if (pattern.size() != ROWS)
  {
    std::stringstream ss;
    ss << "Internal error in "<<__FILE__<<", line No."<<__LINE__;
    throw std::logic_error (ss.str());
  }
  if (pattern[row].size() != COLUMNS)
  {
    std::stringstream ss;
    ss << "Internal error in "<<__FILE__<<", line No."<<__LINE__;
    throw std::logic_error (ss.str());
  }
  pattern[row][column] = amplitude;
}

void Pattern::fillArray (unsigned int (&Array) [ROWS][COLUMNS])
{
  for (uint16_t i=0; i < ROWS; i++)
    for (uint8_t j=0; j < COLUMNS; j++)
    {
      Array [i][j] = pattern [i][j];
    }
}

bool operator== (const Pattern&left, const Pattern& right)
{
  if (left.getLastRow () != right.getLastRow())
    return false;
  for (uint16_t i = 0; i < left.getLastRow(); i++)
  {
    for (uint8_t j = 0; j < COLUMNS; j++)
    {
      if (left.getAmplitude (i, j) != right.getAmplitude (i,j))
        return false;
    }
  }
  return true;
}

bool operator!= (const Pattern&left, const Pattern& right)
{
  return not (left == right);
}
#if 0
void Pattern::diff (const Pattern& right, int res
  [ROWS][COLUMNS])
{
  for (uint16_t i = 0; i < ROWS; i++)
    for (uint8_t j = 0; j < COLUMNS; j++)
      res [i][j] = this->getAmplitude (i,j) - right.getAmplitude (i,j);
}
#endif

