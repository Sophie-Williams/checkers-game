#ifndef TERM_H
#define TERM_H

#include <string>

namespace Term
{
#define GEN_SGR(code) "\x1b[" #code "m"
  const std::string SetBlack   = GEN_SGR(30);
  const std::string SetRed     = GEN_SGR(31);
  const std::string SetGreen   = GEN_SGR(32);
  const std::string SetBrown   = GEN_SGR(33);
  const std::string SetBlue    = GEN_SGR(34);
  const std::string SetMagenta = GEN_SGR(35);
  const std::string SetCyan    = GEN_SGR(36);
  const std::string SetWhite   = GEN_SGR(37);
  const std::string ResetColor = GEN_SGR(39);

  const std::string SetBold    = GEN_SGR( 1);
  const std::string SetFaint   = GEN_SGR( 2);
  const std::string SetRegular = GEN_SGR(22);
#undef GEN_SGR
}

#endif // TERM_H
