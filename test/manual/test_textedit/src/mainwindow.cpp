#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "TestApp1", width - 300, height - 300),
    m_centerTextIndex(0),
    m_background(this),
    m_buttonClose(this, 20, 20, 200, 50, "Close button"),
    m_buttonChangeText(this, 240, 20, 200, 50, "Change text"),
    m_textEdit(this, 20, 100, 200, 30, "Center text", "", 21, 0, 0, 0),
    m_textEditSecondary(this, 20, 140, 200, 30, "", "", 21, 0, 0, 0)
{
    m_buttonClose.connectOnMouseClicked( std::bind(&MainWindow::onButtonCloseClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonChangeText.connectOnMouseClicked( std::bind(&MainWindow::onButtonChangeTextClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
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
            m_textEdit.setText("Changed text to \"1x\"");
            break;
        case 2:
            t0.m_text = "Changed text to";
            t0.m_textColorR = t0.m_textColorG = t0.m_textColorB = 0;
            listText.push_back(t0);
            t1.m_text = " color";
            t1.m_textColorR = 255;
            t1.m_textColorG = t1.m_textColorB = 0;
            listText.push_back(t1);
            t2.m_text = " and here";
            t2.m_textColorR = t2.m_textColorG = t2.m_textColorB = 0;
            listText.push_back(t2);
            m_textEdit.setText(listText);
            break;
        case 3:
            m_textEdit.setText("Changed text to \"1x\" with ગુજરાતી યુનિકોડ ફોન્ટ and 未来の文字コ");
            break;
        case 4:
            m_textEdit.setText("Changed text to \"1x\" with ગુજરાતી યુનિકોડ ફોન્ટ and 未来の文字コ and some more");
            break;
        case 5:
            m_textEdit.setText("ગુજરાતી યુનિકોડ ફોન્ટ");
            break;
        case 6:
            m_textEdit.setText("中国");
            break;
        case 7:
            m_textEdit.setText("一些中文文本 language");
            break;
        case 8:
            m_textEdit.setText("Changed text to \"1x\" with ગુજરાતી યુનિકોડ ફોન્ટ");
            break;
        case 9:
            t0.m_text = "Changed text to";
            t0.m_textColorR = t0.m_textColorG = t0.m_textColorB = 0;
            listText.push_back(t0);
            t1.m_text = " color";
            t1.m_textColorR = t1.m_textColorG = t1.m_textColorB = 0;
            listText.push_back(t1);
            t2.m_text = " and here";
            t2.m_textColorR = t2.m_textColorG = t2.m_textColorB = 0;
            listText.push_back(t2);
            m_textEdit.setText(listText);
            break;
        case 10:
            t0.m_text = "x text to";
            t0.m_textColorR = t0.m_textColorG = t0.m_textColorB = 0;
            listText.push_back(t0);
            t1.m_text = " યુનિકોડ";
            t1.m_textColorG = 255;
            t1.m_textColorR = t1.m_textColorB = 0;
            listText.push_back(t1);
            t2.m_text = " 未来";
            t2.m_textColorR = 255;
            t2.m_textColorG = t2.m_textColorB = 0;
            listText.push_back(t2);
            m_textEdit.setText(listText);
           break;
        default:
            m_textEdit.setText("Change text");
            m_centerTextIndex = 0;
            break;
    }
}