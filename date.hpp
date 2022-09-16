#ifndef DATE_HPP_
#define DATE_HPP_

#include <iostream>

typedef unsigned long long int Time_t;
typedef unsigned char uint8;

namespace Time {
    const unsigned int SECOND = 1;
    const unsigned int MINUTE = 60 * SECOND;
    const unsigned int HOUR = 60 * MINUTE;
    const unsigned int DAY = 24 * HOUR;
    const unsigned int MONTH = 30 * DAY;
    const unsigned int YEAR = 365 * DAY;
    const unsigned int cumulative_days[12] = {
        31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    const char* week_days[7] = {"Sunday", "Monday", "Tuesday", "Wednesday",
                                "Thursday", "Friday", "Saturday"};
}

// Apenas para datas de 1970 a 2099
class Date {
 private:
    Time_t seconds;

 public:
    class FieldDate {
     public:
        short year;
        uint8 month;
        uint8 day;
        uint8 hour;
        uint8 minute;
        uint8 second;
        uint8 weekday;

        FieldDate() {
            this->year = 0;
            this->month = 0;
            this->day = 0;
            this->hour = 0;
            this->minute = 0;
            this->second = 0;
            this->weekday = 0;
        }

        FieldDate(short year, uint8 month, uint8 day) {
            this->year = year;
            if (month > 12 || month < 1) {
                throw std::invalid_argument("Mês inválido.");
            }
            this->month = month;
            if (day > 31 || day < 1) {
                throw std::invalid_argument("Dia inválido.");
            }
            this->day = day;
            this->hour = 0;
            this->minute = 0;
            this->second = 0;
            this->compute_weekday();
        }

        FieldDate(short year, uint8 month, uint8 day,
        uint8 hour, uint8 minute, uint8 second) {
            this->year = year;
            if (month > 12 || month < 1) {
                throw std::invalid_argument("Mês inválido.");
            }
            this->month = month;
            if (day > 31 || day < 1) {
                throw std::invalid_argument("Dia inválido.");
            }
            this->day = day;
            if (hour > 23) {
                throw std::invalid_argument("Hora inválida.");
            }
            this->hour = hour;
            if (minute > 59) {
                throw std::invalid_argument("Minuto inválido.");
            }
            this->minute = minute;
            if (second > 59) {
                throw std::invalid_argument("Segundo inválido.");
            }
            this->second = second;
            this->compute_weekday();
        }
        
        FieldDate(const Date& date, bool local = true) {
            // Obtém o número de anos desde 1970 e o número de segundos restantes
            Time_t remaining_seconds = date.get_absolute_seconds();
            if (!local) {
                remaining_seconds += Time::HOUR * 3;
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
            int century = (year - (int)(month < 3)) / 100;
            int decade = (year - (int)(month < 3)) % 100;
            this->weekday = std::abs(century / 4 - 2 * century - 1 + decade * 5 / 4 + \
                            (month + 1) * 26 / 10 + day) % 7;
        }
    };

    Date() {
        this->seconds = 0;
    }

    Date(time_t seconds) {
        // Horário padrão de Brasília
        this->seconds = seconds - 3 * Time::HOUR;
    }

    Date(const Date::FieldDate& date) {
        if (date.year < 1970) {
            throw std::invalid_argument("Invalid year.");
        }
        this->seconds = (date.year - 1970) * Time::YEAR;
        // Adiciona os dias de anos bissextos
        this->seconds += (date.year - 1972) / 4 * Time::DAY;
        if (date.month > 2 && (date.year - 1972) % 4 == 0) {
            this->seconds += Time::DAY;
        }
        // Adiciona os dias do ano
        this->seconds += date.month > 1 ? (Time::cumulative_days[date.month - 2] * Time::DAY) : 0;
        // Adiciona os valores restantes
        this->seconds += date.day * Time::DAY;
        this->seconds += date.hour * Time::HOUR;
        this->seconds += date.minute * Time::MINUTE;
        this->seconds += date.second;
    }

    Time_t get_absolute_seconds(bool local = true) const {
        if (local) {
            return this->seconds;
        }
        return this->seconds + Time::HOUR * 3;
    }

    bool operator<(const Date& other) const {
        return this->seconds < other.seconds;
    }

    bool operator>(const Date& other) const {
        return other < *this;
    }

    bool operator==(const Date& other) const {
        return this->seconds == other.seconds;
    }
};

std::ostream& operator<<(std::ostream& os, const Date::FieldDate& date) {
    os << date.year << \
    (date.month < 10 ? "-0" : "-") << +date.month << \
    (date.day < 10 ? "-0" : "-") << +date.day << \
    (date.hour < 10 ? " 0" : " ") << +date.hour << \
    (date.minute < 10 ? ":0" : ":") << +date.minute << \
    (date.second < 10 ? ":0" : ":") << +date.second << \
    " " << Time::week_days[date.weekday];
    return os;
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << Date::FieldDate(date);
    return os;
}

#endif  // DATE_HPP_
