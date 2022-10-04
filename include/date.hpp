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

    Date() {}

    Date(short year, int8 month, int8 day,
    int8 hour = 0, int8 minute = 0, int8 second = 0, int8 weekday = -1) {
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
        if (weekday < 0) {
            this->compute_weekday();
        } else {
            this->weekday = weekday;
        }
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

    // Usa algum algoritmo sem nome para calcular o dia da semana
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

    bool operator<=(const Date& other) {
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
                            if (this->second <= other.second) {
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

    bool operator>=(const Date& other) {
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
                            if (this->second >= other.second) {
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

inline void show_number(std::ostream& out, int8 number) {
    if (number < 10) {
        out << '0';
    }
    out << +number;
}

inline void show_number(std::ostream& out, short number) {
    if (number < 10) {
        out << '0';
    }
    out << number;
}

std::ostream& operator<<(std::ostream& out, const Date& date) {
    // out << << '-' << +date.month << '-' << +date.day << ' ' << \
    //     +date.hour << ':' << +date.minute << ':' << +date.second << \
    //     ' ' << Time::week_days[date.weekday];
    show_number(out, date.year);
    out << '-';
    show_number(out, date.month);
    out << '-';
    show_number(out, date.day);
    out << ' ';
    show_number(out, date.hour);
    out << ':';
    show_number(out, date.minute);
    out << ':';
    show_number(out, date.second);
    out << ' ' << Time::week_days[date.weekday];
    return out;
}

#endif  // DATE_HPP_
