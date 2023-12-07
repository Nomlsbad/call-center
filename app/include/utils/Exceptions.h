#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "utils/Macros.h"

namespace CCenter
{

class Exception : public std::exception
{
public:

    using std::exception::exception;

    explicit Exception(const char* reason)
        : reason(reason)
    {
    }

    explicit Exception(std::string reason)
        : reason(std::move(reason))
    {
    }

    [[nodiscard]] const char* what() const noexcept override
    {
        return reason.c_str();
    }

private:

    std::string reason = "";
};

DECLARE_EXCEPTION(WrongRocordingOrder, Exception);
DECLARE_EXCEPTION(AlreadyInQueue, Exception);
DECLARE_EXCEPTION(Overload, Exception);
DECLARE_EXCEPTION(UnkownConfiguration, Exception);
DECLARE_EXCEPTION(DoubleResponse, Exception);

} // namespace ccenter

#endif // EXCEPTIONS_H
