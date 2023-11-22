#ifndef OPERATOR_H
#define OPERATOR_H

#include <thread>

class Operator
{
public:
    using IdType = unsigned long long;

public:
    Operator(): id(nextId++) {};

    void acceptCall(class CallDetail& callDetail);

private:

    void talk();

private:
    IdType id = 0;

    bool isBusy = false;

    std::thread callThread;

    static IdType nextId;
};


#endif //OPERATOR_H
