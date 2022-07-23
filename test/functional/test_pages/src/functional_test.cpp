#include "functional_test.h"
#include <thread>
#include <unistd.h>
#include "../../../../lib/src/global/tg_global_log.h"
#include <X11/Xlib.h>
#include <math.h>
#include <X11/Xutil.h>
#include <string.h>
#include "mainwindow.h"
#include "functional_test_image.h"

static FunctionalTest m_test;

FunctionalTest *getTest()
{
    return &m_test;
}

FunctionalTest::FunctionalTest() :
    m_returnIndex(0)
{

}

void FunctionalTest::setMainWindow(MainWindow *mainWindow)
{
    m_mainWindow = mainWindow;
}

int FunctionalTest::getReturnIndex()
{
    return m_returnIndex;
}

void FunctionalTest::start()
{
    std::thread([this]() {
        int i;
        sleep(2);
        if (m_mainWindow->getOpeningTextWidth() <= 0) {
            TG_ERROR_LOG("Opening text width for page0 - test - is wrong ", m_mainWindow->getOpeningTextWidth());
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }

        if (static_cast<int>(m_mainWindow->getOpeningTextWidth()) != static_cast<int>(m_mainWindow->getTextfield()->getTextWidth()) ) {
            TG_ERROR_LOG("Opening text width for page0 - test - is no more same ",
                m_mainWindow->getOpeningTextWidth(), " ",
                static_cast<int>(m_mainWindow->getTextfield()->getTextWidth()));
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }

        if (m_mainWindow->getOpeningTextHeight() <= 0) {
            TG_ERROR_LOG("Opening text height for page0 - test - is wrong ", m_mainWindow->getOpeningTextHeight());
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }

        if (static_cast<int>(m_mainWindow->getOpeningTextHeight()) != static_cast<int>(m_mainWindow->getTextfield()->getTextHeight()) ) {
            TG_ERROR_LOG("Opening text height for page0 - test - is no more same ",
                m_mainWindow->getOpeningTextHeight(), " ",
                static_cast<int>(m_mainWindow->getTextfield()->getTextHeight()));
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }

        if (!makeEditTextTest()) {
            sleep(1);
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }

        if (!FunctionalTestImage::isImageToEqual(m_mainWindow, "image0.png", 800, 600)) {
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }

        if (!makeTextFieldTest()) {
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }

        sendButtonClick(1, 155, 69, 1);

        sendButtonClick(1, 155, 69, 1);
        if (m_mainWindow->getPages()->getCurrentPageIndex() != 1) {
            TG_ERROR_LOG("Page index is not 1 it is: ", m_mainWindow->getPages()->getCurrentPageIndex());
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }
        sendButtonClick(1, 155, 69, 1);
        if (m_mainWindow->getPages()->getCurrentPageIndex() != 0) {
            TG_ERROR_LOG("Page index is not 0 it is: ", m_mainWindow->getPages()->getCurrentPageIndex());
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }
        for (i=0;i<11;i++) {
            sendKeyPress('\t', 1000);
            if (!isCorrectButtonSelected(0, i % 4)) {
                TG_ERROR_LOG("Incorrect button selected, tab index: ", i);
                m_returnIndex = 1;
                m_mainWindow->exit();
                return;
            }
        }
        sendButtonMoveClick(1, 155, 69, 155, 200, 1);
        if (!isCorrectButtonSelected(0, 0)) {
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }
        sendButtonMoveClick(1, 355, 69, 155, 200, 1);
        if (!isCorrectButtonSelected(0, 1)) {
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }
        if (!isItemRightSize(m_mainWindow, 800, 600)) {
            TG_ERROR_LOG("Mainwindow have a wrong size");
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }
        if (!isItemRightSize(m_mainWindow->getTextfield(), 785, 590)) {
            TG_ERROR_LOG("Textfield have a wrong size");
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }
        changeResolution(1, 700, 500, 1);
        if (!isItemRightSize(m_mainWindow, 700, 500)) {
            TG_ERROR_LOG("Mainwindow have a wrong size");
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }
        if (!isItemRightSize(m_mainWindow->getTextfield(), 685, 490)) {
            TG_ERROR_LOG("Textfield have a wrong size");
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }
        if (!makeCharacterCountTest()) {
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }

        TG_INFO_LOG("All tests ok");
        sleep(1);
        m_mainWindow->exit();
    }).detach();
}

