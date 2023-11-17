#ifndef CALLDETAILRECORD_H
#define CALLDETAILRECORD_H


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
    using Date = boost::posix_time::ptime;
    using TimeDuration = boost::posix_time::time_duration;

public:
    explicit CallDetailRecord(String phone): id(nextId++), phone(std::move(phone)) {};

    CallDetailRecord(const CallDetailRecord& cdr) = delete;
    CallDetailRecord(CallDetailRecord&& cdr) = default;

    CallDetailRecord& operator=(const CallDetailRecord& cdr) = delete;
    CallDetailRecord& operator=(CallDetailRecord&& cdr) = default;

    ~CallDetailRecord() = default;

public:
    [[nodiscard]] String makeCallReport() const;

    [[nodiscard]] IdType getId() const { return id; }

private:
    [[nodiscard]] String getEndingStatusAsString() const;

public:
    void recordCallReceiption();
    void recordCallResponse(IdType acceptedOperatotId);
    void recordCallEnding(CallEndingStatus status);

private:
    IdType id = 0;

    String phone;

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
