#ifndef CALLDETAIL_H
#define CALLDETAIL_H

#include "Types.h"

enum class CallEndingStatus: uint8_t
{
    OK = 0,
    TIMEOUT = 1,
    OVERLOAD = 2,

    NONE = 3
};


class CallDetail
{
public:
    explicit CallDetail(std::string phone): phone(std::move(phone)) {};

    CallDetail(const CallDetail& cdr) = delete;
    CallDetail(CallDetail&& cdr) = default;

    CallDetail& operator=(const CallDetail& cdr) = delete;
    CallDetail& operator=(CallDetail&& cdr) = default;

    ~CallDetail() = default;

public:
    [[nodiscard]] std::string toString() const;

    [[nodiscard]] IdType getId() const { return id; }

private:
    [[nodiscard]] std::string getEndingStatusAsString() const;

public:
    void recordReceiption(Date date);
    void recordResponse(IdType acceptedOperatotId, Date date);
    void recordEnding(CallEndingStatus status, Date date);

private:
    IdType id = 0;

    std::string phone;

    Date receiptDate;

    Date responseDate;

    IdType operatorId = 0;

    Date endingDate;

    CallEndingStatus endingStatus{CallEndingStatus::NONE};

    TimeDuration duration;

private:
    static IdType nextId;
};

#endif //CALLDETAIL_H