bool FunctionalTest::isItemRightSize(TgItem2d *item, int w, int h)
{
    if (static_cast<int>(item->getWidth()) == w && static_cast<int>(item->getHeight()) == h) {
        return true;
    }
    TG_ERROR_LOG("Error: Item have a wrong size, should be: " + std::to_string(w) + "/" + std::to_string(h) + " is: " + std::to_string(item->getWidth()) + "/" + std::to_string(item->getHeight()));
    return false;
}

void FunctionalTest::changeResolution(uint32_t waitBefore, int width, int height, uint32_t waitAfter)
{
    sleep(waitBefore);
    XMoveResizeWindow(m_mainWindow->getDisplay(), *m_mainWindow->getWindow(), 0, 0, width, height);
    sleep(waitAfter);
}

void FunctionalTest::sendButtonClick(uint32_t timeBetweenPressRelease, int x, int y, uint32_t waitAfterRelease)
{
    int button = Button1;
    Display *display = m_mainWindow->getDisplay();

    XEvent event;
    memset(&event, 0x00, sizeof(event));
    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;
    event.xbutton.x = x;
    event.xbutton.y = y;
    XSendEvent(display, *m_mainWindow->getWindow(), True, 0xfff, &event);
    XFlush(display);
    sleep(timeBetweenPressRelease);
    event.type = ButtonRelease;
    event.xbutton.state = 0x100;
    XSendEvent(display, *m_mainWindow->getWindow(), True, 0xfff, &event);
    XFlush(display);
    sleep(waitAfterRelease);
}

void FunctionalTest::sendButtonMoveClick(uint32_t timeBetweenPressRelease, int pressX, int pressY, int releaseX, int releaseY, uint32_t waitAfterRelease, bool sendPress, bool sendRelease)
{
    int button = Button1;
    Display *display = m_mainWindow->getDisplay();

    XEvent event;
    XEvent eventMove;
    memset(&event, 0x00, sizeof(event));
    memset(&eventMove, 0x00, sizeof(event));
    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;
    event.xbutton.x = pressX;
    event.xbutton.y = pressY;
    if (sendPress) {
        XSendEvent(display, *m_mainWindow->getWindow(), True, 0xfff, &event);
    }
    XFlush(display);
    sleep(timeBetweenPressRelease);

    eventMove.xmotion.x = pressX;
    eventMove.xmotion.y = pressY;
    while (1) {
        eventMove.type = MotionNotify;
        eventMove.xmotion.x = moveValueToDirection(eventMove.xmotion.x, releaseX);
        eventMove.xmotion.y = moveValueToDirection(eventMove.xmotion.y, releaseY);
        XSendEvent(display, *m_mainWindow->getWindow(), True, 0xfff, &eventMove);
        XFlush(display);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (eventMove.xmotion.x == releaseX
            && eventMove.xmotion.y == releaseY) {
            break;
        }

    }
    event.type = ButtonRelease;
    event.xbutton.x = releaseX;
    event.xbutton.y = releaseY;
    event.xbutton.state = 0x100;
    if (sendRelease) {
        XSendEvent(display, *m_mainWindow->getWindow(), True, 0xfff, &event);
    }
    XFlush(display);
    sleep(waitAfterRelease);
}

/*!
 * \brief FunctionalTest::moveValueToDirection
 *
 * makes currentValue "smaller" OR "larger" based on
 * directionValue
 *
 * \param currentValue changes this value
 * \param directionValue direction of value
 * \return new current value, based on direction (and current value)
 * if currentValue and directionValue are same, then returns
 * currentValue
 */
