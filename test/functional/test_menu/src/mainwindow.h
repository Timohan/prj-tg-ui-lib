#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <mutex>
#include "button.h"
#include <item2d/tg_menu_top.h>

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
    MouseMenuClicked,
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
    TgItem2d *getItem2d(size_t i);
    std::string getString(HoverVisibleChangeState state);
    void removeMenu(size_t i, size_t indexToRemove);

private:
    std::mutex m_mutex;
    TgMenuTop m_menuTop;
    Button m_button0;
    Button m_button1;
    Button m_button2;
    Button m_button3;
    TgMenuItem *m_bottomRightMenu = nullptr;
    TgMenuItem *m_larger = nullptr;
    TgMenuItem *m_expandMenu = nullptr;
    TgMenuItem *m_smaller = nullptr;
    TgMenuItem *m_somethingElse = nullptr;
    TgMenuItem *m_topLeft0 = nullptr;
    TgMenuItem *m_topLeft1 = nullptr;
    TgMenuItem *m_topLeft2 = nullptr;
    TgMenuItem *m_topLeft20 = nullptr;
    TgMenuItem *m_topLeft21 = nullptr;
    TgMenuItem *m_topLeft22 = nullptr;
    TgMenuItem *m_fileItem = nullptr;
    TgMenuItem *m_editItem = nullptr;
    TgMenuItem *m_selectionItem = nullptr;
    TgMenuItem *m_helpItem = nullptr;


    std::vector<MouseStateChange> m_listMouseStateChange;
    void onMenuNewClick(TgMouseType type, float x, float y);
    void onMenuSaveClick(TgMouseType type, float x, float y);
    void onMenuOpenClick(TgMouseType type, float x, float y);
    void onMenuCopyClick(TgMouseType type, float x, float y);
    void onMenuBottomRight2Click(TgMouseType type, float x, float y);
    void onMenuBottomRight1Click(TgMouseType type, float x, float y);
    void onMenu2ndBottomRight1Click(TgMouseType type, float x, float y);
    void onMenu2ndBottomRight3Click(TgMouseType type, float x, float y);
    void onMenuLargerClick(TgMouseType type, float x, float y);
    void onMenuTopLeft2Click(TgMouseType type, float x, float y);
    void onMenuTopLeft22Click(TgMouseType type, float x, float y);

    void addMenuClicked(size_t index, int x, int y);

};

#endif
