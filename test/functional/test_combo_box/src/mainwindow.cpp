#include "mainwindow.h"
#include <iostream>
#include <thread>

MainWindow::MainWindow(int width, int height)
    : TgMainWindow(width, height, "Test combobox", width - 200, height - 200, width + 200, height + 200),
      m_comboBoxTopLeft(this, 20, 20, 100, 50),
      m_comboBoxTopRight(this, static_cast<float>(width) - 120, 20, 100, 50),
      m_comboBoxBottomLeft(this, 20, static_cast<float>(height) - 70, 100, 50),
      m_comboBoxBottomRight(this, static_cast<float>(width) - 120, static_cast<float>(height) - 70, 100, 50)
{
    m_comboBoxTopLeft.addItemText("TL 0");
    m_comboBoxTopLeft.addItemText("TL 1");
    m_comboBoxTopLeft.addItemText("TL 2");
    m_comboBoxTopLeft.addItemText("TL 3");
    m_comboBoxTopLeft.connectOnSelectedIndexChanged(std::bind(&MainWindow::onComboBoxTopLeftItemChanged, this, std::placeholders::_1));

    m_comboBoxTopRight.connectOnSelectedIndexChanged(std::bind(&MainWindow::onComboBoxTopRightItemChanged, this, std::placeholders::_1));

    m_comboBoxBottomLeft.addItemText("Bottom left 0");
    m_comboBoxBottomLeft.addItemText("Bottom left 1");
    m_comboBoxBottomLeft.addItemText("Bottom left 2");
    m_comboBoxBottomLeft.addItemText("Bottom left 3");
    m_comboBoxBottomLeft.connectOnSelectedIndexChanged(std::bind(&MainWindow::onComboBoxBottomLeftItemChanged, this, std::placeholders::_1));

    m_comboBoxBottomRight.addItemText("Bottom right 0 with long items");
    m_comboBoxBottomRight.addItemText("Bottom right 1 with long items");
    m_comboBoxBottomRight.addItemText("Bottom right 2 with long items");
    m_comboBoxBottomRight.addItemText("Bottom right 3 with long items");
}

MainWindow::~MainWindow()
{
}

/*!
 * \brief MainWindow::onComboBoxTopLeftItemChanged
 *
 * callback when combobox item
 * \param index
 */
void MainWindow::onComboBoxTopLeftItemChanged(size_t index)
{
    std::cout << "on TopLeft Menu Item changed " << index << "\n";
    m_mutex.lock();
    m_listMouseStateChange.push_back(MouseStateChange(0, HoverVisibleChangeState::ComboBoxItemChanged));
    m_listMouseStateChange.back().m_itemIndex = index;
    m_mutex.unlock();
}

void MainWindow::onComboBoxBottomLeftItemChanged(size_t index)
{
    std::cout << "on BottomLeft Menu Item changed " << index << "\n";
    m_mutex.lock();
    m_listMouseStateChange.push_back(MouseStateChange(2, HoverVisibleChangeState::ComboBoxItemChanged));
    m_listMouseStateChange.back().m_itemIndex = index;
    m_mutex.unlock();
}

void MainWindow::onComboBoxTopRightItemChanged(size_t index)
{
    std::cout << "on TopRight Menu Item changed " << index << "\n";
    m_mutex.lock();
    m_listMouseStateChange.push_back(MouseStateChange(1, HoverVisibleChangeState::ComboBoxItemChanged));
    m_listMouseStateChange.back().m_itemIndex = index;
    m_mutex.unlock();
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
    case HoverVisibleChangeState::ComboBoxItemChanged:
        return "ComboBoxItemChanged";
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

void MainWindow::setMakeStep(int index)
{
    switch (index)
    {
    case 1:
        m_comboBoxTopRight.addItemText("TR 0");
        break;
    case 2:
        m_comboBoxTopRight.addItemText("TR -1", 0);
        break;
    case 3:
        m_comboBoxTopRight.addItemText("TR -2", 0);
        break;
    case 4:
        m_comboBoxTopRight.addItemText("TR 1");
        break;
    case 5:
        m_comboBoxTopRight.addItemText("TR 2");
        break;
    case 6:
        m_comboBoxTopRight.addItemText("TR 0.1", 2);
        break;
    case 7:
        m_comboBoxTopRight.addItemText("TR -3", 0);
        break;
    case 8:
        m_comboBoxTopRight.removeItemText( m_comboBoxTopRight.getItemCount()-1 );
        break;
    case 9:
        m_comboBoxTopRight.removeItemText(0);
        break;
    case 10:
        m_comboBoxTopRight.removeItemText(3);
        break;
    default:
        break;
    }
}

bool MainWindow::isSelected(size_t index)
{
    switch (index)
    {
    case 0:
        return m_comboBoxTopLeft.getSelected();
    case 1:
        return m_comboBoxTopRight.getSelected();
    case 2:
        return m_comboBoxBottomLeft.getSelected();
    case 3:
        return m_comboBoxBottomRight.getSelected();
    default:
        break;
    }
    return false;
}