int FunctionalTest::moveValueToDirection(int currentValue, int directionValue)
{
    if (currentValue > directionValue) {
        return currentValue - 1;
    }
    if (currentValue < directionValue) {
        return currentValue + 1;
    }
    return directionValue;
}

void FunctionalTest::sendKeyPress(uint32_t key, uint32_t waitAfterRelease, bool keyPress, bool keyRelease)
{
    XKeyEvent event;
    event.display     = m_mainWindow->getDisplay();
    event.window      = *m_mainWindow->getWindow();
    event.root        = *m_mainWindow->getWindow();
    event.subwindow   = None;
    event.time        = CurrentTime;
    event.x           = 1;
    event.y           = 1;
    event.x_root      = 1;
    event.y_root      = 1;
    event.same_screen = True;
    switch (key) {
        case '\t':
            key = XK_Tab;
            break;
        default:
            break;
    }
    event.keycode     = XKeysymToKeycode(m_mainWindow->getDisplay(), key);
    event.state       = 0;
    event.type = KeyPress;
    if (keyPress) {
        XSendEvent(m_mainWindow->getDisplay(), *m_mainWindow->getWindow(), True, KeyPressMask, (XEvent *)&event);
        std::this_thread::sleep_for(std::chrono::milliseconds(waitAfterRelease));
    }
    if (keyRelease) {
        event.type = KeyRelease;
        XSendEvent(m_mainWindow->getDisplay(), *m_mainWindow->getWindow(), True, KeyPressMask, (XEvent *)&event);
        std::this_thread::sleep_for(std::chrono::milliseconds(waitAfterRelease));
    }
}

bool FunctionalTest::isCorrectButtonSelected(int page, int index)
{
    TgButton *button;
    bool found = false;
    int cmpPage;
    int cmpIndex;
    for (cmpPage=0;cmpPage<3;cmpPage++) {
        cmpIndex = 0;
        while (1) {
            button = m_mainWindow->getButton(cmpPage, cmpIndex);
            if (!button) {
                break;
            }
            if (page == cmpPage && index == cmpIndex) {
                if (!button->getSelected()) {
                    TG_ERROR_LOG("Error: Correct button is not selected, page: ", page, " index: ", index);
                    return false;
                }
                found = true;
            } else {
                if (button->getSelected()) {
                    TG_ERROR_LOG("Error: Incorrect button is selected, page: ", cmpPage, " index: ", cmpIndex);
                    return false;
                }
            }
            cmpIndex++;
        }
    }
    if (!found) {
        TG_ERROR_LOG("Error: Correct button is not selected, page: ", page, " index: ", index);
    }
    return found;
}

