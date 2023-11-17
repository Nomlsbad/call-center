#ifndef CALLDETAILRECORD_H
#define CALLDETAILRECORD_H

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace btime = boost::posix_time;


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

    using IdType = unsigned long long;
    using String = std::string;
    using Date = btime::ptime;
    using TimeDuration = btime::time_duration;

public:

    explicit CallDetailRecord(String phone);

    CallDetailRecord(const CallDetailRecord& cdr) = delete;
    CallDetailRecord(CallDetailRecord&& cdr) = default;

    CallDetailRecord& operator= (const CallDetailRecord& cdr) = delete;
    CallDetailRecord& operator= (CallDetailRecord&& cdr) = default;

    ~CallDetailRecord() = default;

public:

    [[nodiscard]] String makeCallReport() const;

    [[nodiscard]] IdType getId() const { return id; }

    void setCallReceiptDate(const Date& date) { receiptDate = date; }

    void setCallEndingDate(const Date& date) { endingDate = date; }

    void setCallEndingStatus(CallEndingStatus status) { endingStatus = status; }

    void setCallAnswerDate(const Date& date) { answerDate = date; }

    void setOperator(IdType id) { operatorId = id; }

    void setCallDuration(const TimeDuration& callDuration) { duration = callDuration; }

private:

    [[nodiscard]] String getEndingStatusAsString() const;
    [[nodiscard]] bool hasEndingStatus(CallEndingStatus status) const { return status == endingStatus; }

private:

    inline static IdType nextId{1};

    IdType id{0};

    String phone;

    Date receiptDate;

    Date endingDate;

    CallEndingStatus endingStatus{CallEndingStatus::NONE};

    Date answerDate;

    IdType operatorId{0};

    TimeDuration duration;

};

#endif //CALLDETAILRECORD_H
