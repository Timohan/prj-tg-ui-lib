#include "mainwindow.h"
#include <iostream>
#include <thread>

MainWindow::MainWindow(int width, int height)
    : TgMainWindow(width, height, "Test gridview", width - 200, height - 200, width + 200, height + 200),
      m_background(this, 0, 0, 50, 50, 200, 200, 200),
      m_buttonTopLeft(this, 10, 10, 100, 25, "TopLeft"),
      m_buttonTopRight(this, static_cast<float>(width)-120, 10, 100, 25, "TopRight"),
      m_buttonBottomLeft(this, 10, static_cast<float>(height)-30, 100, 25, "BottomLeft"),
      m_buttonBottomRight(this, static_cast<float>(width)-120, static_cast<float>(height)-30, 100, 25, "BottomRight")
{
    m_background.setTooltip("Background tooltip");
    m_buttonTopLeft.setTooltip("Top left tooltip");
    m_buttonTopRight.setTooltip("Top right tooltip");
    m_buttonBottomLeft.setTooltip("Bottom left tooltip");
    m_buttonBottomRight.setTooltip("Bottom right\ntooltip");
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

std::string MainWindow::getString(HoverVisibleChangeState state)
{
    switch (state)
    {
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

bool MainWindow::setMakeStep(int index)
{
    switch (index) {
    case 1:
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
        break;
    case 2:
        m_buttonTopRight.setEnabled(false);
        break;
    case 3:
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        break;
    case 4:
        m_buttonTopRight.setEnabled(true);
        break;
    case 5:
        m_buttonTopRight.setVisible(false);
        break;
    case 6:
        m_buttonTopRight.setVisible(true);
        break;
    case 7:
        m_buttonTopLeft.setEnabled(false);
        break;
    case 8:
        m_buttonTopLeft.setVisible(false);
        break;
    case 9:
        m_backgroundOnBg = new TgRectangle(this, 30, 30, 50, 50, 100, 100, 100);
        m_backgroundOnBg->setTooltip("Bg on Another");
        break;
    case 10:
        m_backgroundOnBg->setEnabled(false);
        break;
    case 11:
        m_backgroundOnBg->setEnabled(true);
        break;
    case 12:
        m_backgroundOnBg->deleteLater();
        m_backgroundOnBg = nullptr;
        break;
    default:
        break;
    }
    return true;
}

