#pragma once
#include "err/error.h"
#include "decl.h"
namespace analib
{
//represent program current state
class Context final
{
public:
  Context();
  //this declaration should be a local var, global var or something similar
  //
  void addDecl(Decl *decl);

  Context &operator+();
  Context &operator-();
  bool operator==(const Context &rhs) const;

private:
  //three memory region
};
} // namespace analib