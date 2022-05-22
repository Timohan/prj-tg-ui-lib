#include "page3.h"
#include <iostream>
#include <item2d/tg_pages.h>

Page3::Page3(TgPages *parent) :
    TgPage(parent, 0, 255, 255),
    m_pages(parent),
    m_buttonChangePage0(this, 20, 20, 250, 50, "Change page to page 0"),
    m_buttonChangeText(this, 290, 20, 250, 50, "Change text"),
    m_textEdit(this, 20, 90, 250, 50, "", nullptr, 21)
{
    m_buttonChangePage0.connectOnMouseClicked( std::bind(&Page3::onButtonClickPage0, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonChangeText.connectOnMouseClicked( std::bind(&Page3::onButtonClickChangeText, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );

}

Page3::~Page3()
{
}

void Page3::onButtonClickPage0(TgMouseType type, float x, float y)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page 3 button to page 0\n";
    m_pages->setPage(0);
}

void Page3::onButtonClickChangeText(TgMouseType type, float x, float y)
{
    (void)type;
    (void)x;
    (void)y;
    std::vector<TgTextFieldText>listText;
    TgTextFieldText t0;
    TgTextFieldText t1;
    TgTextFieldText t2;
    t0.m_text = "a";
    t0.m_textColorR = t0.m_textColorG = t0.m_textColorB = 0;
    listText.push_back(t0);
    t1.m_text = "b";
    t1.m_textColorR = 255;
    t1.m_textColorG = t1.m_textColorB = 0;
    listText.push_back(t1);
    t2.m_text = "c";
    t2.m_textColorR = t2.m_textColorG = t2.m_textColorB = 0;
    listText.push_back(t2);
    m_textEdit.setText(listText);
}

TgTextedit *Page3::getTextEdit()
{
    return &m_textEdit;
}

