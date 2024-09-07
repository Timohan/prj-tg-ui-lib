#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <item2d/tg_rectangle.h>
#include <item2d/tg_button.h>


class MainWindow : public TgMainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();

    void setMakeStep(int index);

private:
    TgRectangle m_rectangle0;
    TgRectangle m_rectangle0_0;
    TgRectangle m_rectangle0_0_0;
    TgItem2d m_item1;
    TgButton m_button1;

};

#endif
