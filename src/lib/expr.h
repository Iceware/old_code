#pragma once
#include "statement.h"
namespace analib
{
class Expr : public Stmt
{
  public:
    virtual ANA_ERR_CODE run(Context *context);
  private:
};
} // namespace analib