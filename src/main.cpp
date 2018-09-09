#include <iostream>
#include <fstream>
#include <filesystem>
#include <glog/logging.h>
#include "thirdparty/cxxopts/include/cxxopts.hpp"
#include "clang-c/Index.h"
#include "clang_fornt/analyzer.h"

namespace fs = std::filesystem;

void setUpGlog()
{
  FLAGS_log_dir = fs::current_path();
  FLAGS_logtostderr = 1;
}

int main(int argc, char **argv)
{
  //todo pass this from command line
  fs::path out_path = fs::path(argv[0]);
  //get the absolute path
  out_path = fs::canonical(out_path).parent_path();

  fs::path dir_path;
  fs::path main_file_path;

  // first setup glog
  setUpGlog();
  google::InitGoogleLogging(argv[0]);

  cxxopts::Options options(argv[0], "toy static analyzer");
  options.add_options()("d,debug", "Enable debugging")("f,file", "File to analyze, should be the entry point of programe", cxxopts::value<std::string>())("i,include", "the extra include directory", cxxopts::value<std::string>())("I", "the folder to analyze,analyzer will only analyze the file in this folder", cxxopts::value<std::string>())("h,help", "Print help");

  auto result = options.parse(argc, argv);

  std::string target_dir;
  std::string include_dir;

  if (result.count("help"))
  {
    std::cout << "must have file and I options"
              << "\n";
    std::cout << options.help() << std::endl;
  }
  //must have file and I param
  else if (!result.count("file") || result["file"].as<std::string>().empty())
  {
    std::cout << "must have file option"
              << "\n";
    std::cout << options.help() << std::endl;
    return 0;
  }
  else
  {
    main_file_path = result["file"].as<std::string>();
    // check file existance
    if (!fs::exists(main_file_path) || fs::is_directory(main_file_path))
    {
      std::cout << "file " << main_file_path << " not found"
                << "\n";
      return 1;
    }

    main_file_path = fs::canonical(main_file_path);
    dir_path = main_file_path.parent_path();
  }

  //whether have specific target dir
  if (result.count("I") && !result["I"].as<std::string>().empty())
  {
    dir_path = result["I"].as<std::string>();
    dir_path = fs::canonical(dir_path);
  }

  //whether have any extra include dir
  if (result.count("include") && !result["include"].as<std::string>().empty())
  {
    include_dir = result["include"].as<std::string>();
  }

  //change the relative path to absolute path

  LOG(INFO) << "start with:\n"
            << "file: " << main_file_path
            << "\nI: " << dir_path
            << "\nout path " << out_path << "\n";

  analib::ClangAnalyzer analyzer(main_file_path, dir_path, {}, out_path);
  analyzer.init_main();
  return 0;
}
