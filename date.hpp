#ifndef DATE_HPP_
#define DATE_HPP_

#include <iostream>

typedef unsigned char uint8;

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
}

// Apenas para datas de 1970 a 2099
class Date {
 public:
    char date[16];

    Date() {
        // for (int i = 0; i < 16; i++) {
        //     this->date[i] = 0;
        // }
        this->date[0] = 0;
    }

    Date(const Date& other) {
        if (other.date[0] == 0) {
            this->date[0] = 0;
        } else {
            std::copy(other.date, other.date + 16, this->date);
        }
    }

    Date(short year, uint8 month, uint8 day,
    uint8 hour = 0, uint8 minute = 0, uint8 second = 0) {
        if (month > 12 || month < 1) {
            throw std::invalid_argument("Mês inválido.");
        }
        if (day > 31 || day < 1) {
            throw std::invalid_argument("Dia inválido.");
        }
        if (hour > 23) {
            throw std::invalid_argument("Hora inválida.");
        }
        if (minute > 59) {
            throw std::invalid_argument("Minuto inválido.");
        }
        if (second > 59) {
            throw std::invalid_argument("Segundo inválido.");
        }
        sprintf(this->date, "%04u%02u%02u%02u%02u%02u%02u",
            year, month, day, hour, minute, second,
            this->compute_weekday(year, month, day));
    }

    Date(time_t remaining_seconds, bool local = true) {
        if (local) {
            remaining_seconds -= Time::HOUR * 3;
        }
        short year = remaining_seconds / Time::YEAR;
        remaining_seconds %= Time::YEAR;
        // 1972 foi o primeiro ano bissexto depois de 1970, que é o valor 0
        bool is_leap_year = (year + 2) % 4 == 0;
        short leap_year_count = (year + 2) / 4 - (is_leap_year ? 1 : 0);
        // Se não há segundos suficientes para remover os dias de ano bissexto,
        // subtrai um ano do número de anos e adiciona os segundos de um ano
        if (remaining_seconds < leap_year_count * Time::DAY) {
            --year;
            remaining_seconds += Time::YEAR;
        }
        remaining_seconds -= leap_year_count * Time::DAY;
        year += 1970;
        // Obtém o número de dias do ano, somando 1 para que o dia 1 seja 1
        uint8 month;
        short day = (remaining_seconds / Time::DAY) + 1;
        remaining_seconds %= Time::DAY;
        // Compara com o número de dias de cada mês para obter o número do mês
        for (short i = 0; i < 12; ++i) {
            if (day <= Time::cumulative_days[i]) {
                // Se o ano é bissexto, subtrai um dia
                if (is_leap_year && i > 1) {
                    --day;
                    // Se o dia era o primeiro do mês antes da subtração,
                    // decrementa o valor do mês
                    if (day == Time::cumulative_days[i - 1]) {
                        --i;
                        // Se o mês é fevereiro, soma um dia e chega em 29
                        if (i == 1) {
                            ++day;
                        }
                    }
                }
                month = i + 1;
                day -= i == 0 ? 0 : Time::cumulative_days[i - 1];
                break;
            }
        }
        // Obtém o número de horas do dia
        uint8 hour = remaining_seconds / Time::HOUR;
        remaining_seconds %= Time::HOUR;
        // Obtém o número de minutos da hora
        uint8 minute = remaining_seconds / Time::MINUTE;
        uint8 second = remaining_seconds % Time::MINUTE;
        // Obtém o dia da semana
        sprintf(this->date, "%04u%02u%02u%02u%02u%02u%02u",
            year, month, day, hour, minute, second,
            this->compute_weekday(year, month, day));
    }

    uint8 compute_weekday(short year, uint8 month, uint8 day) {
        uint8 weekday;
        if (month < 3) --year;
            weekday = std::abs((int)(year + year / 4 - year / 100 + year / 400 + \
                            Time::table[month - 1] + day)) % 7;
            if (month < 3) ++year;
        return weekday;
    }
};

// class Date {
//  private:
//     Time_t seconds;

