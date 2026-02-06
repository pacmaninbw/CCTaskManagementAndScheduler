#include <QTimeZone>
#include "stdChronoToQTConversions.h"

QDateTime chronoTimePointToQDateTime(std::chrono::system_clock::time_point inputChronoTP)
{
    auto duration = inputChronoTP.time_since_epoch();
    auto msecs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    QDateTime qdt = QDateTime::fromMSecsSinceEpoch(msecs, Qt::UTC);

    return qdt;
}

std::chrono::system_clock::time_point qDateTimeToChrono(const QDateTime& qtDateTime) {
    // Ensure the QDateTime is in UTC for a direct conversion to std::chrono::system_clock
    // which typically uses the Unix epoch (UTC) as its reference.

    QDateTime utcDateTime = qtDateTime;
    if (qtDateTime.timeZone() != QTimeZone::utc()) {
        utcDateTime = qtDateTime.toUTC();
    }

    // Get milliseconds since the epoch
    qint64 msecsSinceEpoch = utcDateTime.toMSecsSinceEpoch();

    // Create a std::chrono::milliseconds duration
    std::chrono::milliseconds durationSinceEpoch(msecsSinceEpoch);

    // Create the std::chrono::system_clock::time_point
    std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::from_time_t(0) + durationSinceEpoch;

\    return timePoint;
}

std::chrono::year_month_day qDateToChrono(const QDate &qdate)
{
        // Requires C++20
    std::chrono::sys_days sysDays = qdate.toStdSysDays();

    // std::chrono::year_month_day can be constructed from std::chrono::sys_days
    std::chrono::year_month_day ymd{sysDays};
    return ymd;
}
