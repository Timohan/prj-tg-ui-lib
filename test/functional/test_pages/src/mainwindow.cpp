#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "TestPages", width-200, height-200, width+200, height+200),
    m_pages(this),
    m_page0(&m_pages, this),
    m_page1(&m_pages),
    m_page2(&m_pages),
    m_page3(&m_pages)
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

TgPages *MainWindow::getPages()
{
    return &m_pages;
}

TgButton *MainWindow::getButton(int page, int index)
{
    switch (page) {
        case 0: return m_page0.getButton(index);
        case 1: return m_page1.getButton(index);
        case 2: return m_page2.getButton(index);
        default:
            break;
    }
    return nullptr;
}

TgTextfield *MainWindow::getTextfield()
{
    return m_page0.getTextfield();
}

TgTextedit *MainWindow::getTextEdit()
{
    return m_page3.getTextEdit();
}

float MainWindow::getOpeningTextWidth()
{
    return m_page0.getOpeningTextWidth();
}

float MainWindow::getOpeningTextHeight()
{
    return m_page0.getOpeningTextHeight();
}
