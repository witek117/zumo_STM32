#ifndef ZUMO_COMMAND_H
#define ZUMO_COMMAND_H

#include <functional>
#include <utility>
#include <tuple>
#include <array>
#include <cstring>
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

    template<> double parse<double>(std::string_view const s);

    template<> int parse<int>(std::string_view const s);

    template<> float parse<float>(std::string_view const s);

    template<class T0, class... T>  std::tuple<T0, T...>  get(std::string_view const s);

    template<typename T, int size>
    std::array<T, size> get (const char * data);
}





#endif //ZUMO_COMMAND_H
