#include "button.h"
#include <iostream>
#include "mainwindow.h"

Button::Button(MainWindow *mainWindow, float x, float y, float width, float height, size_t index) :
    TgButton(mainWindow, x, y, width, height, "x"),
    m_mainWindow(mainWindow),
    m_index(index)
{

}

Button::Button(MainWindow *mainWindow, TgItem2d *parent, float x, float y, float width, float height, size_t index) :
    TgButton(parent, x, y, width, height, "x"),
    m_mainWindow(mainWindow),
    m_index(index)
{

}

void Button::onHoverChanged(bool hover)
{
    std::cout << "Button::onHoverChanged " << m_index << "/" << hover << "\n";
    m_mainWindow->addHoverChange(m_index, hover);
}

void Button::onVisibleChanged(bool visible)
{
    std::cout << "Button::onVisibleChanged " << m_index << "/" << visible << "\n";
    m_mainWindow->addVisibleChange(m_index, visible);
}

void Button::onEnabledChanged(bool enabled)
{
    std::cout << "Button::onEnabledChanged " << m_index << "/" << enabled << "\n";
    m_mainWindow->addEnabledChange(m_index, enabled);
}

void Button::onMouseClicked(TgMouseType button, float x, float y)
{
    std::cout << "Button::onMouseClicked " << m_index <<  " " << x << "/" << y << "\n";
    MouseStateChange state(m_index, HoverVisibleChangeState::MouseClicked);
    state.m_x = static_cast<int>(x);
    state.m_y = static_cast<int>(y);
    m_mainWindow->addMouseStateChange(state);
}

void Button::onMousePressed(TgMouseType button, float x, float y)
{
    std::cout << "Button::onMousePressed " << m_index <<  " " << x << "/" << y << "\n";
    MouseStateChange state(m_index, HoverVisibleChangeState::MousePress);
    state.m_x = static_cast<int>(x);
    state.m_y = static_cast<int>(y);
    m_mainWindow->addMouseStateChange(state);
}

void Button::onMouseReleased(TgMouseType button, bool inArea, float x, float y)
{
    std::cout << "Button::onMouseReleased " << m_index <<  " " << x << "/" << y << " " << inArea << "\n";
    MouseStateChange state(m_index, HoverVisibleChangeState::MouseRelease);
    state.m_x = static_cast<int>(x);
    state.m_y = static_cast<int>(y);
    state.m_area = inArea;
    m_mainWindow->addMouseStateChange(state);
}

void Button::Button::onMouseMove(bool inArea, float x, float y)
{
    std::cout << "Button::onMouseMove " << m_index <<  " " << x << "/" << y << " " << inArea << "\n";
    MouseStateChange state(m_index, HoverVisibleChangeState::MouseMove);
    state.m_x = static_cast<int>(x);
    state.m_y = static_cast<int>(y);
    state.m_area = inArea;
    m_mainWindow->addMouseStateChange(state);
}

