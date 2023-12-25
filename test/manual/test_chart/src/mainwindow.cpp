#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "TestApp1", width - 300, height - 300),
    m_buttonClose(this, 20, 20, 200, 50, "Close button"),
    m_buttonAdd(this, 240, 20, 200, 50, "Add button"),
    m_buttonRemove(this, 460, 20, 200, 50, "Remove button"),
    m_buttonChangeBackground(this, 20, 90, 200, 50, "Change background"),
    m_buttonChangeGridColor(this, 240, 90, 200, 50, "Change grid color"),
    m_buttonChangeLineColor(this, 460, 90, 200, 50, "Change line color"),
    m_buttonChangeTextColor(this, 240, 230, 200, 50, "Change text color"),
    m_buttonChangeDrawStyle(this, 460, 230, 200, 50, "Change draw style"),
    m_buttonTopMarginAdd(this, 20, 160, 170, 50, "Add top margin"),
    m_buttonBottomMarginAdd(this, 200, 160, 180, 50, "Add bottom margin"),
    m_buttonLeftMarginAdd(this, 390, 160, 170, 50, "Add left margin"),
    m_buttonRightMarginAdd(this, 570, 160, 170, 50, "Add right margin"),
    m_chart(this, 10, 300, 200, 100, 255, 0)
{
    m_chart.setTextColor(255, 255, 255, 255);
    m_buttonClose.connectOnMouseClicked( std::bind(&MainWindow::onButtonCloseClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );
    m_buttonAdd.connectOnMouseClicked( std::bind(&MainWindow::onButtonAddClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), reinterpret_cast<const void *>("Adding id!"));
    m_buttonRemove.connectOnMouseClicked( std::bind(&MainWindow::onButtonRemoveClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );
    m_buttonChangeBackground.connectOnMouseClicked( std::bind(&MainWindow::onButtonChangeBackgroundClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );
    m_buttonChangeGridColor.connectOnMouseClicked( std::bind(&MainWindow::onButtonChangeGridColorClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );
    m_buttonChangeLineColor.connectOnMouseClicked( std::bind(&MainWindow::onButtonChangeLineColorClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );
    m_buttonChangeTextColor.connectOnMouseClicked( std::bind(&MainWindow::onButtonChangeTextColorClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );
    m_buttonChangeDrawStyle.connectOnMouseClicked( std::bind(&MainWindow::onButtonChangeDrawStyleClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );

    m_buttonTopMarginAdd.connectOnMouseClicked( std::bind(&MainWindow::onButtonTopMarginAddClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );
    m_buttonBottomMarginAdd.connectOnMouseClicked( std::bind(&MainWindow::onButtonBottomMarginAddClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );
    m_buttonLeftMarginAdd.connectOnMouseClicked( std::bind(&MainWindow::onButtonLeftMarginAddClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );
    m_buttonRightMarginAdd.connectOnMouseClicked( std::bind(&MainWindow::onButtonRightMarginAddClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), nullptr );
}

MainWindow::~MainWindow()
{
}

/*!
 * \brief MainWindow::onButtonAddClick
 *
 * callback when button add is clicked
 */
void MainWindow::onButtonAddClick(TgMouseType type, float x, float y, const void *id)
{
    std::cout << "Adding the button  - callback by button: " << type << " id: " << reinterpret_cast<const char *>(id) << "\n";
    TgChartPosition position;
    switch (m_chartIndex) {
        case 0: position.x = 0; position.y = 0; break;
        case 1: position.x = 5; position.y = 5; break;
        case 2: position.x = 6; position.y = 12; break;
        case 3: position.x = 25; position.y = 5; break;
        case 4: position.x = 30; position.y = -5; break;
        case 5: position.x = 31; position.y = 10; break;
        default:
            break;
    }
    m_chart.addChartPosition(position);
    m_chartIndex++;
}

/*!
 * \brief MainWindow::onButtonRemoveClick
 *
 * callback when button remove is clicked
 */
void MainWindow::onButtonRemoveClick(TgMouseType type, float x, float y, const void *id)
{
    std::cout << "Removing the button - callback by button: "  << type << "\n";
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
 * \brief MainWindow::onButtonChangeBackgroundClick
 *
 * callback when button change background button is clicked
 */
void MainWindow::onButtonChangeBackgroundClick(TgMouseType type, float x, float y, const void *id)
{
    std::cout << "Background change\n";
    m_backgroundIndex++;
    switch (m_backgroundIndex) {
        case 0:
            m_chart.setBackgroundColor(255, 255, 255);
            break;
        case 1:
            m_chart.setBackgroundColor(255, 0, 0);
            break;
        case 2:
            m_chart.setBackgroundColor(255, 0, 0, 125);
            break;
        default:
            m_backgroundIndex = -1;
            m_chart.setBackgroundColor(255, 0, 255);
            break;
    }
}

/*!
 * \brief MainWindow::onButtonChangeGridColorClick
 *
 * callback when button change background button is clicked
 */
void MainWindow::onButtonChangeGridColorClick(TgMouseType type, float x, float y, const void *id)
{
    std::cout << "Grid color change\n";
    m_gridColorIndex++;
    switch (m_gridColorIndex) {
        case 0:
            m_chart.setGridLinesColor(255, 255, 255);
            break;
        case 1:
            m_chart.setGridLinesColor(0, 0, 0);
            break;
        case 2:
            m_chart.setGridLinesColor(0, 0, 0, 125);
            break;
        default:
            m_gridColorIndex = -1;
            m_chart.setGridLinesColor(100, 100, 100);
            break;
    }
}

/*!
 * \brief MainWindow::onButtonChangeLineColorClick
 *
 * callback when button change background button is clicked
 */
void MainWindow::onButtonChangeLineColorClick(TgMouseType type, float x, float y, const void *id)
{
    std::cout << "Line color change\n";
    m_lineColorIndex++;
    switch (m_lineColorIndex) {
        case 0:
            m_chart.setLineColor(255, 255, 255);
            break;
        case 1:
            m_chart.setLineColor(0, 0, 0);
            break;
        case 2:
            m_chart.setLineColor(0, 0, 0, 125);
            break;
        default:
            m_lineColorIndex = -1;
            m_chart.setLineColor(100, 100, 100);
            break;
    }
}

/*!
 * \brief MainWindow::onButtonChangeTextColorClick
 *
 * callback when button change background button is clicked
 */
void MainWindow::onButtonChangeTextColorClick(TgMouseType type, float x, float y, const void *id)
{
    std::cout << "Text color change\n";
    m_textColorIndex++;
    switch (m_textColorIndex) {
        case 0:
            m_chart.setTextColor(255, 255, 255);
            break;
        case 1:
            m_chart.setTextColor(0, 0, 0);
            break;
        case 2:
            m_chart.setTextColor(255, 255, 255, 125);
            break;
        default:
            m_textColorIndex = -1;
            m_chart.setTextColor(100, 100, 100);
            break;
    }
}

/*!
 * \brief MainWindow::onButtonChangeDrawStyleClick
 *
 * callback when xy draw style button is clicked
 */
void MainWindow::onButtonChangeDrawStyleClick(TgMouseType type, float x, float y, const void *id)
{
    std::cout << "Change xy draw style to " << !m_chart.getXyDrawIsMaxedToSize() << "\n";
    m_chart.setXyDrawIsMaxedToSize(!m_chart.getXyDrawIsMaxedToSize());
}

void MainWindow::onButtonTopMarginAddClick(TgMouseType type, float x, float y, const void *id)
{
    std::cout << "Top margin change\n";
    m_chart.setTopGridMargin(m_chart.getTopGridMargin()+1);
}
void MainWindow::onButtonBottomMarginAddClick(TgMouseType type, float x, float y, const void *id)
{
    std::cout << "Bottom margin change\n";
    m_chart.setBottomGridMargin(m_chart.getBottomGridMargin()+1);
}
void MainWindow::onButtonLeftMarginAddClick(TgMouseType type, float x, float y, const void *id)
{
    std::cout << "Left margin change\n";
    m_chart.setLeftGridMargin(m_chart.getLeftGridMargin()+1);
}
void MainWindow::onButtonRightMarginAddClick(TgMouseType type, float x, float y, const void *id)
{
    std::cout << "Right margin change\n";
    m_chart.setRightGridMargin(m_chart.getRightGridMargin()+1);
}
