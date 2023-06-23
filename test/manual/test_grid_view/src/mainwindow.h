#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <item2d/tg_button.h>
#include <item2d/tg_grid_view.h>


class MainWindow : public TgMainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();
private:
    int m_stepButtonIndex = 0;
    TgButton m_buttonClose;
    TgButton m_buttonStep;
    TgGridView m_gridview;

    void onButtonCloseClick(TgMouseType type, float x, float y, const void *);
    void onButtonStepClick(TgMouseType type, float x, float y, const void *);
};

#endif