//  public:
    // class FieldDate {
    //  public:
    //     short year;
    //     uint8 month;
    //     uint8 day;
    //     uint8 hour;
    //     uint8 minute;
    //     uint8 second;
    //     uint8 weekday;

    //     FieldDate() {
    //         this->year = 0;
    //         this->month = 0;
    //         this->day = 0;
    //         this->hour = 0;
    //         this->minute = 0;
    //         this->second = 0;
    //         this->weekday = 0;
    //     }

    //     FieldDate(short year, uint8 month, uint8 day,
    //     uint8 hour = 0, uint8 minute = 0, uint8 second = 0) {
    //         this->year = year;
    //         if (month > 12 || month < 1) {
    //             throw std::invalid_argument("Mês inválido.");
    //         }
    //         this->month = month;
    //         if (day > 31 || day < 1) {
    //             throw std::invalid_argument("Dia inválido.");
    //         }
    //         this->day = day;
    //         if (hour > 23) {
    //             throw std::invalid_argument("Hora inválida.");
    //         }
    //         this->hour = hour;
    //         if (minute > 59) {
    //             throw std::invalid_argument("Minuto inválido.");
    //         }
    //         this->minute = minute;
    //         if (second > 59) {
    //             throw std::invalid_argument("Segundo inválido.");
    //         }
    //         this->second = second;
    //         this->compute_weekday();
    //     }
        
    //     FieldDate(const Date& date, bool local = true) {
    //         // Obtém o número de anos desde 1970 e o número de segundos restantes
    //         Time_t remaining_seconds = date.get_absolute_seconds();
    //         if (!local) {
    //             remaining_seconds += Time::HOUR * 3;
    //         }
    //         short value = remaining_seconds / Time::YEAR;
    //         remaining_seconds %= Time::YEAR;
    //         // 1972 foi o primeiro ano bissexto depois de 1970, que é o valor 0
    //         bool is_leap_year = (value + 2) % 4 == 0;
    //         short leap_year_count = (value + 2) / 4 - (is_leap_year ? 1 : 0);
    //         // Se não há segundos suficientes para remover os dias de ano bissexto,
    //         // subtrai um ano do número de anos e adiciona os segundos de um ano
    //         if (remaining_seconds < leap_year_count * Time::DAY) {
    //             --value;
    //             remaining_seconds += Time::YEAR;
    //         }
    //         remaining_seconds -= leap_year_count * Time::DAY;
    //         this->year = value + 1970;
    //         // Obtém o número de dias do ano, somando 1 para que o dia 1 seja 1
    //         value = remaining_seconds / Time::DAY + 1;
    //         remaining_seconds %= Time::DAY;
    //         // Compara com o número de dias de cada mês para obter o número do mês
    //         for (short i = 0; i < 12; ++i) {
    //             if (value <= Time::cumulative_days[i]) {
    //                 // Se o ano é bissexto, subtrai um dia
    //                 if (is_leap_year && i > 1) {
    //                     --value;
    //                     // Se o dia era o primeiro do mês antes da subtração,
    //                     // decrementa o valor do mês
    //                     if (value == Time::cumulative_days[i - 1]) {
    //                         --i;
    //                         // Se o mês é fevereiro, soma um dia e chega em 29
    //                         if (i == 1) {
    //                             ++value;
    //                         }
    //                     }
    //                 }
    //                 this->month = i + 1;
    //                 value -= i == 0 ? 0 : Time::cumulative_days[i - 1];
    //                 break;
    //             }
    //         }
    //         this->day = value;
    //         // Obtém o número de horas do dia
    //         value = remaining_seconds / Time::HOUR;
    //         remaining_seconds %= Time::HOUR;
    //         this->hour = value;
    //         // Obtém o número de minutos da hora
    //         value = remaining_seconds / Time::MINUTE;
    //         remaining_seconds %= Time::MINUTE;
    //         this->minute = value;
    //         this->second = remaining_seconds;
    //         // Obtém o dia da semana
    //         this->compute_weekday();
    //     }

    //     void compute_weekday() {
    //         if (month < 3) --year;
    //         this->weekday = std::abs((int)(year + year / 4 - year / 100 + year / 400 + \
    //                         Time::table[month - 1] + day)) % 7;
    //         if (month < 3) ++year;
    //         // if (month < 3) {
    //         //     --year;
    //         //     this->weekday = std::abs(23 * month / 9 + day + year + 5 + \
    //         //         year / 4 - year / 100 + year / 400) % 7;
    //         //     ++year;
    //         // } else {
    //         //     this->weekday = std::abs(23 * month / 9 + day + year + 2 + \
    //         //         year / 4 - year / 100 + year / 400) % 7;
    //         // }
    //     }

    //     bool operator<(const Date::FieldDate& other) {
    //         if (this->year < other.year) {
    //             return true;
    //         } else if (this->year == other.year) {
    //             if (this->month < other.month) {
    //                 return true;
    //             } else if (this->month == other.month) {
    //                 if (this->day < other.day) {
    //                     return true;
    //                 } else if (this->day == other.day) {
    //                     if (this->hour < other.hour) {
    //                         return true;
    //                     } else if (this->hour == other.hour) {
    //                         if (this->minute < other.minute) {
    //                             return true;
    //                         } else if (this->minute == other.minute) {
    //                             if (this->second < other.second) {
    //                                 return true;
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //         return false;
    //     }

    //     bool operator>(const Date::FieldDate& other) {
    //         if (this->year > other.year) {
    //             return true;
    //         } else if (this->year == other.year) {
    //             if (this->month > other.month) {
    //                 return true;
    //             } else if (this->month == other.month) {
    //                 if (this->day > other.day) {
    //                     return true;
    //                 } else if (this->day == other.day) {
    //                     if (this->hour > other.hour) {
    //                         return true;
    //                     } else if (this->hour == other.hour) {
    //                         if (this->minute > other.minute) {
    //                             return true;
    //                         } else if (this->minute == other.minute) {
    //                             if (this->second > other.second) {
    //                                 return true;
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //         return false;
    //     }
    // };

//     Date() {
//         this->seconds = 0;
//     }

//     Date(time_t seconds) {
//         // Horário padrão de Brasília
//         this->seconds = seconds - 3 * Time::HOUR;
//     }

//     Time_t get_absolute_seconds(bool local = true) const {
//         if (local) {
//             return this->seconds;
//         }
//         return this->seconds + Time::HOUR * 3;
//     }

//     bool operator<(const Date& other) const {
//         return this->seconds < other.seconds;
//     }

//     bool operator>(const Date& other) const {
//         return other < *this;
//     }

//     bool operator==(const Date& other) const {
//         return this->seconds == other.seconds;
//     }
// };

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << date.date[0] << date.date[1] << date.date[2] << date.date[3] << '-' << \
        date.date[4] << date.date[5] << '-' << date.date[6] << date.date[7] << ' ' << \
        date.date[8] << date.date[9] << ':' << date.date[10] << date.date[11] << ':' << \
        date.date[12] << date.date[13] << " " << Time::week_days[date.date[15] - 48];
    return os;
}

#endif  // DATE_HPP_
