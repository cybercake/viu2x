#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include "Viu2x.h"
#include "Timespan.h"

namespace viu2x {

    class Timestamp : public Object {

        public:
            Timestamp();

            Timestamp(const int64_t & timeInNanoSecond);

            Timestamp(const Timestamp & source);

            virtual ~Timestamp();

            double getTimeInSecond() const;

            double getTimeInMilliSecond() const;

            double getTimeInMicroSecond() const;

            const int64_t & getTimeInNanoSecond() const;

            Timestamp & operator = (const Timestamp & op);

            Timestamp operator + (const Timespan & op);

            Timestamp operator - (const Timespan & op);

            bool operator == (const Timestamp & op);

            bool operator != (const Timestamp & op);

            bool operator > (const Timestamp & op);

            bool operator >= (const Timestamp & op);

            bool operator < (const Timestamp & op);

            bool operator <= (const Timestamp & op);

            static Timestamp now();

        private:
            int64_t m_timeInNanoSecond;
    };

}
#endif // TIMESTAMP_H
