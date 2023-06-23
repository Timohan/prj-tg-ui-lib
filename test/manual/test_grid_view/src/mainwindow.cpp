#include "mainwindow.h"
#include <iostream>
#include <item2d/tg_grid_view_cell.h>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "TestApp1", width - 300, height - 300),
    m_buttonClose(this, 20, 20, 200, 50, "Close button"),
    m_buttonStep(this, 250, 20, 100, 50, "Step"),
    m_gridview(this, 20, 80, 500, 400, 40, 40)
{
    m_buttonClose.connectOnMouseClicked( std::bind(&MainWindow::onButtonCloseClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    m_buttonStep.connectOnMouseClicked( std::bind(&MainWindow::onButtonStepClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    size_t x, y;
    for (x=0;x<40;x++) {
        for (y=0;y<40;y++) {
            m_gridview.getCell(x, y)->setText(std::string( std::to_string(x) + "/" + std::to_string(y) ).c_str() );
        }
    }
}

MainWindow::~MainWindow()
{
}

/*!
 * \brief MainWindow::onButtonCloseClick
 *
 * callback when button close is clicked
 */
void MainWindow::onButtonCloseClick(TgMouseType type, float x, float y, const void *)
{
    std::cout << "Closing the application\n";
    exit();
}

void MainWindow::onButtonStepClick(TgMouseType type, float x, float y, const void *)
{
    std::cout << "onButtonStepClick\n";
    switch (m_stepButtonIndex) {
        case 0:
            m_gridview.setRowCount(35);
            break;
        case 1:
            m_gridview.setColumCount(34);
            break;
        case 2:
            m_gridview.setRowCount(45);
            break;
        case 3:
            m_gridview.setColumCount(54);
            break;
        case 4:
            m_gridview.setRowCount(35);
            break;
        case 5:
            m_gridview.setColumCount(34);
            break;
        default:
            break;
    }
    m_stepButtonIndex++;
}
