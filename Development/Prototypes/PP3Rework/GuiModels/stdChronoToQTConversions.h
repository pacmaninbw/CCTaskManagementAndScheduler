#ifndef STDCHRONOTOQTCONVERSIONS_H
#define STDCHRONOTOQTCONVERSIONS_H

#include <chrono>
#include <QDateTime>

extern QDateTime chronoTimePointToQDateTime(std::chrono::system_clock::time_point inputChronoTP);
extern std::chrono::system_clock::time_point qDateTimeToChrono(const QDateTime& qtDateTime);

#endif // STDCHRONOTOQTCONVERSIONS_H
