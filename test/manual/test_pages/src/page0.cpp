#include "page0.h"
#include <iostream>
#include <item2d/tg_pages.h>
#include "mainwindow.h"

Page0::Page0(TgPages *parent, MainWindow *mainWindow) :
    TgPage(parent, 255, 0, 0),
    m_pages(parent),
    m_mainWindow(mainWindow),
    m_buttonChangeToPage1(this, 20, 20, 250, 50, "Change page to page 1"),
    m_buttonChangeToPage2(this, 290, 20, 250, 50, "Change page to page 2"),
    m_buttonClose(this, 20, 90, 200, 50, "Close button"),
    m_textFieldForTestBottomRight(this, "Bottom Right", "", 21, 0, 0, 0),
    m_textFieldForTestTopLeft(this, "Top Left", "", 21, 0, 0, 0)
{
    m_buttonChangeToPage1.connectOnMouseClicked( std::bind(&Page0::onButtonClickPage1, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonChangeToPage2.connectOnMouseClicked( std::bind(&Page0::onButtonClickPage2, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonClose.connectOnMouseClicked( std::bind(&Page0::onButtonCloseClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    this->connectOnVisibleChanged( std::bind(&Page0::onPageVisibleChanged, this, std::placeholders::_1) );
    m_buttonClose.connectOnVisibleChanged( std::bind(&Page0::onButtonCloseVisibleChanged, this, std::placeholders::_1) );

    m_buttonChangeToPage1.connectOnSelectedChanged( std::bind(&Page0::onButtonSelectedChangeToPage1Changed, this, std::placeholders::_1) );
    m_buttonChangeToPage2.connectOnSelectedChanged( std::bind(&Page0::onButtonSelectedChangeToPage2Changed, this, std::placeholders::_1) );
    m_buttonClose.connectOnSelectedChanged( std::bind(&Page0::onButtonSelectedCloseChanged, this, std::placeholders::_1) );
    m_textFieldForTestBottomRight.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignRight);
    m_textFieldForTestBottomRight.setVerticalAlign(TgTextfieldVerticalAlign::AlignBottom);
    m_textFieldForTestTopLeft.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignLeft);
    m_textFieldForTestTopLeft.setVerticalAlign(TgTextfieldVerticalAlign::AlignTop);
    m_textFieldForTestBottomRight.setMarginBottom(10);
    m_textFieldForTestBottomRight.setMarginRight(15);
    m_textFieldForTestTopLeft.setMarginLeft(10);
    m_textFieldForTestTopLeft.setMarginTop(5);
}

Page0::~Page0()
{
}

void Page0::onButtonClickPage1(TgMouseType type, float x, float y)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page 0 button to page 1\n";
    m_pages->setPage(1);
}

void Page0::onButtonClickPage2(TgMouseType type, float x, float y)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page 0 button to page 2\n";
    m_pages->setPage(2);
}

/*!
 * \brief Page0::onButtonCloseClick
 *
 * callback when button close is clicked
 */
void Page0::onButtonCloseClick(TgMouseType type, float x, float y)
{
    std::cout << "Closing the application\n";
    m_mainWindow->exit();
}

void Page0::onPageVisibleChanged(bool visible)
{
    std::cout << "Page0 visible changed: " << visible << "\n";
}

void Page0::onButtonCloseVisibleChanged(bool visible)
{
    std::cout << "Page0 Button close visible changed: " << visible << "\n";
}

void Page0::onButtonSelectedChangeToPage1Changed(bool selected)
{
    std::cout << "Page0::ChangeToPage1 Button selected changed: " << selected << "\n";
}

void Page0::onButtonSelectedChangeToPage2Changed(bool selected)
{
    std::cout << "Page0::ChangeToPage2 Button selected changed: " << selected << "\n";
}

void Page0::onButtonSelectedCloseChanged(bool selected)
{
    std::cout << "Page0::Close Button selected changed: " << selected << "\n";
}
