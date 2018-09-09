#pragma once

namespace analib
{

/// SVal - This represents a symbolic expression, which can be either
///  an L-value or an R-value.
///
enum class SVAL_KIND
{

};
class Sval
{
  public:
    SVAL_KIND getRawKind() const { return Kind_; }

  private:
    SVAL_KIND Kind_;
};
} // namespace analib