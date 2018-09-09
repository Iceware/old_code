#pragma once
#include <system_error>
namespace analib
{
enum class ANA_ERR_CODE
{
    SUCCESS = 0,
    ERR_CLANG_PARSE = 1,
    ERR_FILE_NOT_FUND,
    ERR_FILE_CREATE_FAIL,
};

class ANA_ERRCategory : public std::error_category
{
    const char *name() const noexcept override;
    std::string message(int ev) const override;
};

std::error_code make_error_code(ANA_ERR_CODE e);
} // namespace analib
