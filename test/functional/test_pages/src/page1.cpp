#include "page1.h"
#include <iostream>
#include <item2d/tg_pages.h>

Page1::Page1(TgPages *parent) :
    TgPage(parent, 0, 255, 0),
    m_pages(parent),
    m_buttonChange(this, 20, 20, 250, 50, "Change page to page 0"),
    m_buttonChangeSwitchTime(this, 20, 90, 250, 50, "Page change time to 0.25"),
    m_buttonChangeSwitchTimeToSecond(this, 20, 160, 250, 50, "Page change time to 1")
{
    m_buttonChange.connectOnMouseClicked( std::bind(&Page1::onButtonClickPage0, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    m_buttonChangeSwitchTime.connectOnMouseClicked( std::bind(&Page1::onButtonPageChangeTimeToDefault, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    m_buttonChangeSwitchTimeToSecond.connectOnMouseClicked( std::bind(&Page1::onButtonPageChangeTimeToSecond, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    this->connectOnVisibleChanged( std::bind(&Page1::onPageVisibleChanged, this, std::placeholders::_1) );

    m_buttonChange.connectOnSelectedChanged( std::bind(&Page1::onButtonChangeSelectedChanged, this, std::placeholders::_1) );
    m_buttonChangeSwitchTime.connectOnSelectedChanged( std::bind(&Page1::onButtonChangeSwitchTimeSelectedChanged, this, std::placeholders::_1) );
    m_buttonChangeSwitchTimeToSecond.connectOnSelectedChanged( std::bind(&Page1::onButtonChangeSwitchTimeToSecondSelectedChanged, this, std::placeholders::_1) );
}

Page1::~Page1()
{
}

void Page1::onButtonClickPage0(TgMouseType type, float x, float y, const void *)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page 1 button to page 0\n";
    m_pages->setPage(0);
}

void Page1::onButtonPageChangeTimeToDefault(TgMouseType type, float x, float y, const void *)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Page change time to 0.25\n";
    m_pages->setPageSwitchTime(0.25);
}

void Page1::onButtonPageChangeTimeToSecond(TgMouseType type, float x, float y, const void *)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Page change time to 1\n";
    m_pages->setPageSwitchTime(1);
}

void Page1::onPageVisibleChanged(bool visible)
{
    std::cout << "Page1 visible changed: " << visible << "\n";
}

void Page1::onButtonChangeSelectedChanged(bool selected)
{
    std::cout << "Page1::Change page Button selected changed: " << selected << "\n";
}

void Page1::onButtonChangeSwitchTimeSelectedChanged(bool selected)
{
    std::cout << "Page1::ChangeSwitchTime Button selected changed: " << selected << "\n";
}

void Page1::onButtonChangeSwitchTimeToSecondSelectedChanged(bool selected)
{
    std::cout << "Page1::ChangeSwitchTimeToSecond button selected changed: " << selected << "\n";
}

TgButton *Page1::getButton(int index)
{
    switch (index) {
        case 0: return &m_buttonChange;
        case 1: return &m_buttonChangeSwitchTime;
        case 2: return &m_buttonChangeSwitchTimeToSecond;
        default:
            break;
    }
    return nullptr;
}
