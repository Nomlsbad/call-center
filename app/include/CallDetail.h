#ifndef CALLDETAIL_H
#define CALLDETAIL_H

#include "Types.h"

enum class CallEndingStatus : uint8_t
{
    OK = 0,
    TIMEOUT = 1,
    OVERLOAD = 2,

    NONE = 3
};

class CallDetail
{
public:

    explicit CallDetail(std::string phone);

    CallDetail(const CallDetail& cdr) = delete;
    CallDetail(CallDetail&& cdr) = default;

    CallDetail& operator=(const CallDetail& cdr) = delete;
    CallDetail& operator=(CallDetail&& cdr) = default;

    ~CallDetail() = default;

public:

    void recordReceiption(Date date);
    void recordResponse(IdType acceptedOperatotId, Date date);
    void recordEnding(CallEndingStatus status, Date date);

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] IdType getId() const;
    [[nodiscard]] IdType getOperatorId() const;

private:

    [[nodiscard]] std::string getEndingStatusAsString() const;

private:

    IdType id = 0;

    std::string phone;

    Date receiptDate;

    Date responseDate;

    IdType operatorId = 0;

    Date endingDate;

    CallEndingStatus endingStatus{CallEndingStatus::NONE};

    TimeDuration duration;

    static IdType nextId;
};

#endif // CALLDETAIL_H
