#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <mutex>
#include <item2d/tg_grid_view.h>
#include <item2d/tg_grid_view_cell.h>
#include <item2d/tg_button.h>
#include <item2d/tg_rectangle.h>


class TgTextedit;

enum HoverVisibleChangeState
{
    HoverEnable = 0,
    HoverDisable,
    Visible,
    Invisible,
    Enabled,
    Disabled,
    MouseMove,
    MousePress,
    MouseRelease,
    MouseClicked,
};

struct MouseStateChange {
    MouseStateChange(size_t index, HoverVisibleChangeState state);
    size_t m_index;
    HoverVisibleChangeState m_state;
    int m_x, m_y;
    bool m_area;
};

class MainWindow : public TgMainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();

    void addHoverChange(size_t index, bool hover);
    void addVisibleChange(size_t index, bool visible);
    void addEnabledChange(size_t index, bool enabled);
    void addMouseStateChange(const MouseStateChange &change);
    size_t getMouseStateChangeCount();
    MouseStateChange *getMouseStateChange(size_t i);

    std::string getString(HoverVisibleChangeState state);

    bool setMakeStep(int index);

private:
    std::mutex m_mutex;
    TgRectangle m_background;
    TgButton m_buttonTopLeft;
    TgButton m_buttonTopRight;
    TgButton m_buttonBottomLeft;
    TgButton m_buttonBottomRight;
    TgRectangle *m_backgroundOnBg = nullptr;

    std::vector<MouseStateChange> m_listMouseStateChange;

};

#endif
