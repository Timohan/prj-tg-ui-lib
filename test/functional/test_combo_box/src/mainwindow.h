#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <mutex>
#include <item2d/tg_combo_box.h>
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
    ComboBoxItemChanged,
};

struct MouseStateChange {
    MouseStateChange(size_t index, HoverVisibleChangeState state);
    size_t m_index;
    HoverVisibleChangeState m_state;
    int m_x, m_y;
    bool m_area;
    size_t m_itemIndex = 0;
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
    bool isSelected(size_t index);
    MouseStateChange *getMouseStateChange(size_t i);

    std::string getString(HoverVisibleChangeState state);

    void setMakeStep(int index);

private:
    std::mutex m_mutex;
    TgComboBox m_comboBoxTopLeft;
    TgComboBox m_comboBoxTopRight;
    TgComboBox m_comboBoxBottomLeft;
    TgComboBox m_comboBoxBottomRight;

    std::vector<MouseStateChange> m_listMouseStateChange;

    void onComboBoxTopLeftItemChanged(size_t index);
    void onComboBoxTopRightItemChanged(size_t index);
    void onComboBoxBottomLeftItemChanged(size_t index);
};

#endif
