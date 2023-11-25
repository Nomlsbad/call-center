#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

class CallDetailRecordError : public std::logic_error
{
    using std::logic_error::logic_error;
};

#endif // EXCEPTIONS_H
