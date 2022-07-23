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
        sleep(2);
        size_t i;
        m_testOrders.loadOrders();
        TG_INFO_LOG("Start rolling orders: ", m_testOrders.getOrdersCount());
        for (i=0;i<m_testOrders.getOrdersCount();i++) {
            switch (m_testOrders.getTestOrder(i)->m_type) {
                case TestOrderType::MouseMoveClick:
                    sendButtonMoveClick(m_testOrders.getTestOrder(i)->m_listNumber.at(0),
                                        m_testOrders.getTestOrder(i)->m_listNumber.at(1),
                                        m_testOrders.getTestOrder(i)->m_listNumber.at(2),
                                        m_testOrders.getTestOrder(i)->m_listNumber.at(3),
                                        m_testOrders.getTestOrder(i)->m_listNumber.at(4),
                                        m_testOrders.getTestOrder(i)->m_listNumber.at(5),
                                        m_testOrders.getTestOrder(i)->m_listNumber.at(6),
                                        m_testOrders.getTestOrder(i)->m_listNumber.at(7));
                    break;
                case IsCorrectHover:
                    if (!isCorrectHover(m_testOrders.getTestOrder(i)->m_listNumber.at(0),
                                        m_testOrders.getTestOrder(i)->m_listNumber.at(1))) {
                        TG_ERROR_LOG("Hover change is incorrect, index: ", m_testOrders.getTestOrder(i)->m_lineNumber);
                        m_returnIndex = 1;
                        m_mainWindow->exit();
                        return;
                    }
                    break;
                case IsButtonDownCount:
                    if (m_mainWindow->getMouseCaptureArea(m_testOrders.getTestOrder(i)->m_listNumber.at(0))->getMouseDownCount() !=
                        static_cast<size_t>(m_testOrders.getTestOrder(i)->m_listNumber.at(1))) {
                        TG_ERROR_LOG("Mouse capture - mouse down count is wrong, index: ", m_testOrders.getTestOrder(i)->m_lineNumber);
                        m_returnIndex = 1;
                        m_mainWindow->exit();
                        return;
                    }
                    break;
                case isHoverCount:
                    if (m_mainWindow->getMouseStateChangeCount() != static_cast<size_t>(m_testOrders.getTestOrder(i)->m_listNumber.at(0))) {
                        TG_ERROR_LOG("Hover change count is incorrect ", m_mainWindow->getMouseStateChangeCount(), " should be: ", static_cast<size_t>(m_testOrders.getTestOrder(i)->m_listNumber.at(0)), m_testOrders.getTestOrder(i)->m_lineNumber);
                        m_returnIndex = 1;
                        m_mainWindow->exit();
                        return;
                    }
                    break;
                case setVisibleItem:
                    m_mainWindow->getMouseCaptureArea( m_testOrders.getTestOrder(i)->m_listNumber.at(0) )->setVisible( m_testOrders.getTestOrder(i)->m_listNumber.at(1) );
                    break;
                case getMouseCursorOnHover:
                    if (m_mainWindow->getMouseCaptureArea( m_testOrders.getTestOrder(i)->m_listNumber.at(0) )->getMouseCursorOnHover() != m_testOrders.getTestOrder(i)->m_listNumber.at(1)) {
                        TG_ERROR_LOG("getMouseCaptureArea change is incorrect, index: ", m_testOrders.getTestOrder(i)->m_lineNumber);
                        m_returnIndex = 1;
                        m_mainWindow->exit();
                        return;
                    }
                    break;
                case isVisible:
                    if (!isCorrectVisible(
                                        m_testOrders.getTestOrder(i)->m_listNumber.at(0),
                                        m_testOrders.getTestOrder(i)->m_listNumber.at(1))) {
                        TG_ERROR_LOG("Visible change is incorrect, index: ", m_testOrders.getTestOrder(i)->m_lineNumber);
                        m_returnIndex = 1;
                        m_mainWindow->exit();
                        return;
                    }
                    break;
                case setEnabledItem:
                    m_mainWindow->getMouseCaptureArea( m_testOrders.getTestOrder(i)->m_listNumber.at(0) )->setEnabled( m_testOrders.getTestOrder(i)->m_listNumber.at(1) );
                    break;
                case isEnabled:
                    if (!isCorrectEnabled(
                                        m_testOrders.getTestOrder(i)->m_listNumber.at(0),
                                        m_testOrders.getTestOrder(i)->m_listNumber.at(1))) {
                        TG_ERROR_LOG("Enabled change is incorrect, index: ", m_testOrders.getTestOrder(i)->m_lineNumber);
                        m_returnIndex = 1;
                        m_mainWindow->exit();
                        return;
                    }
                    break;
                case NormalInfoMessage:
                    TG_INFO_LOG("Msg: ", m_testOrders.getTestOrder(i)->m_listString.at(0));
                    break;
                case TestOrderType::isMove:
                    if (!isMove(m_testOrders.getTestOrder(i)->m_listNumber.at(0),
                        m_testOrders.getTestOrder(i)->m_listNumber.at(1),
                        m_testOrders.getTestOrder(i)->m_listNumber.at(2),
                        m_testOrders.getTestOrder(i)->m_listNumber.at(3),
                        m_testOrders.getTestOrder(i)->m_listNumber.at(4),
                        m_testOrders.getTestOrder(i)->m_listNumber.at(5))) {
                        TG_ERROR_LOG("isMove change is incorrect, index: ", m_testOrders.getTestOrder(i)->m_lineNumber);
                        m_returnIndex = 1;
                        m_mainWindow->exit();
                        return;
                    }
                    break;
                case TestOrderType::isMousePressed:
                    if (!checkIsMousePressed(m_testOrders.getTestOrder(i)->m_listNumber.at(0),
                        m_testOrders.getTestOrder(i)->m_listNumber.at(1),
                        m_testOrders.getTestOrder(i)->m_listNumber.at(2))) {
                        TG_ERROR_LOG("isMousePressed change is incorrect, index: ", m_testOrders.getTestOrder(i)->m_lineNumber);
                        m_returnIndex = 1;
                        m_mainWindow->exit();
                        return;
                    }
                    break;
                case TestOrderType::isMouseReleased:
                    if (!checkIsMouseReleased(m_testOrders.getTestOrder(i)->m_listNumber.at(0),
                        m_testOrders.getTestOrder(i)->m_listNumber.at(1),
                        m_testOrders.getTestOrder(i)->m_listNumber.at(2),
                        m_testOrders.getTestOrder(i)->m_listNumber.at(3))) {
                        TG_ERROR_LOG("isMouseReleased change is incorrect, index: ", m_testOrders.getTestOrder(i)->m_lineNumber);
                        m_returnIndex = 1;
                        m_mainWindow->exit();
                        return;
                    }
                    break;
                case TestOrderType::isMouseClicked:
                    if (!checkIsMouseClicked(m_testOrders.getTestOrder(i)->m_listNumber.at(0),
                        m_testOrders.getTestOrder(i)->m_listNumber.at(1),
                        m_testOrders.getTestOrder(i)->m_listNumber.at(2))) {
                        TG_ERROR_LOG("isMouseClicked change is incorrect, index: ", m_testOrders.getTestOrder(i)->m_lineNumber);
                        m_returnIndex = 1;
                        m_mainWindow->exit();
                        return;
                    }
                    break;
                default:
                    TG_ERROR_LOG("Test case is incorrect");
                    m_returnIndex = 1;
                    m_mainWindow->exit();
                    return;
            }
        }
        TG_INFO_LOG("All tests ok");
        sleep(1);
        m_mainWindow->exit();
    }).detach();
}

