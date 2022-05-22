#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <item2d/tg_button.h>
#include <item2d/tg_textedit.h>
#include <item2d/tg_rectangle.h>

class MainWindow : public TgMainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();
private:
    int m_centerTextIndex;
    TgRectangle m_background;
    TgButton m_buttonClose;
    TgButton m_buttonChangeText;
    TgTextedit m_textEdit;

    void onButtonCloseClick(TgMouseType type, float x, float y);
    void onButtonChangeTextClick(TgMouseType type, float x, float y);
};

#endif
