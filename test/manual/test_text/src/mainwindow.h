#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <item2d/tg_button.h>
#include <item2d/tg_textfield.h>

class MainWindow : public TgMainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();
private:
    int m_centerTextIndex;
    TgButton m_buttonClose;
    TgButton m_buttonChangeText;
    TgTextfield m_textFieldForTestBottomRight;
    TgTextfield m_textFieldForTestCenter;

    void onButtonCloseClick(TgMouseType type, float x, float y);
    void onButtonChangeTextClick(TgMouseType type, float x, float y);
};

#endif
