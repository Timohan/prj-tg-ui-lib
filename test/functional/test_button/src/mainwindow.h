#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <mutex>
#include "button.h"


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

    Button *getButton(size_t i);
    std::string getString(HoverVisibleChangeState state);

private:
    std::mutex m_mutex;
    Button m_button0;
    Button m_button1;
    Button m_button2;
    Button m_button3;

    Button m_button4;
    Button m_button4on0;
    Button m_button4on1;

    std::vector<MouseStateChange> m_listMouseStateChange;

};

#endif