bool FunctionalTest::isMove(int index, int fromX, int fromY, int toX, int toY, bool inArea)
{
    int x = fromX;
    int y = fromY;
    while (1) {
        if (!isMove(index, x, y, inArea)) {
            return false;
        }
        if (x == toX && y == toY) {
            break;
        }
        x = moveValueToDirection(x, toX);
        y = moveValueToDirection(y, toY);
    }

    return true;
}

bool FunctionalTest::isMove(int index, int x, int y, bool inArea)
{
    HoverVisibleChangeState wantedState = HoverVisibleChangeState::MouseMove;
    if (m_mainWindow->getMouseStateChangeCount() == m_latestHoverIndex) {
        return false;
    }
    if (m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_index != static_cast<size_t>(index)
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_x != x
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_y != y
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_area != inArea
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_state != wantedState) {
        TG_ERROR_LOG("Move change is incorrect ", m_latestHoverIndex, index, x, y, inArea,
            m_mainWindow->getString(wantedState),
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_index,
            m_mainWindow->getString(m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_state),
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_x,
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_y,
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_area);
        return false;
    }
    m_latestHoverIndex++;
    return true;
}

bool FunctionalTest::checkIsMousePressed(int index, int x, int y)
{
    if (m_mainWindow->getMouseStateChangeCount() == m_latestHoverIndex) {
        return false;
    }
    HoverVisibleChangeState wantedState = HoverVisibleChangeState::MousePress;
    if (m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_index != static_cast<size_t>(index)
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_x != x
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_y != y
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_state != wantedState) {
        TG_ERROR_LOG("MousePressed change is incorrect ", m_latestHoverIndex, index, x, y,
            m_mainWindow->getString(wantedState),
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_index,
            m_mainWindow->getString(m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_state),
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_x,
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_y);
        return false;
    }
    m_latestHoverIndex++;
    return true;
}