bool FunctionalTest::makeCharacterCountTest()
{
    m_mainWindow->getTextfield()->setText("meh");
    if (m_mainWindow->getTextfield()->getText() != "meh") {
        TG_ERROR_LOG("Getting text is not correct ", m_mainWindow->getTextfield()->getText());
        return false;
    }

    if (m_mainWindow->getTextfield()->getCharacterCount() != 3) {
        TG_ERROR_LOG("Character could is wrong for textifeld, it should be: 3, it is: ", m_mainWindow->getTextfield()->getCharacterCount());
        return false;
    }
    m_mainWindow->getTextfield()->setText("mehä");
    if (m_mainWindow->getTextfield()->getCharacterCount() != 4) {
        TG_ERROR_LOG("Character could is wrong for textifeld, it should be: 4, it is: ", m_mainWindow->getTextfield()->getCharacterCount());
        return false;
    }
    if (m_mainWindow->getTextfield()->getCharacterByIndex(0) != 'm'
        || m_mainWindow->getTextfield()->getCharacterByIndex(1) != 'e'
        || m_mainWindow->getTextfield()->getCharacterByIndex(2) != 'h'
        || m_mainWindow->getTextfield()->getCharacterByIndex(3) != 0xe4
        || m_mainWindow->getTextfield()->getCharacterByIndex(4) != 0) {
        TG_ERROR_LOG("Text have invalid character",
            m_mainWindow->getTextfield()->getCharacterByIndex(0), " ",
            m_mainWindow->getTextfield()->getCharacterByIndex(1), " ",
            m_mainWindow->getTextfield()->getCharacterByIndex(2), " ",
            m_mainWindow->getTextfield()->getCharacterByIndex(3), " ",
            m_mainWindow->getTextfield()->getCharacterByIndex(4));
        return false;
    }
    std::vector<TgTextFieldText>listText;
    TgTextFieldText t0;
    TgTextFieldText t1;
    TgTextFieldText t2;

    t0.m_text = "x text to";
    t0.m_textColorR = t0.m_textColorG = t0.m_textColorB = 255;
    listText.push_back(t0);
    t1.m_text = " યુનિકોડ";
    t1.m_textColorG = 255;
    t1.m_textColorR = t1.m_textColorB = 0;
    listText.push_back(t1);
    t2.m_text = " 未来";
    t2.m_textColorR = 255;
    t2.m_textColorG = t2.m_textColorB = 0;
    listText.push_back(t2);
    m_mainWindow->getTextfield()->setText(listText);

    if (m_mainWindow->getTextfield()->getText() != "x text to યુનિકોડ 未来") {
        TG_ERROR_LOG("Getting text is not correct ", m_mainWindow->getTextfield()->getText());
        return false;
    }

    if (m_mainWindow->getTextfield()->getCharacterCount() != 20) {
        TG_ERROR_LOG("Character could is wrong for textifeld, it should be: 20, it is: ", m_mainWindow->getTextfield()->getCharacterCount());
        return false;
    }

    m_mainWindow->getTextfield()->setText("");
    if (m_mainWindow->getTextfield()->getCharacterCount() != 0) {
        TG_ERROR_LOG("Character could is wrong for textifeld, it should be: 0, it is: ", m_mainWindow->getTextfield()->getCharacterCount());
        return false;
    }
    m_mainWindow->getTextfield()->setText("中国");
    if (m_mainWindow->getTextfield()->getCharacterCount() != 2) {
        TG_ERROR_LOG("Character could is wrong for textifeld, it should be: 2, it is: ", m_mainWindow->getTextfield()->getCharacterCount());
        return false;
    }
    return true;
}

