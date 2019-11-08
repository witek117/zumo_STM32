#ifndef ZUMO_COMMAND_H
#define ZUMO_COMMAND_H

#include <functional>
#include <utility>
#include <tuple>
#include <array>
#include <iostream>
class Command {
    const char * name;
    size_t  name_len = 0;
public:
    std::function<void(const char*)> callback_handler;

public:
    Command(const char* name, std::function<void(const char*)> callback_handler ) : name(name), name_len(strlen(name)), callback_handler(std::move(callback_handler)) {

    }

    bool parse(const char * data) {
        char buffer [name_len + 1];
        memcpy(buffer, data, name_len);
        buffer[name_len ] = '\0';

        if (strcmp(buffer, name) == 0) {
            callback_handler((char *)data + name_len + 1);
            return true;
        }
        return false;
    }
};

namespace parser {
    template<class T> T parse(std::string_view const s);

    template<> double parse<double>(std::string_view const s) {
        return atof(s.data());
    }

    template<> int parse<int>(std::string_view const s) {
        return atoi(s.data());
    }

    template<> float parse<float>(std::string_view const s) {
        return atof(s.data());
    }

    template<class T0, class... T>  std::tuple<T0, T...>  get(std::string_view const s) {
        const auto spacePos = s.find_first_of(' ', 0);

        auto val =  parse<T0>(s);

        if constexpr (sizeof ...(T)) {
            return std::tuple_cat(std::make_tuple(val), get<T...>(s.substr(spacePos+ 1)));
        } else {
            return std::make_tuple(val);
        }
    }

    template<typename T, int size>
    std::array<T, size> get (const char * data) {
        std::array<T, size> return_data {};

        if constexpr (std::is_same<T, float>() ) {
            char *end;

            for (int i=0; i < size; i++) {
                return_data[i] = strtod(data, &end);
                data = end;
            }
        } else if constexpr (std::is_same<int, T>() || std::is_same<uint32_t, T>()) {
            char *end;

            for (int i=0; i < size; i++) {
                return_data[i] = strtol(data, &end, 10);
                data = end;
            }
        }
        return return_data;
    }

//    template <typename T>
//    T get(const char * data) {
//        if constexpr (std::is_same<T, float>()) {
//            return strtod(data, nullptr);
//        } else if constexpr (std::is_same<T, int>()) {
//            return strtol(data, nullptr, 10);
//        }
//    }
}





#endif //ZUMO_COMMAND_H