bool FunctionalTest::checkIsMouseClicked(int index, int x, int y)
{
    if (m_mainWindow->getMouseStateChangeCount() == m_latestHoverIndex) {
        return false;
    }
    HoverVisibleChangeState wantedState = HoverVisibleChangeState::MouseClicked;
    if (m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_index != static_cast<size_t>(index)
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_x != x
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_y != y
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_state != wantedState) {
        TG_ERROR_LOG("MouseClicked change is incorrect ", m_latestHoverIndex, index, x, y,
            m_mainWindow->getString(wantedState),
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_index,
            m_mainWindow->getString(m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_state),
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_x,
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_y);
        return false;
    }
    m_latestHoverIndex++;
    return true;
}

bool FunctionalTest::checkIsMouseReleased(int index, int x, int y, bool inArea)
{
    if (m_mainWindow->getMouseStateChangeCount() == m_latestHoverIndex) {
        return false;
    }
    HoverVisibleChangeState wantedState = HoverVisibleChangeState::MouseRelease;
    if (m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_index != static_cast<size_t>(index)
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_x != x
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_y != y
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_area != inArea
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_state != wantedState) {
        TG_ERROR_LOG("checkIsMouseReleased change is incorrect ", m_latestHoverIndex, index, x, y, inArea,
            m_mainWindow->getString(wantedState),
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_index,
            m_mainWindow->getString(m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_state),
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_x,
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_y,
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_area);
        return false;
    }
    m_latestHoverIndex++;
    return true;
}

bool FunctionalTest::isCorrectHover(size_t index, bool hover)
{
    if (m_mainWindow->getMouseStateChangeCount() == m_latestHoverIndex) {
        return false;
    }
    HoverVisibleChangeState wantedState = hover ? HoverVisibleChangeState::HoverEnable : HoverVisibleChangeState::HoverDisable;
    if (m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_index != index
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_state != wantedState) {
        TG_ERROR_LOG("Hover change is incorrect ", m_latestHoverIndex, index,
            m_mainWindow->getString(wantedState),
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_index,
            m_mainWindow->getString(m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_state));
        return false;
    }
    m_latestHoverIndex++;
    return true;
}

bool FunctionalTest::isCorrectVisible(size_t index, bool visible)
{
    if (m_mainWindow->getMouseStateChangeCount() == m_latestHoverIndex) {
        return false;
    }
    HoverVisibleChangeState wantedState = visible ? HoverVisibleChangeState::Visible : HoverVisibleChangeState::Invisible;
    if (m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_index != index
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_state !=  wantedState) {
        TG_ERROR_LOG("Visible change is incorrect ", m_latestHoverIndex, index,
            m_mainWindow->getString(wantedState),
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_index,
            m_mainWindow->getString(m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_state));
        return false;
    }
    m_latestHoverIndex++;
    return true;
}

bool FunctionalTest::isCorrectEnabled(size_t index, bool enabled)
{
    if (m_mainWindow->getMouseStateChangeCount() == m_latestHoverIndex) {
        return false;
    }
    HoverVisibleChangeState wantedState = enabled ? HoverVisibleChangeState::Enabled : HoverVisibleChangeState::Disabled;
    if (m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_index != index
        || m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_state !=  wantedState) {
        TG_ERROR_LOG("Enabled change is incorrect ", m_latestHoverIndex, index,
            m_mainWindow->getString(wantedState),
            m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_index,
            m_mainWindow->getString(m_mainWindow->getMouseStateChange(m_latestHoverIndex)->m_state));
        return false;
    }
    m_latestHoverIndex++;
    return true;
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
    if (timeBetweenPressRelease) {
        sleep(timeBetweenPressRelease);
    }

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
    if (waitAfterRelease) {
        sleep(waitAfterRelease);
    }
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