bool FunctionalTest::makeEditTextTest()
{
    float w, h;
    std::vector<TgTextFieldText>listText;
    TgTextFieldText t0;
    TgTextFieldText t1;
    TgTextFieldText t2;

    sendButtonClick(1, 320, 100, 1);
    t0.m_text = "x text to";
    t0.m_textColorR = t0.m_textColorG = t0.m_textColorB = 124;
    listText.push_back(t0);
    t1.m_text = " યુનિકોડ";
    t1.m_textColorG = 255;
    t1.m_textColorR = t1.m_textColorB = 0;
    listText.push_back(t1);
    t2.m_text = " 未来";
    t2.m_textColorR = 255;
    t2.m_textColorG = t2.m_textColorB = 0;
    listText.push_back(t2);
    m_mainWindow->getTextEdit()->setText(listText);

    sleep(1);
    if (!FunctionalTestImage::isImageToEqual(m_mainWindow, "image_textedit13.png", 800, 600)) {
        return false;
    }
    m_mainWindow->getTextEdit()->setFontSize(23);
    w =  m_mainWindow->getTextEdit()->getTextWidth();
    if (static_cast<int>(w) != 231) {
        TG_ERROR_LOG("Getting text width is incorrect ", w);
        return false;
    }
    h =  m_mainWindow->getTextEdit()->getTextHeight();
    if (static_cast<int>(h) != 19) {
        TG_ERROR_LOG("Getting text width is incorrect ", h);
        return false;
    }

    sleep(1);
    w =  m_mainWindow->getTextEdit()->getTextWidth();
    if (static_cast<int>(w) != 231) {
        TG_ERROR_LOG("Getting text width is incorrect ", w);
        return false;
    }
    h =  m_mainWindow->getTextEdit()->getTextHeight();
    if (static_cast<int>(h) != 19) {
        TG_ERROR_LOG("Getting text width is incorrect ", h);
        return false;
    }

    if (!FunctionalTestImage::isImageToEqual(m_mainWindow, "image_textedit14.png", 800, 600)) {
        return false;
    }

    m_mainWindow->getTextEdit()->setFontSize(21);
    listText.clear();
    t0.m_text = "x text to";
    t0.m_textColorR = t0.m_textColorG = t0.m_textColorB = 255;
    listText.push_back(t0);
    t1.m_text = " યુનિકોડ";
    t1.m_textColorG = 255;
    t1.m_textColorR = t1.m_textColorB = 0;
    listText.push_back(t1);
    t2.m_text = " 未来";
    t2.m_textColorR = 255;
    t2.m_textColorG = t2.m_textColorB = 0;
    listText.push_back(t2);
    m_mainWindow->getTextEdit()->setText(listText);

    if (m_mainWindow->getTextEdit()->getText() != "x text to યુનિકોડ 未来") {
        TG_ERROR_LOG("Getting text is not correct ", m_mainWindow->getTextEdit()->getText());
        return false;
    }

    w =  m_mainWindow->getTextEdit()->getTextWidth();
    if (static_cast<int>(w) != 210) {
        TG_ERROR_LOG("Getting text width is incorrect ", w);
        return false;
    }

    h =  m_mainWindow->getTextEdit()->getTextHeight();
    if (static_cast<int>(h) != 17) {
        TG_ERROR_LOG("Getting text height is incorrect ", h);
        return false;
    }

    m_mainWindow->getTextEdit()->setText("meh");
    if (m_mainWindow->getTextEdit()->getText() != "meh") {
        TG_ERROR_LOG("Getting text is not correct ", m_mainWindow->getTextEdit()->getText());
        return false;
    }

    w =  m_mainWindow->getTextEdit()->getTextWidth();
    if (static_cast<int>(w) != 37) {
        TG_ERROR_LOG("Getting text width is incorrect ", w);
        return false;
    }

    sleep(1);
    w =  m_mainWindow->getTextEdit()->getTextWidth();
    if (static_cast<int>(w) != 37) {
        TG_ERROR_LOG("Getting text width is incorrect ", w);
        return false;
    }

    m_mainWindow->getTextEdit()->setText("abc");
    w =  m_mainWindow->getTextEdit()->getTextWidth();
    if (static_cast<int>(w) != 33) {
        TG_ERROR_LOG("Getting text width is incorrect ", w);
        return false;
    }

    m_mainWindow->getTextEdit()->setText("");
    w = m_mainWindow->getTextEdit()->getTextWidth();
    if (w > 0) {
        TG_ERROR_LOG("Getting text width is incorrect ", w);
        return false;
    }
    sleep(1);
    if (!FunctionalTestImage::isImageToEqual(m_mainWindow, "image_textedit0.png", 800, 600)) {
        return false;
    }
    sendKeyPress('\t', 50);
    if (!FunctionalTestImage::isImageToEqual(m_mainWindow, "image_textedit1.png", 800, 600)) {
        return false;
    }
    sendKeyPress('\t', 50);
    sendKeyPress('\t', 50);

    if (!FunctionalTestImage::isImagesToEqual(m_mainWindow, "image_textedit2_0.png", "image_textedit2_1.png", 800, 600)) {
        return false;
    }

    sendKeyPress('a', 50);
    sendKeyPress('b', 50);
    sendKeyPress('c', 50);
    sendKeyPress('d', 50);
    sendKeyPress(' ', 50);
    sendKeyPress('e', 50);
    sendKeyPress('f', 50);
    sendKeyPress('g', 50);
    sendKeyPress('h', 50);

    if (m_mainWindow->getTextEdit()->getText() != "abcd efgh") {
        TG_ERROR_LOG("Getting text is not correct ", m_mainWindow->getTextEdit()->getText());
        return false;
    }

    if (!FunctionalTestImage::isImagesToEqual(m_mainWindow, "image_textedit3_0.png", "image_textedit3_1.png", 800, 600)) {
        return false;
    }

    sendKeyPress(XK_Left, 50);
    sendKeyPress(XK_Left, 50);

    if (!FunctionalTestImage::isImagesToEqual(m_mainWindow, "image_textedit3_0.png", "image_textedit4_1.png", 800, 600)) {
        return false;
    }

    sendKeyPress('x', 50);
    sendKeyPress(XK_Shift_L, 50, true, false);
    sendKeyPress(XK_Right, 50);
    sendKeyPress(XK_Right, 50);
    sendKeyPress(XK_Right, 50);

    if (!FunctionalTestImage::isImagesToEqual(m_mainWindow, "image_textedit5_0.png", "image_textedit5_1.png", 800, 600)) {
        return false;
    }

    sendKeyPress(XK_Shift_L, 50, false, true);
    if (!FunctionalTestImage::isImagesToEqual(m_mainWindow, "image_textedit5_0.png", "image_textedit5_1.png", 800, 600)) {
        TG_ERROR_LOG("Images testing failed after releasing XK_Shift_L");
        return false;
    }

    sendKeyPress('0', 50);
    sendKeyPress('1', 50);
    sendKeyPress('2', 50);
    sendKeyPress('3', 50);
    sendKeyPress('4', 50);
    sendKeyPress('5', 50);
    sendKeyPress('6', 50);
    sendKeyPress('7', 50);
    sendKeyPress('8', 50);
    sendKeyPress('9', 50);

    sendKeyPress(XK_Shift_R, 50, true, false);
    sendKeyPress('a', 50);
    sendKeyPress('b', 50);
    sendKeyPress('c', 50);
    sendKeyPress('d', 50);
    sendKeyPress('e', 50);
    sendKeyPress('f', 50);
    sendKeyPress('g', 50);
    sendKeyPress('h', 50);
    sendKeyPress(XK_Left, 50);
    sendKeyPress(XK_Left, 50);
    sendKeyPress(XK_Left, 50);
    sendKeyPress(XK_Left, 50);

    if (m_mainWindow->getTextEdit()->getCursorPosition() != 22) {
        TG_ERROR_LOG("Cursor position is not 22, it is: ", m_mainWindow->getTextEdit()->getCursorPosition());
        return false;
    }

    if (m_mainWindow->getTextEdit()->getSelectedTextSize() != 4) {
        TG_ERROR_LOG("Cursor position is not 4, it is: ", m_mainWindow->getTextEdit()->getSelectedTextSize());
        return false;
    }

    sendKeyPress(XK_Shift_R, 50, false, true);

    if (!FunctionalTestImage::isImagesToEqual(m_mainWindow, "image_textedit6_0.png", "image_textedit6_1.png", 800, 600)) {
        return false;
    }

    for (size_t i=0;i<50;i++) {
        sendKeyPress(XK_Left, 50);
    }

    if (!FunctionalTestImage::isImagesToEqual(m_mainWindow, "image_textedit7_0.png", "image_textedit7_1.png", 800, 600)) {
        return false;
    }

    sendButtonMoveClick(1, 35, 110, 85, 115, 1);

    if (!FunctionalTestImage::isImagesToEqual(m_mainWindow, "image_textedit8_0.png", "image_textedit8_1.png", 800, 600)) {
        return false;
    }
    sendButtonMoveClick(1, 35, 110, 330, 115, 1);
    sendKeyPress('o', 50);

    if (!FunctionalTestImage::isImagesToEqual(m_mainWindow, "image_textedit9_0.png", "image_textedit9_1.png", 800, 600)) {
        return false;
    }

    sendKeyPress('0', 50);
    sendKeyPress('1', 50);
    sendKeyPress('2', 50);
    sendKeyPress('3', 50);
    sendKeyPress('4', 50);
    sendKeyPress('5', 50);
    sendKeyPress('6', 50);
    sendKeyPress('7', 50);
    sendKeyPress('8', 50);
    sendKeyPress('9', 50);

    sendKeyPress('a', 50);
    sendKeyPress('b', 50);
    sendKeyPress('c', 50);
    sendKeyPress('d', 50);
    sendKeyPress('e', 50);
    sendKeyPress('f', 50);
    sendKeyPress('g', 50);
    sendKeyPress('h', 50);

    sendKeyPress('0', 50);
    sendKeyPress('1', 50);
    sendKeyPress('2', 50);
    sendKeyPress('3', 50);
    sendKeyPress('4', 50);
    sendKeyPress('5', 50);
    sendKeyPress('6', 50);
    sendKeyPress('7', 50);
    sendKeyPress('8', 50);
    sendKeyPress('9', 50);

    sendKeyPress('a', 50);
    sendKeyPress('b', 50);
    sendKeyPress('c', 50);
    sendKeyPress('d', 50);
    sendKeyPress('e', 50);
    sendKeyPress('f', 50);
    sendKeyPress('g', 50);
    sendKeyPress('h', 50);
    sendButtonMoveClick(1, 240, 110, 0, 115, 1);
    sendKeyPress('j', 50);

    if (!FunctionalTestImage::isImagesToEqual(m_mainWindow, "image_textedit10_0.png", "image_textedit10_1.png", 800, 600)) {
        return false;
    }

    if (m_mainWindow->getTextEdit()->getCursorPosition() != 1) {
        TG_ERROR_LOG("Cursor position is not 1, it is: ", m_mainWindow->getTextEdit()->getCursorPosition());
        return false;
    }

    if (m_mainWindow->getTextEdit()->getCharacterCount() != 4) {
        TG_ERROR_LOG("Character count is not 4, it is: ", m_mainWindow->getTextEdit()->getCharacterCount());
        return false;
    }

    if (m_mainWindow->getTextEdit()->getCharacterByIndex(0) != 'j'
        || m_mainWindow->getTextEdit()->getCharacterByIndex(1) != 'f'
        || m_mainWindow->getTextEdit()->getCharacterByIndex(2) != 'g'
        || m_mainWindow->getTextEdit()->getCharacterByIndex(3) != 'h'
        || m_mainWindow->getTextEdit()->getCharacterByIndex(4) != 0) {
        TG_ERROR_LOG("Text have invalid character",
            m_mainWindow->getTextEdit()->getCharacterByIndex(0), " ",
            m_mainWindow->getTextEdit()->getCharacterByIndex(1), " ",
            m_mainWindow->getTextEdit()->getCharacterByIndex(2), " ",
            m_mainWindow->getTextEdit()->getCharacterByIndex(3), " ",
            m_mainWindow->getTextEdit()->getCharacterByIndex(4));
        return false;
    }
    if (m_mainWindow->getTextEdit()->getSelectedTextSize() != 0) {
        TG_ERROR_LOG("Cursor selected text size is not 0, it is: ", m_mainWindow->getTextEdit()->getSelectedTextSize());
        return false;
    }

    sendKeyPress(XK_Shift_R, 50, true, false);
    sendKeyPress(XK_Right, 50);
    sendKeyPress(XK_Shift_R, 50, false, true);

    if (m_mainWindow->getTextEdit()->getCursorPosition() != 2) {
        TG_ERROR_LOG("Cursor position is not 2, it is: ", m_mainWindow->getTextEdit()->getCursorPosition());
        return false;
    }

    if (m_mainWindow->getTextEdit()->getSelectedTextSize() != -1) {
        TG_ERROR_LOG("Cursor selected text size is not -1, it is: ", m_mainWindow->getTextEdit()->getSelectedTextSize());
        return false;
    }

    sendKeyPress(XK_Right, 50);
    sendKeyPress(XK_Shift_R, 50, true, false);
    sendKeyPress(XK_Left, 50);
    sendKeyPress(XK_Shift_R, 50, false, true);

    if (m_mainWindow->getTextEdit()->getCursorPosition() != 1) {
        TG_ERROR_LOG("Cursor position is not 1, it is: ", m_mainWindow->getTextEdit()->getCursorPosition());
        return false;
    }

    if (m_mainWindow->getTextEdit()->getSelectedTextSize() != 1) {
        TG_ERROR_LOG("Cursor selected text size is not 1, it is: ", m_mainWindow->getTextEdit()->getSelectedTextSize());
        return false;
    }
    sendButtonClick(1, 320, 35, 1);
    if (m_mainWindow->getTextEdit()->getCharacterByIndex(0) != 'a'
        || m_mainWindow->getTextEdit()->getCharacterByIndex(1) != 'b'
        || m_mainWindow->getTextEdit()->getCharacterByIndex(2) != 'c'
        || m_mainWindow->getTextEdit()->getCharacterByIndex(3) != 0) {
        TG_ERROR_LOG("Text have invalid character",
            m_mainWindow->getTextEdit()->getCharacterByIndex(0), " ",
            m_mainWindow->getTextEdit()->getCharacterByIndex(1), " ",
            m_mainWindow->getTextEdit()->getCharacterByIndex(2), " ",
            m_mainWindow->getTextEdit()->getCharacterByIndex(3));
        return false;
    }

    w =  m_mainWindow->getTextEdit()->getTextWidth();
    if (static_cast<int>(w) != 33) {
        TG_ERROR_LOG("Getting text width is incorrect ", w);
        return false;
    }

    h =  m_mainWindow->getTextEdit()->getTextHeight();
    if (static_cast<int>(h) != 16) {
        TG_ERROR_LOG("Getting text height is incorrect ", h);
        return false;
    }

    if (m_mainWindow->getTextEdit()->getSelectedTextSize() != 0) {
        TG_ERROR_LOG("Cursor selected text size is not 0, it is: ", m_mainWindow->getTextEdit()->getSelectedTextSize());
        return false;
    }

    if (!FunctionalTestImage::isImageToEqual(m_mainWindow, "image_textedit11.png", 800, 600)) {
        return false;
    }
    sendButtonMoveClick(1, 35, 110, 36, 110, 1, false, false);
    if (!FunctionalTestImage::isImageToEqual(m_mainWindow, "image_textedit12.png", 800, 600)) {
        return false;
    }


    sendButtonClick(1, 40, 35, 1);
    sendButtonMoveClick(0, 400, 400, 401, 401, 0, false, false);
    sleep(1);
    return true;
}

