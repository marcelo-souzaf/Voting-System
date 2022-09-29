#ifndef DATE_HPP_
#define DATE_HPP_

#include <iostream>
#include <fstream>

typedef signed char int8;

namespace Time {
    constexpr unsigned int SECOND = 1;
    constexpr unsigned int MINUTE = 60 * SECOND;
    constexpr unsigned int HOUR = 60 * MINUTE;
    constexpr unsigned int DAY = 24 * HOUR;
    constexpr unsigned int MONTH = 30 * DAY;
    constexpr unsigned int YEAR = 365 * DAY;
    constexpr unsigned int cumulative_days[12] = {
        31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    constexpr int table[12] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    constexpr const char* week_days[7] = {"Domingo", "Segunda", "Terça", "Quarta",
                                        "Quinta", "Sexta", "Sábado"};
}  // namespace Time

// Classe descartada que funcionaria com RadixSort
// class Date {
//  public:
//     char date[16];

//     Date() {
//         // for (int i = 0; i < 16; i++) {
//         //     this->date[i] = 0;
//         // }
//         this->date[0] = 0;
//     }

//     Date(const Date& other) {
//         if (other.date[0] == 0) {
//             this->date[0] = 0;
//         } else {
//             std::copy(other.date, other.date + 16, this->date);
//         }
//     }

//     Date(short year, uint8 month, uint8 day,
//     uint8 hour = 0, uint8 minute = 0, uint8 second = 0) {
//         if (month > 12 || month < 1) {
//             throw std::invalid_argument("Mês inválido.");
//         }
//         if (day > 31 || day < 1) {
//             throw std::invalid_argument("Dia inválido.");
//         }
//         if (hour > 23) {
//             throw std::invalid_argument("Hora inválida.");
//         }
//         if (minute > 59) {
//             throw std::invalid_argument("Minuto inválido.");
//         }
//         if (second > 59) {
//             throw std::invalid_argument("Segundo inválido.");
//         }
//         sprintf(this->date, "%04u%02u%02u%02u%02u%02u%02u",
//             year, month, day, hour, minute, second,
//             this->compute_weekday(year, month, day));
//     }

//     Date(time_t remaining_seconds, bool local = true) {
//         if (local) {
//             remaining_seconds -= Time::HOUR * 3;
//         }
//         short year = remaining_seconds / Time::YEAR;
//         remaining_seconds %= Time::YEAR;
//         // 1972 foi o primeiro ano bissexto depois de 1970, que é o valor 0
//         bool is_leap_year = (year + 2) % 4 == 0;
//         short leap_year_count = (year + 2) / 4 - (is_leap_year ? 1 : 0);
//         // Se não há segundos suficientes para remover os dias de ano bissexto,
//         // subtrai um ano do número de anos e adiciona os segundos de um ano
//         if (remaining_seconds < leap_year_count * Time::DAY) {
//             --year;
//             remaining_seconds += Time::YEAR;
//         }
//         remaining_seconds -= leap_year_count * Time::DAY;
//         year += 1970;
//         // Obtém o número de dias do ano, somando 1 para que o dia 1 seja 1
//         uint8 month;
//         short day = (remaining_seconds / Time::DAY) + 1;
//         remaining_seconds %= Time::DAY;
//         // Compara com o número de dias de cada mês para obter o número do mês
//         for (short i = 0; i < 12; ++i) {
//             if (day <= Time::cumulative_days[i]) {
//                 // Se o ano é bissexto, subtrai um dia
//                 if (is_leap_year && i > 1) {
//                     --day;
//                     // Se o dia era o primeiro do mês antes da subtração,
//                     // decrementa o valor do mês
//                     if (day == Time::cumulative_days[i - 1]) {
//                         --i;
//                         // Se o mês é fevereiro, soma um dia e chega em 29
//                         if (i == 1) {
//                             ++day;
//                         }
//                     }
//                 }
//                 month = i + 1;
//                 day -= i == 0 ? 0 : Time::cumulative_days[i - 1];
//                 break;
//             }
//         }
//         // Obtém o número de horas do dia
//         uint8 hour = remaining_seconds / Time::HOUR;
//         remaining_seconds %= Time::HOUR;
//         // Obtém o número de minutos da hora
//         uint8 minute = remaining_seconds / Time::MINUTE;
//         uint8 second = remaining_seconds % Time::MINUTE;
//         // Obtém o dia da semana
//         sprintf(this->date, "%04u%02u%02u%02u%02u%02u%02u",
//             year, month, day, hour, minute, second,
//             this->compute_weekday(year, month, day));
//     }

//     uint8 compute_weekday(short year, uint8 month, uint8 day) {
//         uint8 weekday;
//         if (month < 3) --year;
//         weekday = std::abs(static_cast<int>(year + year / 4 - year / 100 + \
//                             year / 400 + Time::table[month - 1] + day)) % 7;
//         if (month < 3) ++year;
//         return weekday;
//     }
// };

// Apenas para datas de 1970 a 2099
class Date {
    public:
    short year;
    int8 month;
    int8 day;
    int8 hour;
    int8 minute;
    int8 second;
    int8 weekday;

