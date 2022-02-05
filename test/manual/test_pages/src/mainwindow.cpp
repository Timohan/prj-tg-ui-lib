#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "TestApp1"),
    m_pages(this),
    m_page0(&m_pages, this),
    m_page1(&m_pages),
    m_page2(&m_pages)
{
}

MainWindow::~MainWindow()
{
}

/*!
 * \brief MainWindow::onButtonAddClick
 *
 * callback when button add is clicked
 */
void MainWindow::onButtonAddClick(TgMouseType type, float x, float y)
{
}

/*!
 * \brief MainWindow::onButtonRemoveClick
 *
 * callback when button remove is clicked
 */
void MainWindow::onButtonRemoveClick(TgMouseType type, float x, float y)
{
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
