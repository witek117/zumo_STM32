#pragma once

#include "cyclicBuffer.hpp"
#include <functional>
#include <utility>
#include "Command.h"

constexpr char endChar = '\n';

class PrintManager {
    void (*printByte)(uint8_t) = nullptr;
public:
    char buff[10] = {0};

    size_t print(const char *s) {
        size_t size = 0;
        char c;
        while ((c = *s) != '\0') {
            print(c);
            s++;
            size++;
        }
        return size;
    }

    void print(uint16_t value) {
        sprintf(buff, "%d", value);
        print(buff);
    }

    void print(uint32_t value) {
        sprintf(buff, "%lu", value);
        print(buff);
    }

    void print(int16_t value) {
        sprintf(buff, "%d", value);
        print(buff);
    }

    void print(float value) {
        sprintf(buff, "%.2f", value);
        print(buff);
    }

    void print(char c) {
        if (printByte) {
            printByte(c);
        }
    }

    void init();

    static void setPrintFunction(void(*printHandler)(uint8_t));
};

class ReadManager {
public:
    constexpr static size_t buff_size = 50;
    CyclicBuffer_data<char, buff_size> buffer_rx;
    uint8_t commands_in_buffer = 0;

    void putToBuffer(char c) {
        buffer_rx.append(c);
        if (c == endChar) {
            commands_in_buffer++;
        }
    }
    void init();
    static bool isEnabled();
    static void putChar(uint8_t);
};

template <int size>
class CommandManager {
    ReadManager read;
public:
    PrintManager printer;
private:
    void(*enable_interrupts)() = nullptr;
    void(*disable_interrupts)() = nullptr;
    std::array<Command, size> commands;
    uint8_t command_title_len = 0;
public:
    explicit CommandManager(void(*enable_interrupts)(), void(*disable_interrupts)(),  std::array<Command, size> commands) :
        enable_interrupts(enable_interrupts), disable_interrupts(disable_interrupts), commands(commands) {
    }

    void init() {
        read.init();
        printer.init();
    }

    bool run() {
        if (!read.isEnabled()) {
            return false;
        }

        disable_interrupts();
        uint8_t commands_in_fifo_local = read.commands_in_buffer;
        read.commands_in_buffer = 0;
        enable_interrupts();

        while (commands_in_fifo_local--) {
            command_title_len = 0;
            char* cmd_buffer = copy_from_fifo_to_buffer();
            parse(cmd_buffer);
        }
        return true;
    }

    void parse(char* data) {
        for (uint8_t i = 0; i < 20; i ++) {
            if (data[i] == ' ' || data[i] == '\0') {
                data[i] = '\0';
                command_title_len = i;
                break;
            }
        }
        for (auto command : commands) {
            if (command.parse(data, command_title_len)) {
                return;
            }
            printer.print("undefined\n");
        }
    }

    char *copy_from_fifo_to_buffer() {
        static std::array<char, 100> cmd_buffer;
        auto it = cmd_buffer.begin();
        disable_interrupts();
        while (read.buffer_rx.getSize() != 0 && it != cmd_buffer.end()) {
            *it = read.buffer_rx.get();
            if(*it == endChar) {
                *it = '\0';
                break;
            }
            it++;
        }
        enable_interrupts();
        return cmd_buffer.data();
    }
};

