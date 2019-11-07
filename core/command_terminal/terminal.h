#pragma once

#include <gsl/span>
#include <cstring>
#include <functional>

namespace terminal {

void init ();


template<class...> constexpr std::false_type always_false{};

template<int index, typename T>
struct Fill {
    static void fill(T& target, char* arg) {
        if constexpr(std::is_same_v<T, int>) {
            target = atoi(arg);
        } else if constexpr(std::is_same_v<T, uint32_t>) {
            target = atol(arg);
        } else if constexpr(std::is_same_v<T, float>) {
            target = atof(arg);
        } else if constexpr(std::is_same_v<T, char*> || std::is_same_v<T, const char*>) {
            target = arg;
        } else {
            static_assert(always_false<T>);
        }
    }
};

template<int index, int index_max, typename T>
void iter(gsl::span<char*> args, T& target) {
    if constexpr (index < index_max) {
        Fill<index, std::tuple_element_t<index, T>>::fill(std::get<index>(target), args[index]);

        iter<index+1, index_max, T>(args, target);
    } else {
        (void)args;
    }
}

template<typename... T>
struct Command {
    Command(const char* name) : name(name) {}

    virtual void callback(T... args) = 0;

    void callbackDispatcher(const gsl::span<char*> parameters) {
        std::tuple<T...> t;
        iter<0, requiredArguments>(parameters, t);

        std::apply([&](T... t){this->callback(t...);}, t);
    }

    const char* name;
    static constexpr uint8_t requiredArguments = sizeof...(T);
};

template<typename... T>
class Terminal {
    std::function<void(char)> print_char_fun = nullptr;
public:
    Terminal(T&... cmds ) : commands(std::tuple<T&...>(cmds...)){ }

    bool init (std::function<void(char)> fun) {
        if (fun && !print_char_fun) {
            print_char_fun = fun;
            return true;
        }
        return false;
    }

    void send (char c) {
        if (print_char_fun) {
            print_char_fun(c);
        }
    }

    bool is_enabled() {
        return (print_char_fun != nullptr);
    }

    bool parse_line(char *line) {
        size_t len = std::strlen(line);

        for (size_t i = 0; i < len; ++i) {
            if (std::isspace(line[i])) {
                line[i] = '\0';
            }
        }
        char* iter = line;
        std::array<char*, 20> params;
        uint8_t param_nr = 0;

        while (iter < line + len) {
            while (iter < line + len && *iter != '\0')
                iter++;

            while (iter < line + len && *iter == '\0')
                iter++;

            if (iter < line + len && param_nr < params.size()) {
                params[param_nr++] = iter;
            }
        }
        gsl::span<char*> view(params.data(), param_nr);


        bool any = false;

        auto foo = [&](auto& cmd) {
            if (!any && cmd.requiredArguments == param_nr && strcmp(cmd.name, line) == 0) {
                // found command
                cmd.callbackDispatcher(view);
                any = true;
            }
        };

        std::apply([&](auto&&... args) {(foo(args), ...);}, commands);

        return any;
    }

private:
    std::tuple<T&...> commands;
};

}  // namespace command_terminal