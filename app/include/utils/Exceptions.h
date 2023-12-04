#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

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

class CallDetailRecordError : public Exception
{
    using Exception::Exception;
};

class Overload : public Exception
{
    using Exception::Exception;
};

} // namespace ccenter

#endif // EXCEPTIONS_H
