#include "command_manager.h"

void PrintManager::init() {
    staticManager = this;
}

void PrintManager::printManagerPutChar(uint8_t c) {
    staticManager->put_char(c);
}

void PrintManager::setPrintFunction(void(*printHandler)(uint8_t)) {
    staticManager->printByte = printHandler;
}
