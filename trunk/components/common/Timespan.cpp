#include "Timespan.h"

namespace viu2x {

    const Timespan Timespan::Infinite(9223372036854775807);

    const Timespan Timespan::Zero(0);

    Timespan::Timespan() {
        m_timeInNanoSecond = 0;
    }

    Timespan::Timespan(const int64_t & timeInNanoSecond) {
        m_timeInNanoSecond = timeInNanoSecond;
    }

    Timespan::Timespan(const int & timeInNanoSecond) {
        m_timeInNanoSecond = timeInNanoSecond;
    }

    Timespan::Timespan(const Timespan & source) {
        m_timeInNanoSecond = source.getTimeInNanoSecond();
    }

    Timespan::~Timespan() {
    }

    double Timespan::getTimeInSecond() const {
        return m_timeInNanoSecond * 1e-9;
    }

    double Timespan::getTimeInMilliSecond() const {
        return m_timeInNanoSecond * 1e-6;
    }

    double Timespan::getTimeInMicroSecond() const {
        return m_timeInNanoSecond * 1e-3;
    }

    const int64_t & Timespan::getTimeInNanoSecond() const {
        return m_timeInNanoSecond;
    }

    Timespan & Timespan::operator = (const Timespan & op) {
        m_timeInNanoSecond = op.getTimeInNanoSecond();
        return *this;
    }

    Timespan Timespan::operator + (const Timespan & op) {
        return Timespan(m_timeInNanoSecond + op.getTimeInNanoSecond());
    }

    Timespan Timespan::operator - (const Timespan & op) {
        return Timespan(m_timeInNanoSecond - op.getTimeInNanoSecond());
    }

    Timespan Timespan::operator * (const int op) {
        return Timespan(m_timeInNanoSecond * op);
    }

    Timespan Timespan::operator * (const double & op) {
        return Timespan((int64_t)(m_timeInNanoSecond * op));
    }

    Timespan Timespan::operator / (const int op) {
        return Timespan(m_timeInNanoSecond / op);
    }

    Timespan Timespan::operator / (const double & op) {
        return Timespan((int64_t)(m_timeInNanoSecond / op));
    }

    bool Timespan::operator == (const Timespan & op) {
        return m_timeInNanoSecond == op.getTimeInNanoSecond();
    }

    bool Timespan::operator != (const Timespan & op) {
        return m_timeInNanoSecond != op.getTimeInNanoSecond();
    }

    bool Timespan::operator > (const Timespan & op) {
        return m_timeInNanoSecond > op.getTimeInNanoSecond();
    }

    bool Timespan::operator >= (const Timespan & op) {
        return m_timeInNanoSecond >= op.getTimeInNanoSecond();
    }

    bool Timespan::operator < (const Timespan & op) {
        return m_timeInNanoSecond < op.getTimeInNanoSecond();
    }

    bool Timespan::operator <= (const Timespan & op) {
        return m_timeInNanoSecond <= op.getTimeInNanoSecond();
    }
}
