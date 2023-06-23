#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <item2d/tg_button.h>
#include <item2d/tg_menu_top.h>

class MainWindow : public TgMainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();
private:
    TgMenuTop m_menuTop;
    TgButton m_buttonClose;
    TgButton m_buttonTopRight;
    TgButton m_buttonBottomLeft;
    TgButton m_buttonBottomRight;
    TgMenuItem *m_newItem;
    TgMenuItem *m_newBottomRightSub;

    void onButtonCloseClick(TgMouseType type, float x, float y, const void *);
    void onMenuItemClicked(TgMouseType type, float x, float y, const void *);
    void onNewMenuItemClicked(TgMouseType type, float x, float y, const void *);

    void onNewTopRightItemClicked(TgMouseType type, float x, float y, const void *);
    void onNewBottomLeftItemClicked(TgMouseType type, float x, float y, const void *);
    void onNewBottomRightItemClicked(TgMouseType type, float x, float y, const void *);
    void onNewBottomRight1ItemClicked(TgMouseType type, float x, float y, const void *);
    void onNewBottomRightSub5ItemClicked(TgMouseType type, float x, float y, const void *);
    void onNewBottomRightSub5sItemClicked(TgMouseType type, float x, float y, const void *);
};

#endif
