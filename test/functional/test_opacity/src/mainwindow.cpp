#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "Opacity test", width-200, height-200, width+200, height+200),
    m_rectangle0(this, 255, 255, 255),
    m_rectangle0_0(&m_rectangle0, 20.0f, 20.0f, 100.0f, 100.0f, 255, 0, 0),
    m_rectangle0_0_0(&m_rectangle0_0, 20.0f, 20.0f, 100.0f, 100.0f, 0, 255, 0),
    m_item1(&m_rectangle0, 70, 20, 110, 100),
    m_button1(&m_item1, 5, 5, 100, 20, "button")
{
    m_item1.setVisible(false);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setMakeStep(int index)
{
    switch (index)
    {
    case 1:
        m_rectangle0_0_0.setOpacity(0.5f);
        break;
    case 2:
        m_rectangle0_0.setOpacity(0.5f);
        break;
    case 3:
        m_rectangle0_0_0.setOpacity(1.0f);
        break;
    case 4:
        m_rectangle0_0.setOpacity(1.0f);
        break;
    case 5:
        m_rectangle0.setOpacity(0.1f);
        break;
    case 6:
        m_rectangle0.setOpacity(0.5f);
        break;
    case 7:
        m_rectangle0.setOpacity(1.01f);
        break;
    case 8:
        m_rectangle0.setOpacity(-0.1f);
        break;
    case 9:
        m_rectangle0.setOpacity(1.0f);
        break;
    case 10:
        m_item1.setVisible(true);
        break;
    case 11:
        m_rectangle0.setOpacity(0.5f);
        break;
    case 12:
        m_button1.setOpacity(0.5f);
        break;
    case 13:
        m_button1.setOpacity(1.0f);
        break;
    case 14:
        m_item1.setOpacity(0.5f);
        break;
    case 15:
        m_item1.setOpacity(0.75f);
        break;
    case 16:
        m_rectangle0.setOpacity(1.0f);
        m_item1.setOpacity(0.0f);
        break;
    default:
        break;
    }
}