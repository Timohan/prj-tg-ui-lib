#ifndef FUNCTIONAL_TEST_H
#define FUNCTIONAL_TEST_H

#include <stdint.h>
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
    void sendButtonClick(uint32_t timeBetweenPressRelease, int x, int y, uint32_t waitAfterRelease);
    void sendButtonMoveClick(uint32_t timeBetweenPressRelease, int pressX, int pressY, int releaseX, int releaseY, uint32_t waitAfterRelease, bool sendPress = true, bool sendRelease = true);
    void sendKeyPress(uint32_t key, uint32_t waitAfterRelease, bool keyPress = true, bool keyRelease = true);
    bool isCorrectButtonSelected(int page, int index);
    bool isItemRightSize(TgItem2d *item, int w, int h);

    static int moveValueToDirection(int currentValue, int directionValue);
    void changeResolution(uint32_t waitBefore, int width, int height, uint32_t waitAfter);
    bool makeCharacterCountTest();
    bool makeTextFieldTest();
    bool makeEditTextTest();
};

FunctionalTest *getTest();

#endif
