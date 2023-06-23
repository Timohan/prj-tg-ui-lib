#include "page2.h"
#include <iostream>
#include <item2d/tg_pages.h>

Page2::Page2(TgPages *parent) :
    TgPage(parent, 0, 0, 255),
    m_centerTextIndex(0),
    m_pages(parent),
    m_buttonChange(this, 20, 20, 250, 50, "Change page to page 0"),
    m_buttonChangeSwitchTypeDirect(this, 20, 90, 250, 50, "Change Switch to Direct"),
    m_buttonChangeSwitchTypeFromTop(this, 20, 160, 300, 50, "Change Switch to From up"),
    m_buttonChangeSwitchTypeFromBottom(this, 20, 250, 300, 50, "Change Switch to From bottom"),
    m_buttonChangeSwitchTypeFromLeft(this, 20, 320, 300, 50, "Change Switch to From left"),
    m_buttonChangeSwitchTypeFromRight(this, 20, 390, 300, 50, "Change Switch to From right"),
    m_buttonChangeText(this, 290, 20, 250, 50, "Change text"),
    m_textFieldForTestCenter(this, "Center text", "", 21, 255, 255, 255)
{
    m_buttonChange.connectOnMouseClicked( std::bind(&Page2::onButtonClickPage0, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    m_buttonChangeSwitchTypeDirect.connectOnMouseClicked( std::bind(&Page2::onButtonClickFromDirect, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    m_buttonChangeSwitchTypeFromTop.connectOnMouseClicked( std::bind(&Page2::onButtonClickFromTop, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    m_buttonChangeSwitchTypeFromBottom.connectOnMouseClicked( std::bind(&Page2::onButtonClickFromBottom, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    m_buttonChangeSwitchTypeFromLeft.connectOnMouseClicked( std::bind(&Page2::onButtonClickFromLeft, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    m_buttonChangeSwitchTypeFromRight.connectOnMouseClicked( std::bind(&Page2::onButtonClickFromRight, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    m_buttonChangeText.connectOnMouseClicked( std::bind(&Page2::onButtonChangeTextClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    this->connectOnVisibleChanged( std::bind(&Page2::onPageVisibleChanged, this, std::placeholders::_1) );

    m_buttonChange.connectOnSelectedChanged( std::bind(&Page2::onButtonChangeSelectedChanged, this, std::placeholders::_1) );
    m_buttonChangeSwitchTypeDirect.connectOnSelectedChanged( std::bind(&Page2::onButtonChangeSwitchTypeDirectSelectedChanged, this, std::placeholders::_1) );
    m_buttonChangeSwitchTypeFromTop.connectOnSelectedChanged( std::bind(&Page2::onButtonChangeSwitchTypeFromTopChanged, this, std::placeholders::_1) );
    m_buttonChangeSwitchTypeFromBottom.connectOnSelectedChanged( std::bind(&Page2::onButtonChangeSwitchTypeFromBottomChanged, this, std::placeholders::_1) );
    m_buttonChangeSwitchTypeFromLeft.connectOnSelectedChanged( std::bind(&Page2::onButtonChangeSwitchTypeFromLeftChanged, this, std::placeholders::_1) );
    m_buttonChangeSwitchTypeFromRight.connectOnSelectedChanged( std::bind(&Page2::onButtonChangeSwitchTypeFromRightChanged, this, std::placeholders::_1) );

    m_textFieldForTestCenter.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignCenterH);
    m_textFieldForTestCenter.setVerticalAlign(TgTextfieldVerticalAlign::AlignCenterV);
}

Page2::~Page2()
{
}

void Page2::onButtonClickPage0(TgMouseType type, float x, float y, const void *)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page 2 button to page 0\n";
    m_pages->setPage(0);
}

void Page2::onButtonClickFromDirect(TgMouseType type, float x, float y, const void *)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page switch to direct\n";
    m_pages->setPageSwitchType(TgPagesPageSwitchType::PageSwitchType_Direct);
}

void Page2::onButtonClickFromTop(TgMouseType type, float x, float y, const void *)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page switch from top\n";
    m_pages->setPageSwitchType(TgPagesPageSwitchType::PageSwitchType_SlideFromTop);
}

void Page2::onButtonClickFromBottom(TgMouseType type, float x, float y, const void *)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page switch from bottom\n";
    m_pages->setPageSwitchType(TgPagesPageSwitchType::PageSwitchType_SlideFromBottom);
}

void Page2::onButtonClickFromLeft(TgMouseType type, float x, float y, const void *)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page switch from left\n";
    m_pages->setPageSwitchType(TgPagesPageSwitchType::PageSwitchType_SlideFromLeft);
}

void Page2::onButtonClickFromRight(TgMouseType type, float x, float y, const void *)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page switch from right\n";
    m_pages->setPageSwitchType(TgPagesPageSwitchType::PageSwitchType_SlideFromRight);
}

void Page2::onPageVisibleChanged(bool visible)
{
    std::cout << "Page2 visible changed: " << visible << "\n";
}

void Page2::onButtonChangeSelectedChanged(bool selected)
{
    std::cout << "Page2::Change page from page 2 Button selected changed: " << selected << "\n";
}

void Page2::onButtonChangeSwitchTypeDirectSelectedChanged(bool selected)
{
    std::cout << "Page2::ChangeSwitchTypeDirect Button selected changed: " << selected << "\n";
}

void Page2::onButtonChangeSwitchTypeFromTopChanged(bool selected)
{
    std::cout << "Page2::ChangeSwitchTypeFromTop button selected changed: " << selected << "\n";
}

void Page2::onButtonChangeSwitchTypeFromBottomChanged(bool selected)
{
    std::cout << "Page2::ChangeSwitchTypeFromBottom button selected changed: " << selected << "\n";
}

void Page2::onButtonChangeSwitchTypeFromLeftChanged(bool selected)
{
    std::cout << "Page2::ChangeSwitchTypeFromLeft button selected changed: " << selected << "\n";
}

void Page2::onButtonChangeSwitchTypeFromRightChanged(bool selected)
{
    std::cout << "Page2::ChangeSwitchTypeFromRight button selected changed: " << selected << "\n";
}

TgButton *Page2::getButton(int index)
{
    switch (index) {
        case 0: return &m_buttonChange;
        case 1: return &m_buttonChangeSwitchTypeDirect;
        case 2: return &m_buttonChangeSwitchTypeFromTop;
        case 3: return &m_buttonChangeSwitchTypeFromBottom;
        case 4: return &m_buttonChangeSwitchTypeFromLeft;
        case 5: return &m_buttonChangeSwitchTypeFromRight;
        default:
            break;
    }
    return nullptr;
}

TgTextfield *Page2::getTextfield()
{
    return &m_textFieldForTestCenter;
}

float Page2::getAllDrawHeight(int index)
{
    return m_allDrawHeight[index];
}

/*!
 * \brief Page2::onButtonChangeTextClick
 *
 * callback when button added is clicked
 */
void Page2::onButtonChangeTextClick(TgMouseType type, float x, float y, const void *)
{
    std::cout << "Change text button clicked\n";
    std::vector<TgTextFieldText>listText;
    TgTextFieldText t0;
    TgTextFieldText t1;
    TgTextFieldText t2;
    m_centerTextIndex++;
    switch (m_centerTextIndex) {
        case 1:
            m_textFieldForTestCenter.setText("Changed text to \"1\"");
            break;
        case 2:
            m_textFieldForTestCenter.setText("Changed text to \"1\" with ગુજરાતી યુનિકોડ ફોન્ટ");
            break;
        case 3:
            m_textFieldForTestCenter.setText("Changed text to \"1\" with ગુજરાતી યુનિકોડ ફોન્ટ and 未来の文字コ");
            break;
        case 4:
            m_textFieldForTestCenter.setText("Changed text to \"1\" with ગુજરાતી યુનિકોડ ફોન્ટ and 未来の文字コ and some more");
            break;
        case 5:
            m_textFieldForTestCenter.setText("ગુજરાતી યુનિકોડ ફોન્ટ");
            break;
        case 6:
            m_textFieldForTestCenter.setText("中国");
            break;
        case 7:
            m_textFieldForTestCenter.setText("一些中文文本 language");
            break;
        case 8:
            t0.m_text = "Changed text to";
            t0.m_textColorR = t0.m_textColorG = t0.m_textColorB = 255;
            listText.push_back(t0);
            t1.m_text = " color";
            t1.m_textColorR = 255;
            t1.m_textColorG = t1.m_textColorB = 0;
            listText.push_back(t1);
            t2.m_text = " and here";
            t2.m_textColorR = t2.m_textColorG = t2.m_textColorB = 255;
            listText.push_back(t2);
            m_textFieldForTestCenter.setText(listText);
            listText[0].m_text = "button";
            m_buttonChangeText.setText(listText);
            break;
        case 9:
            t0.m_text = "Changed text to";
            t0.m_textColorR = t0.m_textColorG = t0.m_textColorB = 255;
            listText.push_back(t0);
            t1.m_text = " color";
            t1.m_textColorR = t1.m_textColorG = t1.m_textColorB = 255;
            listText.push_back(t1);
            t2.m_text = " and here";
            t2.m_textColorR = t2.m_textColorG = t2.m_textColorB = 255;
            listText.push_back(t2);
            m_textFieldForTestCenter.setText(listText);
            listText[0].m_text = "button";
            m_buttonChangeText.setText(listText);
            break;
        case 10:
            t0.m_text = "x text to";
            t0.m_textColorR = t0.m_textColorG = t0.m_textColorB = 255;
            listText.push_back(t0);
            t1.m_text = " યુનિકોડ";
            t1.m_textColorG = 255;
            t1.m_textColorR = t1.m_textColorB = 0;
            listText.push_back(t1);
            t2.m_text = " 未来";
            t2.m_textColorR = 255;
            t2.m_textColorG = t2.m_textColorB = 0;
            listText.push_back(t2);
            m_textFieldForTestCenter.setText(listText);
            listText[0].m_text = "button";
            m_buttonChangeText.setText(listText);
           break;
        case 11:
            m_textFieldForTestCenter.setText("");
            m_buttonChangeText.setText("");
            break;
        case 12:
            m_textFieldForTestCenter.setMaxLineCount(2);
            t0.m_text = "multiline text";
            t0.m_textColorR = t0.m_textColorG = t0.m_textColorB = 255;
            listText.push_back(t0);
            t1.m_text = " wqdqw aswqdw";
            t1.m_textColorG = t1.m_textColorB = 255;
            t1.m_textColorR = 0;
            listText.push_back(t1);
            t2.m_text = " and here is very long text to make this multiline text for testing and\n perhaps more testing";
            t2.m_textColorR = t2.m_textColorG = t2.m_textColorB = 255;
            listText.push_back(t2);
            m_textFieldForTestCenter.setText(listText);
            break;
        case 13:
            m_textFieldForTestCenter.setMarginLeft(50);
            m_textFieldForTestCenter.setMarginRight(10);
            break;
        case 14:
            m_textFieldForTestCenter.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignRight);
            m_textFieldForTestCenter.setVerticalAlign(TgTextfieldVerticalAlign::AlignBottom);
            break;
        case 15:
            m_textFieldForTestCenter.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignLeft);
            m_textFieldForTestCenter.setVerticalAlign(TgTextfieldVerticalAlign::AlignTop);
            break;
        case 16:
            m_textFieldForTestCenter.setMarginLeft(0);
            m_textFieldForTestCenter.setMarginRight(0);
            m_textFieldForTestCenter.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignCenterH);
            m_textFieldForTestCenter.setVerticalAlign(TgTextfieldVerticalAlign::AlignCenterV);
            break;
        case 17:
            m_textFieldForTestCenter.setMaxLineCount(3);
            break;
        case 18:
            t0.m_text = "xmultiline text";
            t0.m_textColorR = t0.m_textColorG = t0.m_textColorB = 255;
            listText.push_back(t0);
            t1.m_text = " wqdqw aswqdw";
            t1.m_textColorG = t1.m_textColorB = 255;
            t1.m_textColorR = 0;
            listText.push_back(t1);
            t2.m_text = " and here is very long text to make this\n multiline\n text for\n testing and\n perhaps more testing";
            t2.m_textColorR = t2.m_textColorG = t2.m_textColorB = 255;
            listText.push_back(t2);
            m_textFieldForTestCenter.setText(listText);
            m_buttonChangeText.setText(listText);
            break;
        case 19:
            m_textFieldForTestCenter.setMaxLineCount(0);
            break;
        case 20:
            m_textFieldForTestCenter.setMaxLineCount(2);
            break;
        case 21:
            m_textFieldForTestCenter.setAllowBreakLineGoOverMaxLine(true);
            m_buttonChangeText.setText("Change text");
            break;
        case 22:
            t0.m_text = "xmultiline text";
            t0.m_textColorR = t0.m_textColorG = t0.m_textColorB = 255;
            listText.push_back(t0);
            t1.m_text = " wqdqw aswqdw";
            t1.m_textColorG = t1.m_textColorB = 255;
            t1.m_textColorR = 0;
            listText.push_back(t1);
            t2.m_text = " and here is very long text to make this\nmultiline\ntext for\ntesting and\nperhaps more testing";
            t2.m_textColorR = t2.m_textColorG = t2.m_textColorB = 255;
            listText.push_back(t2);
            m_textFieldForTestCenter.setText(listText);
            m_textFieldForTestCenter.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignRight);
            m_textFieldForTestCenter.setVerticalAlign(TgTextfieldVerticalAlign::AlignBottom);
            break;
        case 23:
            m_textFieldForTestCenter.setMaxLineCount(3);
            break;
        case 24:
            m_textFieldForTestCenter.setText("xmultiline text for testing and more testing wqdqw aswqdw and here is very long text to make this\nmultiline\ntext for\ntesting and\nperhaps more testing");
            m_textFieldForTestCenter.setMaxLineCount(2);
            break;
        case 25:
            m_textFieldForTestCenter.setMaxLineCount(3);
            break;
        case 26:
            m_textFieldForTestCenter.setMaxLineCount(1);
            m_textFieldForTestCenter.setAllowBreakLineGoOverMaxLine(false);
            m_textFieldForTestCenter.setWordWrap(TgTextFieldWordWrap::WordWrapOff);
            break;
        case 27:
            m_textFieldForTestCenter.setAllowBreakLineGoOverMaxLine(true);
            break;
        case 28:
            m_textFieldForTestCenter.setMaxLineCount(0);
            break;
        case 29:
            m_textFieldForTestCenter.setAllowBreakLineGoOverMaxLine(false);
            break;
        case 30:
            m_textFieldForTestCenter.setMaxLineCount(2);
            break;
        case 31:
            m_textFieldForTestCenter.setAllowBreakLineGoOverMaxLine(true);
            break;
        case 32:
            m_textFieldForTestCenter.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignLeft);
            m_textFieldForTestCenter.setVerticalAlign(TgTextfieldVerticalAlign::AlignTop);
            break;
        case 33:
            m_textFieldForTestCenter.setMaxLineCount(1);
            m_textFieldForTestCenter.setAllowBreakLineGoOverMaxLine(true);
            m_textFieldForTestCenter.setWordWrap(TgTextFieldWordWrap::WordWrapOn);
            break;
        case 34:
            m_textFieldForTestCenter.setMaxLineCount(2);
            m_textFieldForTestCenter.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignRight);
            m_textFieldForTestCenter.setVerticalAlign(TgTextfieldVerticalAlign::AlignBottom);
            break;
        case 35:
            m_textFieldForTestCenter.setMaxLineCount(0);
            break;
        case 36:
            m_textFieldForTestCenter.setText("xmultilineatextaforatestingaandamoreatestingawqdqwaaswqdwaandahereaisaveryalongatextatoamake this\nmultiline\ntext for\ntesting and\nperhaps more testing");
            break;
        case 37:
            m_textFieldForTestCenter.setMaxLineCount(2);
            m_textFieldForTestCenter.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignLeft);
            m_textFieldForTestCenter.setVerticalAlign(TgTextfieldVerticalAlign::AlignTop);
            m_textFieldForTestCenter.setText("dwqwqddqdwdwqdqwqwwqwdqwdwqwddqwdqwdwqwddqwdqwqwddqwdwqxmultilineatextaforatestingaandamoreatestingawqdqwaaswqdwaandahereaisaveryalongatextatoamake this\nmultiline\ntext for\ntesting and\nperhaps more testing");
            break;
        default:
            m_textFieldForTestCenter.setMaxLineCount(1);
            m_textFieldForTestCenter.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignCenterH);
            m_textFieldForTestCenter.setVerticalAlign(TgTextfieldVerticalAlign::AlignCenterV);
            m_textFieldForTestCenter.setText("Change text");
            m_buttonChangeText.setText("Change text");
            m_centerTextIndex = 0;
            break;
    }
    setDrawHeight(m_centerTextIndex, m_textFieldForTestCenter.getAllDrawTextHeight());
}

void Page2::setDrawHeight(int centerTextIndex, float value)
{
    while (1) {
        if (static_cast<size_t>(centerTextIndex)+1 > m_allDrawHeight.size()) {
            m_allDrawHeight.push_back(0);
        } else {
            break;
        }
    }
    m_allDrawHeight[centerTextIndex] = value;
}