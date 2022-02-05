#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <item2d/tg_button.h>

class MainWindow : public TgMainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();
private:
    TgMouseCapture m_mouseCaptureForSwipe;
    TgButton m_buttonClose;
    TgButton m_buttonUp;
    TgButton m_buttonDown;
    TgButton m_buttonLeft;
    TgButton m_buttonRight;
    bool m_swipeAvailableUp;
    bool m_swipeAvailableDown;
    bool m_swipeAvailableLeft;
    bool m_swipeAvailableRight;

    void setSwipeType();
    std::string generateSwipeTypeToString();

    void onSwipe(TgMouseType type, SwipeType swipeType, double time);
    void onButtonCloseClick(TgMouseType type, float x, float y);
    void onButtonUpClick(TgMouseType type, float x, float y);
    void onButtonDownClick(TgMouseType type, float x, float y);
    void onButtonLeftClick(TgMouseType type, float x, float y);
    void onButtonRightClick(TgMouseType type, float x, float y);
    void onButtonClick(TgMouseType type, float x, float y);
};

#endif
