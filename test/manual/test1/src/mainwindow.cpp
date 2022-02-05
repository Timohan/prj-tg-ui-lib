#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "TestApp1"),
    m_buttonClose(this, 20, 20, 200, 50, "Close button"),
    m_buttonAdd(this, 240, 20, 200, 50, "Add button"),
    m_buttonRemove(this, 460, 20, 200, 50, "Remove button"),
    m_buttonAdded(nullptr),
    m_buttonAddCount(this, 20, 160, 300, 50, "Add mouse button count"),
    m_buttonReduceCount(this, 340, 160, 300, 50, "Reduce mouse button count")
{
    m_buttonClose.connectOnMouseClicked( std::bind(&MainWindow::onButtonCloseClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonAdd.connectOnMouseClicked( std::bind(&MainWindow::onButtonAddClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonRemove.connectOnMouseClicked( std::bind(&MainWindow::onButtonRemoveClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );

    m_buttonAddCount.connectOnMouseClicked( std::bind(&MainWindow::onButtonAddedCountClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonReduceCount.connectOnMouseClicked( std::bind(&MainWindow::onButtonReduceCountClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );

}

MainWindow::~MainWindow()
{
    if (m_buttonAdded) {
        delete m_buttonAdded;
        m_buttonAdded = nullptr;
    }
}

/*!
 * \brief MainWindow::onButtonAddClick
 *
 * callback when button add is clicked
 */
void MainWindow::onButtonAddClick(TgMouseType type, float x, float y)
{
    if (m_buttonAdded) {
        std::cout << "Adding the button - button is already added  - callback by button: " << type << "\n";
        return;
    }
    std::cout << "Adding the button  - callback by button: " << type << "\n";
    m_buttonAdded = new TgButton(this, 20, 90, 200, 50, "Added button");
    m_buttonAdded->connectOnMouseClicked( std::bind(&MainWindow::onButtonAddedClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
}

/*!
 * \brief MainWindow::onButtonRemoveClick
 *
 * callback when button remove is clicked
 */
void MainWindow::onButtonRemoveClick(TgMouseType type, float x, float y)
{
    if (!m_buttonAdded) {
        std::cout << "Removing the button - button is not added  - callback by button: "  << type << "\n";
        return;
    }
    std::cout << "Removing the button - callback by button: "  << type << "\n";
    TgItem2d *item = m_buttonAdded;
    delete item;
    m_buttonAdded = nullptr;
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
 * \brief MainWindow::onButtonAddedClick
 *
 * callback when button added is clicked
 */
void MainWindow::onButtonAddedClick(TgMouseType type, float x, float y)
{
    std::cout << "Added button clicked\n";
}

/*!
 * \brief MainWindow::onButtonAddedCountClick
 *
 * callback when button count added
 */
void MainWindow::onButtonAddedCountClick(TgMouseType type, float x, float y)
{
    setAllowedNumberMouseButtonCount(getAllowedNumberMouseButtonCount()+1);
    std::cout << "Added button count clicked " << getAllowedNumberMouseButtonCount() << "\n";
}

/*!
 * \brief MainWindow::onButtonReduceCountClick
 *
 * callback when button count reduced
 */
void MainWindow::onButtonReduceCountClick(TgMouseType type, float x, float y)
{
    if (getAllowedNumberMouseButtonCount() > 0) {
        setAllowedNumberMouseButtonCount(getAllowedNumberMouseButtonCount()-1);
    }
    std::cout << "Reduce button count clicked " << getAllowedNumberMouseButtonCount() << "\n";
}
