#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <item2d/tg_button.h>
#include <item2d/tg_chart.h>

class MainWindow : public TgMainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();
private:
    TgButton m_buttonClose;
    TgButton m_buttonAdd;
    TgButton m_buttonRemove;
    TgButton m_buttonChangeBackground;
    TgButton m_buttonChangeGridColor;
    TgButton m_buttonChangeLineColor;
    TgButton m_buttonChangeTextColor;
    TgButton m_buttonChangeDrawStyle;

    TgButton m_buttonTopMarginAdd;
    TgButton m_buttonBottomMarginAdd;
    TgButton m_buttonLeftMarginAdd;
    TgButton m_buttonRightMarginAdd;

    TgChart m_chart;

    int m_backgroundIndex = 0;
    int m_gridColorIndex = 0;
    int m_lineColorIndex = 0;
    int m_textColorIndex = 0;
    int m_chartIndex = 0;

    void onButtonAddClick(TgMouseType type, float x, float y, const void *id);
    void onButtonCloseClick(TgMouseType type, float x, float y, const void *id);
    void onButtonRemoveClick(TgMouseType type, float x, float y, const void *id);
    void onButtonChangeBackgroundClick(TgMouseType type, float x, float y, const void *id);
    void onButtonChangeGridColorClick(TgMouseType type, float x, float y, const void *id);
    void onButtonChangeLineColorClick(TgMouseType type, float x, float y, const void *id);
    void onButtonChangeTextColorClick(TgMouseType type, float x, float y, const void *id);
    void onButtonChangeDrawStyleClick(TgMouseType type, float x, float y, const void *id);

    void onButtonTopMarginAddClick(TgMouseType type, float x, float y, const void *id);
    void onButtonBottomMarginAddClick(TgMouseType type, float x, float y, const void *id);
    void onButtonLeftMarginAddClick(TgMouseType type, float x, float y, const void *id);
    void onButtonRightMarginAddClick(TgMouseType type, float x, float y, const void *id);
};

#endif
