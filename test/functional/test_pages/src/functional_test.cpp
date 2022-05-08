#include "functional_test.h"
#include <thread>
#include <unistd.h>
#include "../../../../lib/src/global/tg_global_log.h"
#include <X11/Xlib.h>
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

        if (!FunctionalTestImage::isImageToEqual(m_mainWindow, "image0.png", 800, 600)) {
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }
        for (i=0;i<20;i++) {
            sendButtonClick(1, 320, 70, 1);
            std::string tmp;
            if ((i % 11) == 0 && i != 0) {
                tmp = "image_text" + std::to_string(i % 11) + "_second.png";
            } else {
                tmp = "image_text" + std::to_string(i % 11) + ".png";
            }
            if (!FunctionalTestImage::isImageToEqual(m_mainWindow, tmp.c_str(), 800, 600)) {
                sleep(1);
                m_returnIndex = 1;
                m_mainWindow->exit();
                return;
            }
        }

        sendButtonClick(1, 155, 70, 1);

        sendButtonClick(1, 155, 70, 1);
        if (m_mainWindow->getPages()->getCurrentPageIndex() != 1) {
            TG_ERROR_LOG("Page index is not 1 it is: ", m_mainWindow->getPages()->getCurrentPageIndex());
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }
        sendButtonClick(1, 155, 70, 1);
        if (m_mainWindow->getPages()->getCurrentPageIndex() != 0) {
            TG_ERROR_LOG("Page index is not 0 it is: ", m_mainWindow->getPages()->getCurrentPageIndex());
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }
        for (i=0;i<8;i++) {
            sendKeyPress('\t', 1);
            if (!isCorrectButtonSelected(0, i % 3)) {
                m_returnIndex = 1;
                m_mainWindow->exit();
                return;
            }
        }
        sendButtonMoveClick(1, 155, 70, 155, 200, 1);
        if (!isCorrectButtonSelected(0, 0)) {
            m_returnIndex = 1;
            m_mainWindow->exit();
            return;
        }
        sendButtonMoveClick(1, 355, 70, 155, 200, 1);
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

void FunctionalTest::sendButtonMoveClick(uint32_t timeBetweenPressRelease, int pressX, int pressY, int releaseX, int releaseY, uint32_t waitAfterRelease)
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
    XSendEvent(display, *m_mainWindow->getWindow(), True, 0xfff, &event);
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
        usleep(1000);
        if (eventMove.xmotion.x == releaseX
            && eventMove.xmotion.y == releaseY) {
            break;
        }

    }
    event.type = ButtonRelease;
    event.xbutton.x = releaseX;
    event.xbutton.y = releaseY;
    event.xbutton.state = 0x100;
    XSendEvent(display, *m_mainWindow->getWindow(), True, 0xfff, &event);
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

void FunctionalTest::sendKeyPress(uint32_t key, uint32_t waitAfterRelease)
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
    event.keycode     = XKeysymToKeycode(m_mainWindow->getDisplay(), XK_Tab);
    event.state       = 0;
    event.type = KeyPress;
    XSendEvent(m_mainWindow->getDisplay(), *m_mainWindow->getWindow(), True, KeyPressMask, (XEvent *)&event);
    sleep(waitAfterRelease);
    event.type = KeyRelease;
    XSendEvent(m_mainWindow->getDisplay(), *m_mainWindow->getWindow(), True, KeyPressMask, (XEvent *)&event);
    sleep(waitAfterRelease);
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
