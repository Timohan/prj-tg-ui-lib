#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <mutex>
#include "mouse_capture_area.h"


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

    MouseCaptureArea *getMouseCaptureArea(size_t i);
    std::string getString(HoverVisibleChangeState state);

private:
    std::mutex m_mutex;
    MouseCaptureArea m_mouseCapture0;
    MouseCaptureArea m_mouseCapture1;
    MouseCaptureArea m_mouseCapture2;
    MouseCaptureArea m_mouseCapture3;

    MouseCaptureArea m_mouseCapture4;
    MouseCaptureArea m_mouseCapture4on0;
    MouseCaptureArea m_mouseCapture4on1;

    std::vector<MouseStateChange> m_listMouseStateChange;

};

#endif
