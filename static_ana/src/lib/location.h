#pragma once
#include <string>
namespace analib
{

//record the file row,col of the code
class Location
{
public:
  Location(){};
  Location(unsigned int line, unsigned int column, const std::string &filename) : line_(line), column_(column), filename_(filename) {}

  //S should support operator <<
  template <class S>
  void fill(unsigned int line_, unsigned int column_, const S &s)
  {
    filename_.clear();
    filename_ << s;
    line_ = line_;
    column_ = column_;
  }

  bool operator==(const Location &rhs) const
  {
    return (this->line_ == rhs.line_) &&
           (this->column_ == rhs.column_) &&
           (this->filename_ == rhs.filename_);
  }

  bool operator!=(const Location &rhs) const
  {
    return !(*this == rhs);
  }

  unsigned int getLine() const
  {
    return line_;
  }

  unsigned int getColumn() const
  {
    return column_;
  }

  std::string getFilename() const
  {
    return filename_;
  }

private:
  unsigned int line_;
  unsigned int column_;
  //full path of the file
  std::string filename_;
};

} // namespace analib

//---------------------------------------------
//add hash for location
namespace std
{
template <>
struct hash<analib::Location>
{
  typedef analib::Location argument_type;
  typedef std::size_t result_type;

  result_type operator()(argument_type const &arg) const
  {
    std::string s = arg.getFilename();
    s.append("#").append(to_string(arg.getLine()));
    s.append("#").append(to_string(arg.getColumn()));
    return std::hash<std::string>{}(s);
  }
};
} // namespace std
  //------------------------------------------------------------