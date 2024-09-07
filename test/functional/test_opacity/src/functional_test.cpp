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
                    break;
                case IsCorrectHover:
                    break;
                case IsButtonDownCount:
                    break;
                case isHoverCount:
                    break;
                case setVisibleItem:
                    break;
                case getMouseCursorOnHover:
                    break;
                case isVisible:
/*                    if (!isCorrectVisible(
                                        m_testOrders.getTestOrder(i)->m_listNumber.at(0),
                                        m_testOrders.getTestOrder(i)->m_listNumber.at(1))) {
                        TG_ERROR_LOG("Visible change is incorrect, index: ", m_testOrders.getTestOrder(i)->m_lineNumber);
                        m_returnIndex = 1;
                        m_mainWindow->exit();
                        return;
                    }*/
                    break;
                case setEnabledItem:
                    break;
                case isEnabled:
                    break;
                case NormalInfoMessage:
                    TG_INFO_LOG("Msg: ", m_testOrders.getTestOrder(i)->m_listString.at(0));
                    break;
                case TestOrderType::isMove:
                    break;
                case TestOrderType::isMousePressed:
                    break;
                case TestOrderType::isMouseReleased:
                    break;
                case TestOrderType::isMouseClicked:
                    break;
                case setSelected:
                    break;
                case isItemSelected:
                    break;
                case TestOrderType::isImage:
                    std::this_thread::sleep_for(std::chrono::milliseconds( 100 ) );
                    if (!FunctionalTestImage::isImageToEqual(m_mainWindow,
                        m_testOrders.getTestOrder(i)->m_listString[0].c_str(), 800, 600)) {
                        TG_ERROR_LOG("Image is not correct, index: ", m_testOrders.getTestOrder(i)->m_lineNumber, "/", m_testOrders.getTestOrder(i)->m_listString[0]);
                        m_returnIndex = 1;
                        sleep(10);
                        m_mainWindow->exit();
                        return;
                    }
                    break;
                case TestOrderType::SleepWaitTimeMs:
                    std::this_thread::sleep_for(std::chrono::milliseconds(m_testOrders.getTestOrder(i)->m_listNumber.at(0)));
                    break;
                case TestOrderType::MakeStep:
                    m_mainWindow->setMakeStep( m_testOrders.getTestOrder(i)->m_listNumber.at(0) );
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

