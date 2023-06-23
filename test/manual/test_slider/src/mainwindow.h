#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <item2d/tg_button.h>
#include <item2d/tg_slider.h>

class MainWindow : public TgMainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();
private:
    TgButton m_buttonClose;
    TgSlider m_sliderHorizontal;
    TgSlider m_sliderVertical;

    TgButton m_buttonAddHorizontalMaxPosition;
    TgButton m_buttonReduceHorizontalMaxPosition;

    void onButtonCloseClick(TgMouseType type, float x, float y, const void *);
    void onButtonAddHorizontalClick(TgMouseType type, float x, float y, const void *);
    void onButtonReduceHorizontalClick(TgMouseType type, float x, float y, const void *);

    void onSliderHorizontalPositionChanged(uint64_t position);
    void onSliderVerticalPositionChanged(uint64_t position);
    void onSliderHorizontalMaxPositionChanged(uint64_t maxPosition);
    void onSliderVerticalMaxPositionChanged(uint64_t maxPosition);
};

#endif
