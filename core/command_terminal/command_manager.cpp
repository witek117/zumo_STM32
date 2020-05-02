#include "command_manager.h"

void PrintManager::initt() {
    staticManager = this;
    staticManager->print('c');
}

void PrintManager::printManagerPutChar(uint8_t c) {
    (void) c;
//    staticManager->put_char(c);

}

void PrintManager::setPrintFunction(void(*printHandler)(uint8_t)) {
    staticManager->printByte = printHandler;
}
