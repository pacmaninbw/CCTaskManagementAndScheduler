#ifndef STDCHRONOTOQTCONVERSIONS_H
#define STDCHRONOTOQTCONVERSIONS_H

#include <chrono>
#include <QDate>
#include <QDateTime>

extern QDateTime chronoTimePointToQDateTime(std::chrono::system_clock::time_point inputChronoTP);
extern std::chrono::system_clock::time_point qDateTimeToChrono(const QDateTime& qtDateTime);
extern std::chrono::year_month_day qDateToChrono(const QDate& qdate);

#endif // STDCHRONOTOQTCONVERSIONS_H
