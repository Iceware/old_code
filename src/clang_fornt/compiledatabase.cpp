#include "compiledatabase.h"
namespace analib
{

CompileDatabaseHelper::CompileDatabaseHelper()
{
}
void CompileDatabaseHelper::addCompileEntry(std::string directory, const std::vector<std::string> &args, std::string filename)
{
    //todo support cross platfrom
    std::string arg_temp = "/usr/bin/clang++ ";
    for (auto &arg : args)
    {
        arg_temp.append(arg);
        arg_temp.append(" ");
    }
    arg_temp.append(" -c ").append(filename);
    json temp;
    temp["directory"] = directory;
    temp["command"] = arg_temp;
    temp["file"] = filename;
    json_.push_back(temp);
}

ANA_ERR_CODE CompileDatabaseHelper::create()
{
    std::ofstream fs(out_path_);
    if (!fs.is_open())
        return ANA_ERR_CODE::ERR_FILE_CREATE_FAIL;
    fs << json_;
    fs.close();
    return ANA_ERR_CODE::SUCCESS;
}

} // namespace analib
