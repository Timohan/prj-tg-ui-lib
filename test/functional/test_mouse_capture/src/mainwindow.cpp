#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "TestHover", width-200, height-200, width+200, height+200),
    m_mouseCapture0(this, 0, 5, 5, 5, 0),
    m_mouseCapture1(this, 5, 5, 5, 5, 1),
    m_mouseCapture2(this, 0, 50, 20, 20, 2),
    m_mouseCapture3(this, 10, 50, 20, 20, 3),
    m_mouseCapture4(this, 0, 100, 40, 20, 4),
    m_mouseCapture4on0(this, &m_mouseCapture4, 10, 0, 30, 20, 5),
    m_mouseCapture4on1(this, &m_mouseCapture4on0, 10, 0, 20, 20, 6)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::addHoverChange(size_t index, bool hover)
{
    m_mutex.lock();
    m_listMouseStateChange.push_back(MouseStateChange(index,
        hover ? HoverVisibleChangeState::HoverEnable : HoverVisibleChangeState::HoverDisable));
    m_mutex.unlock();
}

void MainWindow::addVisibleChange(size_t index, bool visible)
{
    m_mutex.lock();
    m_listMouseStateChange.push_back(MouseStateChange(index,
        visible ? HoverVisibleChangeState::Visible : HoverVisibleChangeState::Invisible));
    m_mutex.unlock();
}

void MainWindow::addEnabledChange(size_t index, bool enabled)
{
    m_mutex.lock();
    m_listMouseStateChange.push_back(MouseStateChange(index,
        enabled ? HoverVisibleChangeState::Enabled : HoverVisibleChangeState::Disabled));
    m_mutex.unlock();
}

size_t MainWindow::getMouseStateChangeCount()
{
    size_t ret;
    m_mutex.lock();
    ret = m_listMouseStateChange.size();
    m_mutex.unlock();
    return ret;
}

MouseStateChange *MainWindow::getMouseStateChange(size_t i)
{
    return &m_listMouseStateChange.at(i);
}

MouseCaptureArea *MainWindow::getMouseCaptureArea(size_t i)
{
    switch (i) {
        case 0:
            return &m_mouseCapture0;
        case 1:
            return &m_mouseCapture1;
        case 2:
            return &m_mouseCapture2;
        case 3:
            return &m_mouseCapture3;
        case 4:
            return &m_mouseCapture4;
        case 5:
            return &m_mouseCapture4on0;
        case 6:
            return &m_mouseCapture4on1;
        default:
            break;
    }
    return nullptr;
}

std::string MainWindow::getString(HoverVisibleChangeState state)
{
    switch (state) {
        case HoverVisibleChangeState::HoverEnable:
            return "HoverEnable";
        case HoverVisibleChangeState::HoverDisable:
            return "HoverDisable";
        case HoverVisibleChangeState::Invisible:
            return "Invisible";
        case HoverVisibleChangeState::Visible:
            return "Visible";
        case HoverVisibleChangeState::Enabled:
            return "Enabled";
        case HoverVisibleChangeState::Disabled:
            return "Disabled";
        case HoverVisibleChangeState::MouseMove:
            return "MouseMove";
        case HoverVisibleChangeState::MousePress:
            return "MousePress";
        case HoverVisibleChangeState::MouseRelease:
            return "MouseRelease";
        case HoverVisibleChangeState::MouseClicked:
            return "MouseClicked";
        default:
            return "";
    }
}

void MainWindow::addMouseStateChange(const MouseStateChange &change)
{
    m_listMouseStateChange.push_back(change);
}

MouseStateChange::MouseStateChange(size_t index, HoverVisibleChangeState state)
{
    m_index = index;
    m_state = state;
}
