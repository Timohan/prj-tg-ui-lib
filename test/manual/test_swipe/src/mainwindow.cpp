#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "TestSwipe"),
    m_mouseCaptureForSwipe(this),
    m_buttonClose(this, 20, 20, 200, 50, "Close button"),
    m_buttonUp(this, 20, 80, 200, 50, "Up"),
    m_buttonDown(this, 20, 140, 200, 50, "Down"),
    m_buttonLeft(this, 20, 200, 200, 50, "Left"),
    m_buttonRight(this, 20, 260, 200, 50, "Right"),
    m_swipeAvailableUp(true),
    m_swipeAvailableDown(true),
    m_swipeAvailableLeft(true),
    m_swipeAvailableRight(true)
{
    m_mouseCaptureForSwipe.connectOnMouseSwipe( std::bind(&MainWindow::onSwipe, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonClose.connectOnMouseClicked( std::bind(&MainWindow::onButtonCloseClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonUp.connectOnMouseClicked( std::bind(&MainWindow::onButtonUpClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonDown.connectOnMouseClicked( std::bind(&MainWindow::onButtonDownClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonLeft.connectOnMouseClicked( std::bind(&MainWindow::onButtonLeftClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonRight.connectOnMouseClicked( std::bind(&MainWindow::onButtonRightClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_mouseCaptureForSwipe.connectOnMouseClicked( std::bind(&MainWindow::onButtonClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
}

MainWindow::~MainWindow()
{
}

/*!
 * \brief MainWindow::setSwipeType
 *
 * set swipe type
 */
void MainWindow::setSwipeType()
{
    uint8_t newSwipeType = 0;
    if (m_swipeAvailableUp) {
        newSwipeType |= SwipeType::SwipeTypeUp;
    }
    if (m_swipeAvailableDown) {
        newSwipeType |= SwipeType::SwipeTypeDown;
    }
    if (m_swipeAvailableLeft) {
        newSwipeType |= SwipeType::SwipeTypeLeft;
    }
    if (m_swipeAvailableRight) {
        newSwipeType |= SwipeType::SwipeTypeRight;
    }
    m_mouseCaptureForSwipe.setSwipeType(newSwipeType);
}

/*!
 * \brief MainWindow::generateSwipeTypeToString
 *
 * \return generated swipe type string
 */
std::string MainWindow::generateSwipeTypeToString()
{
    std::string ret = "Available swipe types: ";
    if (m_mouseCaptureForSwipe.getSwipeType() & SwipeType::SwipeTypeUp) {
        ret += " Up";
    }
    if (m_mouseCaptureForSwipe.getSwipeType() & SwipeType::SwipeTypeDown) {
        ret += " Down";
    }
    if (m_mouseCaptureForSwipe.getSwipeType() & SwipeType::SwipeTypeLeft) {
        ret += " Left";
    }
    if (m_mouseCaptureForSwipe.getSwipeType() & SwipeType::SwipeTypeRight) {
        ret += " Right";
    }
    return ret;
}

/*!
 * \brief MainWindow::onButtonAddClick
 *
 * callback when button up is clicked
 */
void MainWindow::onSwipe(TgMouseType type, SwipeType swipeType, double time)
{
    std::cout << "Swipe clicked " << type << " " << swipeType << " " << time << "\n";
}

/*!
 * \brief MainWindow::onButtonAddClick
 *
 * callback when button up is clicked
 */
void MainWindow::onButtonUpClick(TgMouseType type, float x, float y)
{
    m_swipeAvailableUp = !m_swipeAvailableUp;
    setSwipeType();
    std::cout << "Up clicked " << generateSwipeTypeToString() << "\n";
}

/*!
 * \brief MainWindow::onButtonDownClick
 *
 * callback when button down is clicked
 */
void MainWindow::onButtonDownClick(TgMouseType type, float x, float y)
{
    m_swipeAvailableDown = !m_swipeAvailableDown;
    setSwipeType();
    std::cout << "Down clicked " << generateSwipeTypeToString() << "\n";
}

/*!
 * \brief MainWindow::onButtonLeftClick
 *
 * callback when button left is clicked
 */
void MainWindow::onButtonLeftClick(TgMouseType type, float x, float y)
{
    m_swipeAvailableLeft = !m_swipeAvailableLeft;
    setSwipeType();
    std::cout << "Left clicked " << generateSwipeTypeToString() << "\n";
}

/*!
 * \brief MainWindow::onButtonRightClick
 *
 * callback when button right is clicked
 */
void MainWindow::onButtonRightClick(TgMouseType type, float x, float y)
{
    m_swipeAvailableRight = !m_swipeAvailableRight;
    setSwipeType();
    std::cout << "Right clicked " << generateSwipeTypeToString() << "\n";
}

/*!
 * \brief MainWindow::onButtonCloseClick
 *
 * callback when button close is clicked
 */
void MainWindow::onButtonCloseClick(TgMouseType type, float x, float y)
{
    std::cout << "Closing the application\n";
    exit();
}

/*!
 * \brief MainWindow::onButtonClick
 *
 * callback when button close is clicked
 */
void MainWindow::onButtonClick(TgMouseType type, float x, float y)
{
    std::cout << "Mouse click\n";
}
