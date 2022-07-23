#include "mouse_capture_area.h"
#include <iostream>
#include "mainwindow.h"

MouseCaptureArea::MouseCaptureArea(MainWindow *mainWindow, float x, float y, float width, float height, size_t index) :
    TgMouseCapture(mainWindow, x, y, width, height),
    m_mainWindow(mainWindow),
    m_index(index),
    m_background(this, 255, 255, static_cast<uint8_t>(255-index*30))
{

}

MouseCaptureArea::MouseCaptureArea(MainWindow *mainWindow, TgItem2d *parent, float x, float y, float width, float height, size_t index) :
    TgMouseCapture(parent, x, y, width, height),
    m_mainWindow(mainWindow),
    m_index(index),
    m_background(this, 255, 255, 255)
{

}

void MouseCaptureArea::onHoverChanged(bool hover)
{
    std::cout << "MouseCaptureArea::onHoverChanged " << m_index << "/" << hover << "\n";
    m_mainWindow->addHoverChange(m_index, hover);
}

void MouseCaptureArea::onVisibleChanged(bool visible)
{
    std::cout << "MouseCaptureArea::onVisibleChanged " << m_index << "/" << visible << "\n";
    m_mainWindow->addVisibleChange(m_index, visible);
}

void MouseCaptureArea::onEnabledChanged(bool enabled)
{
    std::cout << "MouseCaptureArea::onEnabledChanged " << m_index << "/" << enabled << "\n";
    m_mainWindow->addEnabledChange(m_index, enabled);
}

void MouseCaptureArea::onMouseClicked(TgMouseType button, float x, float y)
{
    std::cout << "MouseCaptureArea::onMouseClicked " << m_index <<  " " << x << "/" << y << "\n";
    MouseStateChange state(m_index, HoverVisibleChangeState::MouseClicked);
    state.m_x = static_cast<int>(x);
    state.m_y = static_cast<int>(y);
    m_mainWindow->addMouseStateChange(state);
}

void MouseCaptureArea::onMousePressed(TgMouseType button, float x, float y)
{
    std::cout << "MouseCaptureArea::onMousePressed " << m_index <<  " " << x << "/" << y << "\n";
    MouseStateChange state(m_index, HoverVisibleChangeState::MousePress);
    state.m_x = static_cast<int>(x);
    state.m_y = static_cast<int>(y);
    m_mainWindow->addMouseStateChange(state);
}

void MouseCaptureArea::onMouseReleased(TgMouseType button, bool inArea, float x, float y)
{
    std::cout << "MouseCaptureArea::onMouseReleased " << m_index <<  " " << x << "/" << y << " " << inArea << "\n";
    MouseStateChange state(m_index, HoverVisibleChangeState::MouseRelease);
    state.m_x = static_cast<int>(x);
    state.m_y = static_cast<int>(y);
    state.m_area = inArea;
    m_mainWindow->addMouseStateChange(state);
}

void MouseCaptureArea::MouseCaptureArea::onMouseMove(bool inArea, float x, float y)
{
    std::cout << "MouseCaptureArea::onMouseMove " << m_index <<  " " << x << "/" << y << " " << inArea << "\n";
    MouseStateChange state(m_index, HoverVisibleChangeState::MouseMove);
    state.m_x = static_cast<int>(x);
    state.m_y = static_cast<int>(y);
    state.m_area = inArea;
    m_mainWindow->addMouseStateChange(state);
}

