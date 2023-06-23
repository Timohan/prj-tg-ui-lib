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
    TgButton m_buttonClose;
    TgButton m_buttonAdd;
    TgButton m_buttonRemove;

    TgButton *m_buttonAdded;

    TgButton m_buttonAddCount;
    TgButton m_buttonReduceCount;

    void onButtonAddClick(TgMouseType type, float x, float y, const void *id);
    void onButtonCloseClick(TgMouseType type, float x, float y, const void *id);
    void onButtonAddedClick(TgMouseType type, float x, float y, const void *id);
    void onButtonRemoveClick(TgMouseType type, float x, float y, const void *id);

    void onButtonAddedCountClick(TgMouseType type, float x, float y, const void *id);
    void onButtonReduceCountClick(TgMouseType type, float x, float y, const void *id);
};

#endif
