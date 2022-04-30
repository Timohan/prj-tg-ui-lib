#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "TestApp1", width - 300, height - 300),
    m_centerTextIndex(0),
    m_buttonClose(this, 20, 20, 200, 50, "Close button"),
    m_buttonChangeText(this, 240, 20, 200, 50, "Change text"),
    m_textFieldForTestBottomRight(this, "Bottom ગુજરાતી યુનિકોડ ફોન્ટ સૂચી 未来の文字コ Right", "", 21, 255, 255, 255),
    m_textFieldForTestCenter(this, "Center text", "", 21, 255, 255, 255)
{
    m_buttonClose.connectOnMouseClicked( std::bind(&MainWindow::onButtonCloseClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonChangeText.connectOnMouseClicked( std::bind(&MainWindow::onButtonChangeTextClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );

    m_textFieldForTestBottomRight.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignRight);
    m_textFieldForTestBottomRight.setVerticalAlign(TgTextfieldVerticalAlign::AlignBottom);
    m_textFieldForTestBottomRight.setMarginBottom(10);
    m_textFieldForTestBottomRight.setMarginRight(15);

    m_textFieldForTestCenter.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignCenterH);
    m_textFieldForTestCenter.setVerticalAlign(TgTextfieldVerticalAlign::AlignCenterV);

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

/*!
 * \brief MainWindow::onButtonChangeTextClick
 *
 * callback when button added is clicked
 */
void MainWindow::onButtonChangeTextClick(TgMouseType type, float x, float y)
{
    std::cout << "Change text button clicked\n";
    m_centerTextIndex++;
    std::vector<TgTextFieldText>listText;
    TgTextFieldText t0;
    TgTextFieldText t1;
    TgTextFieldText t2;
    switch (m_centerTextIndex) {
        case 1:
            m_textFieldForTestCenter.setText("Changed text to \"1x\"");
            m_buttonChangeText.setText("\"1x\"");
            break;
        case 2:
            m_textFieldForTestCenter.setText("Changed text to \"1x\" with ગુજરાતી યુનિકોડ ફોન્ટ");
            m_buttonChangeText.setText("ગુજરાતી યુનિકોડ ફોન્ટ");
            break;
        case 3:
            m_textFieldForTestCenter.setText("Changed text to \"1x\" with ગુજરાતી યુનિકોડ ફોન્ટ and 未来の文字コ");
            m_buttonChangeText.setText("and યુનિકોડ 未来");
            break;
        case 4:
            m_textFieldForTestCenter.setText("cc \"1x\" with ગુજરાતી યુનિકોડ ફોન્ટ and 未来の文字コ and some more");
            break;
        case 5:
            m_textFieldForTestCenter.setText("ગુજરાતી યુનિકોડ ફોન્ટ");
            break;
        case 6:
            m_textFieldForTestCenter.setText("中国");
            m_buttonChangeText.setText("中国");
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
        default:
            m_textFieldForTestCenter.setText("Change text");
            m_centerTextIndex = 0;
            break;
    }
}