    Date() {
        this->year = 0;
        this->month = 0;
        this->day = 0;
        this->hour = 0;
        this->minute = 0;
        this->second = 0;
        this->weekday = 0;
    }

    Date(short year, int8 month, int8 day,
    int8 hour = 0, int8 minute = 0, int8 second = 0, int8 weekday = 0) {
        this->year = year;
        if (month > 12 || month < 1) {
            throw std::invalid_argument("Mês inválido.");
        }
        this->month = month;
        if (day > 31 || day < 1) {
            throw std::invalid_argument("Dia inválido.");
        }
        this->day = day;
        if (hour > 23 || hour < 0) {
            throw std::invalid_argument("Hora inválida.");
        }
        this->hour = hour;
        if (minute > 59 || minute < 0) {
            throw std::invalid_argument("Minuto inválido.");
        }
        this->minute = minute;
        if (second > 59 || minute < 0) {
            throw std::invalid_argument("Segundo inválido.");
        }
        this->second = second;
        this->compute_weekday();
    }

    Date(time_t remaining_seconds, bool local = true) {
        // Obtém o número de anos desde 1970 e o número de segundos restantes
        if (local) {
            remaining_seconds -= Time::HOUR * 3;
        }
        short value = remaining_seconds / Time::YEAR;
        remaining_seconds %= Time::YEAR;
        // 1972 foi o primeiro ano bissexto depois de 1970, que é o valor 0
        bool is_leap_year = (value + 2) % 4 == 0;
        short leap_year_count = (value + 2) / 4 - (is_leap_year ? 1 : 0);
        // Se não há segundos suficientes para remover os dias de ano bissexto,
        // subtrai um ano do número de anos e adiciona os segundos de um ano
        if (remaining_seconds < leap_year_count * Time::DAY) {
            --value;
            remaining_seconds += Time::YEAR;
        }
        remaining_seconds -= leap_year_count * Time::DAY;
        this->year = value + 1970;
        // Obtém o número de dias do ano, somando 1 para que o dia 1 seja 1
        value = remaining_seconds / Time::DAY + 1;
        remaining_seconds %= Time::DAY;
        // Compara com o número de dias de cada mês para obter o número do mês
        for (short i = 0; i < 12; ++i) {
            if (value <= Time::cumulative_days[i]) {
                // Se o ano é bissexto, subtrai um dia
                if (is_leap_year && i > 1) {
                    --value;
                    // Se o dia era o primeiro do mês antes da subtração,
                    // decrementa o valor do mês
                    if (value == Time::cumulative_days[i - 1]) {
                        --i;
                        // Se o mês é fevereiro, soma um dia e chega em 29
                        if (i == 1) {
                            ++value;
                        }
                    }
                }
                this->month = i + 1;
                value -= i == 0 ? 0 : Time::cumulative_days[i - 1];
                break;
            }
        }
        this->day = value;
        // Obtém o número de horas do dia
        value = remaining_seconds / Time::HOUR;
        remaining_seconds %= Time::HOUR;
        this->hour = value;
        // Obtém o número de minutos da hora
        value = remaining_seconds / Time::MINUTE;
        remaining_seconds %= Time::MINUTE;
        this->minute = value;
        this->second = remaining_seconds;
        // Obtém o dia da semana
        this->compute_weekday();
    }

    void compute_weekday() {
        if (month < 3) --year;
        this->weekday = std::abs(year + year / 4 - year / 100 + \
                                year / 400 + Time::table[month - 1] + day) % 7;
        if (month < 3) ++year;
    }

    bool operator<(const Date& other) {
        if (this->year < other.year) {
            return true;
        } else if (this->year == other.year) {
            if (this->month < other.month) {
                return true;
            } else if (this->month == other.month) {
                if (this->day < other.day) {
                    return true;
                } else if (this->day == other.day) {
                    if (this->hour < other.hour) {
                        return true;
                    } else if (this->hour == other.hour) {
                        if (this->minute < other.minute) {
                            return true;
                        } else if (this->minute == other.minute) {
                            if (this->second < other.second) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    bool operator>(const Date& other) {
        if (this->year > other.year) {
            return true;
        } else if (this->year == other.year) {
            if (this->month > other.month) {
                return true;
            } else if (this->month == other.month) {
                if (this->day > other.day) {
                    return true;
                } else if (this->day == other.day) {
                    if (this->hour > other.hour) {
                        return true;
                    } else if (this->hour == other.hour) {
                        if (this->minute > other.minute) {
                            return true;
                        } else if (this->minute == other.minute) {
                            if (this->second > other.second) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }
};

std::ostream& operator<<(std::ostream& out, const Date& date) {
    out << +date.year << '-' << +date.month << '-' << +date.day << ' ' << \
        +date.hour << ':' << +date.minute << ':' << +date.second << \
        ' ' << Time::week_days[date.weekday];
    return out;
}

#endif  // DATE_HPP_
