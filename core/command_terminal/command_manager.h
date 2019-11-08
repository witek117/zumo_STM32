#ifndef ZUMO_COMMAND_MANAGER_H
#define ZUMO_COMMAND_MANAGER_H

#include "cyclicBuffer.hpp"
#include <functional>
#include <utility>
#include "Command.h"

extern volatile bool print_flag;

template <int size, char end_char, bool echo>
class CommandManager {

    constexpr static size_t buff_size = 50;
    CyclicBuffer_data<char, buff_size> buffer_rx;
    CyclicBuffer_data<char, buff_size> buffer_tx;
    uint8_t commands_in_buffer = 0;

    std::function<void(void)> enable_interrupts = nullptr;
    std::function<void(void)> disable_interrupts = nullptr;

    std::array<Command, size> commands;

    std::function<void(char)> print_handler = nullptr;

public:
    explicit CommandManager(std::function<void(void)> enable_interrupts, std::function<void(void)> disable_interrupts,  std::array<Command, size> commands) :
        enable_interrupts(std::move(enable_interrupts)), disable_interrupts(std::move(disable_interrupts)), commands(commands), print_handler(nullptr) {
    }

    bool init(std::function<void(char)> print_handler_) {
        if (print_handler == nullptr) {
            print_handler = print_handler_;
            return true;
        }
        return false;
    }

    void deinit() {
        print_handler = nullptr;
    }

    void print(char c) {
        buffer_tx.append(c);
    }

    bool is_enabled() {
        return (print_handler != nullptr);
    }

    void put_char(char c) {
        if constexpr (echo) {
            print(c);
        }

        buffer_rx.append(c);
        if (c == end_char) {
            commands_in_buffer++;
        }
    }

    void run() {
        if (!is_enabled()) {
            return;
        }

        if (!buffer_tx.isEmpty()) {
            if (print_handler) {
                print_handler(buffer_tx.get());
            }
        }

        disable_interrupts();
        uint8_t commands_in_fifo_local = commands_in_buffer;
        commands_in_buffer = 0;
        enable_interrupts();

        while (commands_in_fifo_local--) {
            char* cmd_buffer = copy_from_fifo_to_buffer();
            parse(cmd_buffer);

        }
    }

    void parse(const char * data) {
        for (auto command : commands) {
            command.parse(data);
        }
    }

    char *copy_from_fifo_to_buffer() {
        static std::array<char, 100> cmd_buffer;

        auto it = cmd_buffer.begin();

        disable_interrupts();
        while (buffer_rx.getSize() != 0 && it != cmd_buffer.end()) {
            *it = buffer_rx.get();
            if(*it == end_char) {
                *it = '\0';
                break;
            }
            it++;
        }
        enable_interrupts();
        return cmd_buffer.data();
    }


};


#endif //ZUMO_COMMAND_MANAGER_H
