#include "driver.h"

#include <sstream>

using namespace XYZ;

Driver::Driver()
    : m_scanner(*this), m_parser(m_scanner, *this), m_location(0) {}

int Driver::parse() {
  m_location = 0;
  return m_parser.parse();
}

void Driver::clear() { m_location = 0; }

std::string Driver::str() const {
  std::stringstream s;
  s << "Driver: ";
  return s.str();
}

void Driver::switchInputStream(std::istream *is) {
  m_scanner.switch_streams(is, NULL);
}

void Driver::increaseLocation(unsigned int loc) {
  m_location += loc;
  cout << "increaseLocation(): " << loc << ", total = " << m_location << endl;
}

unsigned int Driver::location() const { return m_location; }
