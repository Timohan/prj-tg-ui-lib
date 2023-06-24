#ifndef FUNCTIONAL_TEST_H
#define FUNCTIONAL_TEST_H

#include <stdint.h>
#include <cstddef>
#include <string>
#include "functional_test_orders.h"
class MainWindow;
class TgItem2d;

class FunctionalTest
{
public:
    FunctionalTest();
    void setMainWindow(MainWindow *mainWindow);
    void start();
    int getReturnIndex();

private:
    MainWindow *m_mainWindow;
    int m_returnIndex;
    size_t m_latestHoverIndex { 0 };
    FunctionalTestOrders m_testOrders;

    void sendButtonClick(uint32_t timeBetweenPressRelease, int x, int y, uint32_t waitAfterRelease);
    void sendButtonMoveClick(uint32_t timeBetweenPressRelease, int pressX, int pressY, int releaseX, int releaseY, uint32_t waitAfterRelease, bool sendPress = true, bool sendRelease = true);
    void sendButtonMoveRightClick(uint32_t timeBetweenPressRelease, int pressX, int pressY, int releaseX, int releaseY, uint32_t waitAfterRelease, bool sendPress, bool sendRelease);
    void sendKeyPress(uint32_t key, uint32_t waitAfterRelease, bool keyPress = true, bool keyRelease = true);

    static int moveValueToDirection(int currentValue, int directionValue);
    void changeResolution(uint32_t waitBefore, int width, int height, uint32_t waitAfter);
    bool isCorrectHover(size_t index, bool hover);
    bool isCorrectEnabled(size_t index, bool enabled);
    bool isCorrectVisible(size_t index, bool visible);
    bool isMove(int index, int fromX, int fromY, int toX, int toY, bool inArea);
    bool isMove(int index, int x, int y, bool inArea);
    bool checkIsMousePressed(int index, int x, int y);
    bool checkIsMouseReleased(int index, int x, int y, bool inArea);
    bool checkIsMouseClicked(int index, int x, int y);
    bool checkIsMenuClicked(int index);

};

FunctionalTest *getTest();

#endif
