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
    switch (m_centerTextIndex) {
        case 1:
            m_textFieldForTestCenter.setText("Changed text to \"1x\"");
            break;
        case 2:
            m_textFieldForTestCenter.setText("Changed text to \"1x\" with ગુજરાતી યુનિકોડ ફોન્ટ");
            break;
        case 3:
            m_textFieldForTestCenter.setText("Changed text to \"1x\" with ગુજરાતી યુનિકોડ ફોન્ટ and 未来の文字コ");
            break;
        case 4:
            m_textFieldForTestCenter.setText("Changed text to \"1x\" with ગુજરાતી યુનિકોડ ફોન્ટ and 未来の文字コ and some more");
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
        default:
            m_textFieldForTestCenter.setText("Change text");
            m_centerTextIndex = 0;
            break;
    }
}