#pragma once
#include "../thirdparty/json.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

#include "../lib/err/error.h"
namespace fs = std::filesystem;
using json = nlohmann::json;

namespace analib
{

class CompileDatabaseHelper
{
public:
  CompileDatabaseHelper();

  void setOutPath(const fs::path &path)
  {
    out_path_ = path / "compile_commands.json";
  }

  void addCompileEntry(std::string directory, const std::vector<std::string> &args, std::string filename);

  ANA_ERR_CODE create();

private:
  json json_;

  //path for compilation database
  fs::path out_path_;
};
} // namespace analib
