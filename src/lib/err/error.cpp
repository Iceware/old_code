#include "error.h"
namespace analib
{
const char *ANA_ERRCategory::name() const noexcept
{
    return "static_ana";
}

std::string ANA_ERRCategory::message(int ev) const
{
    switch (static_cast<ANA_ERR_CODE>(ev))
    {
    case ANA_ERR_CODE::SUCCESS:
        return "Success";
        break;
    case ANA_ERR_CODE::ERR_CLANG_PARSE:
        return "Unable to parse translation unit";
        break;
    case ANA_ERR_CODE::ERR_FILE_NOT_FUND:
        return "Specific file not found";
        break;
    case ANA_ERR_CODE::ERR_FILE_CREATE_FAIL:
        return "failed in file creation";
        break;
    default:
        return "Unknown error"
    }
}

const ANA_ERRCategory theANA_ERRCategory{};

std::error_code make_error_code(ANA_ERR_CODE e)
{
    return {static_cast<int>(e), theANA_ERRCategory};
}
} // namespace analib
