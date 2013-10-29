#ifndef TIMESPAN_H
#define TIMESPAN_H

#include "Viu2x.h"

namespace viu2x {

    class Timespan : public Object {

        public:
            Timespan();

            Timespan(const int64_t & timeInNanoSecond);

            Timespan(const int & timeInNanoSecond);

            Timespan(const Timespan & source);

            virtual ~Timespan();

            double getTimeInSecond() const;

            double getTimeInMilliSecond() const;

            double getTimeInMicroSecond() const;

            const int64_t & getTimeInNanoSecond() const;

            Timespan & operator = (const Timespan & op);

            Timespan operator + (const Timespan & op);

            Timespan operator - (const Timespan & op);

            Timespan operator * (const int op);

            Timespan operator * (const double & op);

            Timespan operator / (const int op);

            Timespan operator / (const double & op);

            bool operator == (const Timespan & op);

            bool operator != (const Timespan & op);

            bool operator > (const Timespan & op);

            bool operator >= (const Timespan & op);

            bool operator < (const Timespan & op);

            bool operator <= (const Timespan & op);

            bool isInfinite() const;

            static const Timespan Infinite;

            static const Timespan Zero;

        private:
            int64_t m_timeInNanoSecond;
    };

}
#endif // TIMESPAN_H
