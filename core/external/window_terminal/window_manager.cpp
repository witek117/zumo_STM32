#include "window_manager.hpp"

namespace window_manager {
    constexpr size_t buff_size = 20;
    CyclicBuffer_data<uint8_t, buff_size> uart_buffer;

    void change_active_window();

    void change_active_box(Keyboard_action action);

    Window *activeWindow = nullptr;
    Window *lastActiveWindow = nullptr;

    Box *activeBox = nullptr;
    Box *lastActiveBox = nullptr;

    uint8_t index = 0;
    uint8_t recBuff[10];

    std::vector<Window> *windows;

    static uint8_t active_box_index = 0;
    static size_t active_window_index = 0;

    void refresh_all() {
        for (size_t i = 0; i < windows->size(); i++) {
            windows->at(i).refresh();
        }
    }

    void init_all() {
        for (size_t i = 0; i < windows->size(); i++) {
            windows->at(i).init();
        }
    }

    void refresh_value(bool refresh_hard = false) {
        for (size_t i = 0; i < windows->size(); i++) {
            windows->at(i).refresh_value(refresh_hard);
        }
    }

    void init(std::vector<Window> *wins) {
        windows = wins;
        init_all();
    }

    void in_RX_callback(uint8_t received_char) { uart_buffer.append(received_char); }

    void read_from_cyclic_Buffer() {
        if (uart_buffer.isNotEmpty()) {
            char received_char = uart_buffer.get_unsafe();
            recBuff[index] = received_char;

            if (received_char == 27 /* && index == 0*/  ) {
                recBuff[0] = received_char;
                index = 1;
            } else if (index == 1) {
                index++;
            } else if (index == 2) {
                index++;
                if (activeWindow) {
                    if (received_char == static_cast<uint8_t>(Keyboard_action::ARROW_UP) ||
                        received_char == static_cast<uint8_t>(Keyboard_action::ARROW_DOWN)) {

                        lastActiveBox = activeBox;

                        change_active_box(static_cast<Keyboard_action>(received_char));

                        if (lastActiveBox) {
                            lastActiveBox->set_unactive();
                        }

                        if (activeBox) {
                            activeBox->set_active();
                        }
                    } else if (activeBox) {
                        activeBox->keyboard_event(recBuff, index);
                    }
                }
                index = 0;
            } else {
                if (received_char == static_cast<uint8_t>(Keyboard_action::TAB)) {
                    if (activeBox) {
                        activeBox->set_unactive(true); // tu chyba było bez true
                    }

                    lastActiveWindow = activeWindow;

                    change_active_window();

                    if (lastActiveWindow) {
                        lastActiveWindow->set_unactive();
                    }

                    if (activeWindow) {
                        activeWindow->set_active();
                    }

                } else if (received_char == static_cast<uint8_t>(Keyboard_action::QUIT)) {
                    active_window_index = 0;
                    if (activeBox) {
                        activeBox->set_unactive(true);
                    } else {
                        if (activeWindow) {
                            activeWindow->set_unactive(true);
                        }
                    }
                } else {
                    if (activeBox) {
                        activeBox->keyboard_event(recBuff, 1);
                    }
                }
                index = 0;
            }
        } else {
            uart_buffer.flush();
        }
    }

    void change_active_window() {
        active_box_index = 0;
        size_t windows_size = windows->size();
        uint8_t window_index = 0;
        uint8_t offset = 1;
        if (!activeWindow) {
            offset = 0;
        }

        for (size_t i = active_window_index + offset; i < (windows_size + active_window_index); i++){
            window_index = i % windows_size;
            if (windows->at(window_index).get_editable()) {
                activeWindow = &(windows->at(window_index));
                active_window_index = window_index;
                return;
            }
        }
        activeWindow = nullptr;
    }

    void change_active_box(Keyboard_action action) {
        uint8_t offset = 1;
        if (!activeBox) {
            offset = 0;
        }
        size_t bsize = activeWindow->boxes.size();
        uint8_t box_index = 0;

        if (action == Keyboard_action::ARROW_DOWN){
            for (size_t i = active_box_index + offset; i < (bsize + active_box_index); i++){
                box_index = i % bsize;
                if (activeWindow->boxes[box_index]->get_editable()) {
                    activeBox = activeWindow->boxes[box_index];
                    active_box_index = box_index;
                    return;
                }
            }
        } else if (action == Keyboard_action::ARROW_UP) {
            for (size_t i = bsize + active_box_index - 1; i > active_box_index; i--){
                box_index = i % bsize;
                if (activeWindow->boxes[box_index]->get_editable()) {
                    activeBox = activeWindow->boxes[box_index];
                    active_box_index = box_index;
                    return;
                }
            }
        }
        activeBox = nullptr;
    }

    void run() {
        read_from_cyclic_Buffer();
        if (activeWindow) {
            if (activeWindow->in_loop()) {
                activeWindow = nullptr;
            }
        }

        if (lastActiveWindow) {
            lastActiveWindow->in_loop();
            lastActiveWindow = nullptr;
        }

        if (activeBox) {
            if (activeBox->in_loop()) {
                activeBox = nullptr;
            }
        }

        if (lastActiveBox) {
            lastActiveBox->in_loop();
            lastActiveBox = nullptr;
        }

        refresh_value();
    }

} // namespace window_manager