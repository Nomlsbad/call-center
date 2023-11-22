#ifndef CALLDETAILRECORD_H
#define CALLDETAILRECORD_H

#include "Types.h"

enum class CallEndingStatus: uint8_t
{
    OK = 0,
    TIMEOUT = 1,
    OVERLOAD = 2,

    NONE = 3
};


class CallDetailRecord
{
public:
    explicit CallDetailRecord(std::string phone): id(nextId++), phone(std::move(phone)) {};

    CallDetailRecord(const CallDetailRecord& cdr) = delete;
    CallDetailRecord(CallDetailRecord&& cdr) = default;

    CallDetailRecord& operator=(const CallDetailRecord& cdr) = delete;
    CallDetailRecord& operator=(CallDetailRecord&& cdr) = default;

    ~CallDetailRecord() = default;

public:
    [[nodiscard]] std::string makeCallReport() const;

    [[nodiscard]] IdType getId() const { return id; }

private:
    [[nodiscard]] std::string getEndingStatusAsString() const;

public:
    void recordCallReceiption();
    void recordCallResponse(IdType acceptedOperatotId);
    void recordCallEnding(CallEndingStatus status);

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
    inline static IdType nextId = 1;
};

#endif //CALLDETAILRECORD_H
