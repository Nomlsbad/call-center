#ifndef CALLDETAIL_H
#define CALLDETAIL_H

#include "utils/Types.h"

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

    void recordReceiption(Date date);
    void recordResponse(IdType acceptedOperatotId, Date date);
    void recordEnding(CallEndingStatus status, Date date);

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] IdType getId() const;
    [[nodiscard]] IdType getOperatorId() const;
    [[nodiscard]] std::string getPhone() const;

private:

    [[nodiscard]] std::string getEndingStatusAsString() const;

private:

    static IdType nextId;

private:

    IdType id = 0;

    std::string phone;

    Date receiptDate;

    Date responseDate;

    IdType operatorId = 0;

    Date endingDate;

    CallEndingStatus endingStatus = CallEndingStatus::NONE;

    TimeDuration duration;
};

#endif // CALLDETAIL_H
