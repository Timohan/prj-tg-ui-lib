#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "TestApp1", width - 300, height - 300),
    m_buttonClose(this, 20, 20, 200, 50, "Close button"),
    m_buttonAdd(this, 240, 20, 200, 50, "Add button"),
    m_buttonRemove(this, 460, 20, 200, 50, "Remove button"),
    m_buttonAdded(nullptr),
    m_buttonAddCount(this, 20, 160, 300, 50, "Add mouse button count"),
    m_buttonReduceCount(this, 340, 160, 300, 50, "Reduce mouse button count")
{
    m_buttonClose.connectOnMouseClicked( std::bind(&MainWindow::onButtonCloseClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );
    m_buttonAdd.connectOnMouseClicked( std::bind(&MainWindow::onButtonAddClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), reinterpret_cast<const void *>("Adding id!"));
    m_buttonRemove.connectOnMouseClicked( std::bind(&MainWindow::onButtonRemoveClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );

    m_buttonAddCount.connectOnMouseClicked( std::bind(&MainWindow::onButtonAddedCountClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );
    m_buttonReduceCount.connectOnMouseClicked( std::bind(&MainWindow::onButtonReduceCountClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );

    m_buttonClose.setTooltip("Close");
    m_buttonAdd.setTooltip("Add\nItem");
    m_buttonRemove.setTooltip("Remove item");
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
void MainWindow::onButtonAddClick(TgMouseType type, float x, float y, const void *id)
{
    if (m_buttonAdded) {
        std::cout << "Adding the button - button is already added  - callback by button: " << type << "\n";
        return;
    }
    std::cout << "Adding the button  - callback by button: " << type << " id: " << reinterpret_cast<const char *>(id) << "\n";
    m_buttonAdded = new TgButton(this, 20, 90, 200, 50, "Added button");
    m_buttonAdded->connectOnMouseClicked( std::bind(&MainWindow::onButtonAddedClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );
    m_buttonRemove.setNextTabItem(m_buttonAdded);
    m_buttonAdded->setPrevTabItem(&m_buttonRemove);
    m_buttonAdded->setNextTabItem(&m_buttonAddCount);
    m_buttonAddCount.setPrevTabItem(m_buttonAdded);
}

/*!
 * \brief MainWindow::onButtonRemoveClick
 *
 * callback when button remove is clicked
 */
void MainWindow::onButtonRemoveClick(TgMouseType type, float x, float y, const void *id)
{
    if (!m_buttonAdded) {
        std::cout << "Removing the button - button is not added  - callback by button: "  << type << "\n";
        return;
    }
    std::cout << "Removing the button - callback by button: "  << type << "\n";
    m_buttonAdded->deleteLater();
    m_buttonAdded = nullptr;
}

/*!
 * \brief MainWindow::onButtonCloseClick
 *
 * callback when button close is clicked
 */
void MainWindow::onButtonCloseClick(TgMouseType type, float x, float y, const void *id)
{
    std::cout << "Closing the application\n";
    exit();
}

/*!
 * \brief MainWindow::onButtonAddedClick
 *
 * callback when button added is clicked
 */
void MainWindow::onButtonAddedClick(TgMouseType type, float x, float y, const void *id)
{
    std::cout << "Added button clicked\n";
}

/*!
 * \brief MainWindow::onButtonAddedCountClick
 *
 * callback when button count added
 */
void MainWindow::onButtonAddedCountClick(TgMouseType type, float x, float y, const void *id)
{
    setAllowedNumberMouseButtonCount(getAllowedNumberMouseButtonCount()+1);
    std::cout << "Added button count clicked " << getAllowedNumberMouseButtonCount() << "\n";
}

/*!
 * \brief MainWindow::onButtonReduceCountClick
 *
 * callback when button count reduced
 */
void MainWindow::onButtonReduceCountClick(TgMouseType type, float x, float y, const void *id)
{
    if (getAllowedNumberMouseButtonCount() > 0) {
        setAllowedNumberMouseButtonCount(getAllowedNumberMouseButtonCount()-1);
    }
    std::cout << "Reduce button count clicked " << getAllowedNumberMouseButtonCount() << "\n";
}
