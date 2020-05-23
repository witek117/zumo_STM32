#include "command_manager.h"

ReadManager* readManager = nullptr;
PrintManager* printManager = nullptr;
void ReadManager::init() {
    readManager = this;
}
void ReadManager::putChar(uint8_t c) {
    readManager->putToBuffer(c);
}

bool ReadManager::isEnabled() {
    return readManager != nullptr;
}

void PrintManager::init() {
    printManager = this;
}

void PrintManager::setPrintFunction(void(*printHandler)(uint8_t)) {
    printManager->printByte = printHandler;
}