bool FunctionalTest::makeTextFieldTest()
{
    int i;
    float value;
    float value2;
    const int numberOfClicks = 38;

    for (i=0;i<45;i++) {
        sendButtonClick(1, 320, 40, 1);
        if (i == 0) {
            sendButtonMoveClick(0, 320, 40, 320, 90, 0, false, false);
        }

        std::string tmp;
        if ((i % numberOfClicks) == 0 && i != 0) {
            tmp = "image_text" + std::to_string(i % numberOfClicks) + "_second.png";
        } else {
            tmp = "image_text" + std::to_string(i % numberOfClicks) + ".png";
        }
        if ((i % numberOfClicks) == i && i >= 1) {
            value = m_mainWindow->getAllDrawHeight(2, i);
        }
        if (!FunctionalTestImage::isImageToEqual(m_mainWindow, tmp.c_str(), 800, 600)) {
            TG_ERROR_LOG("Incorrect image ", tmp, " index: ", i);
            return false;
        }
        if ((i % numberOfClicks) == i && i >= 1) {
            value2 = m_mainWindow->getTextfield(2)->getAllDrawTextHeight();
            if (std::fabs(value - value2) > std::numeric_limits<double>::epsilon()) {
                TG_ERROR_LOG("Incorrect All draw text height: ", value, " ", value2, " index:", i);
                return false;
            }
        }
    }
    return true;
}