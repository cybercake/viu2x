#include "Timestamp.h"

namespace viu2x {

    Timestamp::Timestamp() {
        m_timeInNanoSecond = 0;
    }

    Timestamp::Timestamp(const int64_t & timeInNanoSecond) {
        m_timeInNanoSecond = timeInNanoSecond;
    }

    Timestamp::Timestamp(const Timestamp & source) {
        m_timeInNanoSecond = source.getTimeInNanoSecond();
    }

    Timestamp::~Timestamp() {
    }

    double Timestamp::getTimeInSecond() const {
        return m_timeInNanoSecond * 1e-9;
    }

    double Timestamp::getTimeInMilliSecond() const {
        return m_timeInNanoSecond * 1e-6;
    }

    double Timestamp::getTimeInMicroSecond() const {
        return m_timeInNanoSecond * 1e-3;
    }

    const int64_t & Timestamp::getTimeInNanoSecond() const {
        return m_timeInNanoSecond;
    }

    Timestamp & Timestamp::operator = (const Timestamp & op) {
        m_timeInNanoSecond = op.getTimeInNanoSecond();
        return *this;
    }

    Timestamp Timestamp::operator + (const Timespan & op) {
        return Timestamp(m_timeInNanoSecond + op.getTimeInNanoSecond());
    }

    Timestamp Timestamp::operator - (const Timespan & op) {
        return Timestamp(m_timeInNanoSecond - op.getTimeInNanoSecond());
    }

    bool Timestamp::operator == (const Timestamp & op) {
        return m_timeInNanoSecond == op.getTimeInNanoSecond();
    }

    bool Timestamp::operator != (const Timestamp & op) {
        return m_timeInNanoSecond != op.getTimeInNanoSecond();
    }

    bool Timestamp::operator > (const Timestamp & op) {
        return m_timeInNanoSecond > op.getTimeInNanoSecond();
    }

    bool Timestamp::operator >= (const Timestamp & op) {
        return m_timeInNanoSecond >= op.getTimeInNanoSecond();
    }

    bool Timestamp::operator < (const Timestamp & op) {
        return m_timeInNanoSecond < op.getTimeInNanoSecond();
    }

    bool Timestamp::operator <= (const Timestamp & op) {
        return m_timeInNanoSecond <= op.getTimeInNanoSecond();
    }

    Timestamp Timestamp::now() {
        // @todo Use higher precision timer here!!!
        return Timestamp(GetTickCount() * 1000000);
    }
}
