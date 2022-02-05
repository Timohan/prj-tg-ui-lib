#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "TestApp1"),
    m_buttonClose(this, 20, 20, 200, 50, "Close button"),
    m_sliderHorizontal(this, 20, 100, 400, 20, TgSliderType::SliderType_Horizontal),
    m_sliderVertical(this, 20, 140, 20, 200, TgSliderType::SliderType_Vertical),
    m_buttonAddHorizontalMaxPosition(this, 240, 20, 100, 50, "Add"),
    m_buttonReduceHorizontalMaxPosition(this, 380, 20, 100, 50, "Reduce")
{
    m_buttonClose.connectOnMouseClicked( std::bind(&MainWindow::onButtonCloseClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );

    m_buttonAddHorizontalMaxPosition.connectOnMouseClicked( std::bind(&MainWindow::onButtonAddHorizontalClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonReduceHorizontalMaxPosition.connectOnMouseClicked( std::bind(&MainWindow::onButtonReduceHorizontalClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );

    m_sliderHorizontal.connectOnSliderPositionChanged( std::bind(&MainWindow::onSliderHorizontalPositionChanged, this, std::placeholders::_1) );
    m_sliderVertical.connectOnSliderPositionChanged( std::bind(&MainWindow::onSliderVerticalPositionChanged, this, std::placeholders::_1) );
    m_sliderHorizontal.connectOnSliderMaxPositionChanged( std::bind(&MainWindow::onSliderHorizontalMaxPositionChanged, this, std::placeholders::_1) );
    m_sliderVertical.connectOnSliderMaxPositionChanged( std::bind(&MainWindow::onSliderVerticalMaxPositionChanged, this, std::placeholders::_1) );
}

MainWindow::~MainWindow()
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

void MainWindow::onButtonAddHorizontalClick(TgMouseType type, float x, float y)
{
    m_sliderHorizontal.setSliderMaxPosition(m_sliderHorizontal.getSliderMaxPosition()+1);
}

void MainWindow::onButtonReduceHorizontalClick(TgMouseType type, float x, float y)
{
    if (m_sliderHorizontal.getSliderMaxPosition()) {
        m_sliderHorizontal.setSliderMaxPosition(m_sliderHorizontal.getSliderMaxPosition()-1);
    }
}
void MainWindow::onSliderHorizontalPositionChanged(uint64_t position)
{
    std::cout << "Horizontal position changed: " << position << "\n";
}

void MainWindow::onSliderVerticalPositionChanged(uint64_t position)
{
    std::cout << "Vertical position changed: " << position << "\n";
}

void MainWindow::onSliderHorizontalMaxPositionChanged(uint64_t maxPosition)
{
    std::cout << "Horizontal max position changed: " << maxPosition << "\n";
}

void MainWindow::onSliderVerticalMaxPositionChanged(uint64_t maxPosition)
{
    std::cout << "Vertical max position changed: " << maxPosition << "\n";
}

