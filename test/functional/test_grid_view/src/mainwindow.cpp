#include "mainwindow.h"
#include <iostream>
#include <thread>
#include <cstring>
#include <item2d/tg_menu_item.h>

MainWindow::MainWindow(int width, int height) : TgMainWindow(width, height, "Test gridview", width - 200, height - 200, width + 200, height + 200),
                                                m_background(this, 200, 200, 200),
                                                m_gridview(this, 10, 50, static_cast<float>(width - 20), static_cast<float>(height - 60), 40, 40)
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
    case HoverVisibleChangeState::MenuClicked:
        return "MenuClicked";
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
    TgShortCut shortCut;
    switch (index) {
    case 1:
        for (size_t x = 0; x < m_gridview.getColumCount(); x++) {
            for (size_t y = 0; y < m_gridview.getRowCount(); y++) {
                m_gridview.getCell(x, y)->setText(std::string(std::to_string(x) + "/" + std::to_string(y)).c_str());
            }
        }
        break;
    case 2:
        m_gridview.setRowCount(35);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 3:
        m_gridview.setColumCount(34);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 4:
        m_gridview.setRowCount(37);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 5:
        m_gridview.setColumCount(38);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 6:
        for (size_t x = 0; x < m_gridview.getColumCount(); x++) {
            for (size_t y = 0; y < m_gridview.getRowCount(); y++) {
                if (!m_gridview.getCell(x, y)->getText().empty()) {
                    continue;
                }
                TgTextFieldText field0;
                field0.m_text = std::string(std::to_string(x) + "/" + std::to_string(y));
                field0.m_textColorR = 255;
                field0.m_textColorG = 0;
                field0.m_textColorB = 0;
                std::vector<TgTextFieldText> vecText;
                vecText.push_back(field0);
                m_gridview.getCell(x, y)->setText(vecText);
            }
        }
        break;
    case 7:
        m_gridview.setRowCount(20);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 8:
        m_gridview.setColumCount(7);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 9:
        m_gridview.setRowCount(21);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 10:
        m_gridview.setColumCount(8);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 11:
        m_gridview.getCell(1, 5)->setWidth(350);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 12:
        m_gridview.setColumCount(9);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 13:
        m_gridview.getCell(5, 1)->setHeight(350);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 14:
        m_gridview.setRowCount(22);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 15:
        m_gridview.setColumCount(2);
        m_gridview.setRowCount(3);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 16:
        m_gridview.setColumCount(0);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 17:
        m_gridview.setColumCount(4);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 18:
        m_gridview.setRowCount(0);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 19:
        m_gridview.setRowCount(3);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 20:
        m_gridview.getCell(1, 1)->setBackground(255, 10, 10, 255);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 21:
        m_gridview.setColumCount(6);
        m_gridview.setRowCount(6);
        for (size_t x = 0; x < m_gridview.getColumCount(); x++) {
            for (size_t y = 0; y < m_gridview.getRowCount(); y++) {
                if (!m_gridview.getCell(x, y)->getText().empty()) {
                    continue;
                }
                TgTextFieldText field0;
                field0.m_text = std::string(std::to_string(x) + "/" + std::to_string(y));
                field0.m_textColorR = 255;
                field0.m_textColorG = 0;
                field0.m_textColorB = 0;
                std::vector<TgTextFieldText> vecText;
                vecText.push_back(field0);
                m_gridview.getCell(x, y)->setText(vecText);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 22:
        m_gridview.getCell(0, 0)->setTextMarginLeft(5);
        m_gridview.getCell(0, 1)->setTextMarginRight(15);
        m_gridview.getCell(0, 2)->setTextMarginTop(10);
        m_gridview.getCell(0, 3)->setTextMarginBottom(6);
        m_gridview.getCell(1, 0)->setTextHorizontalAlign(TgTextfieldHorizontalAlign::AlignLeft);
        m_gridview.getCell(1, 1)->setTextHorizontalAlign(TgTextfieldHorizontalAlign::AlignRight);
        m_gridview.getCell(1, 2)->setTextVerticalAlign(TgTextfieldVerticalAlign::AlignBottom);
        m_gridview.getCell(1, 3)->setTextVerticalAlign(TgTextfieldVerticalAlign::AlignTop);
        m_gridview.getCell(0, 4)->setFontSize(15);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 23:
        if (static_cast<int>(m_gridview.getCell(0, 0)->getTextMarginLeft()) != 5
            || static_cast<int>(m_gridview.getCell(0, 1)->getTextMarginRight()) != 15
            || static_cast<int>(m_gridview.getCell(0, 2)->getTextMarginTop()) != 10
            || static_cast<int>(m_gridview.getCell(0, 3)->getTextMarginBottom()) != 6
            || m_gridview.getCell(1, 0)->getTextAlignHorizontal() != TgTextfieldHorizontalAlign::AlignLeft
            || m_gridview.getCell(1, 1)->getTextAlignHorizontal() != TgTextfieldHorizontalAlign::AlignRight
            || m_gridview.getCell(1, 2)->getTextAlignVertical() != TgTextfieldVerticalAlign::AlignBottom
            || m_gridview.getCell(1, 3)->getTextAlignVertical() != TgTextfieldVerticalAlign::AlignTop) {
            return false;
        }
        break;
    case 24:
        m_gridview.getCell(0, 0)->setText("This and that");
        m_gridview.getCell(0, 1)->setText("T");
        m_gridview.getCell(1, 0)->setText("This and that");
        m_gridview.getCell(1, 1)->setText("T");
        m_gridview.getCell(1, 1)->setWidthType(TgGridViewCellSizeType::TgGridViewCellSize_SizeFollowTextSize);
        break;
    case 25: {
            m_gridview.getCell(1, 1)->addMenu("Menu Item0", nullptr);
            m_gridview.getCell(1, 1)->addMenu("Menu Item on cell again", nullptr);
            shortCut.m_ctrl = true;
            shortCut.m_key = 'a';
            m_gridview.getCell(1, 1)->addMenu("Menu Item on cell again 2", &shortCut);
        }
        break;
    case 26:
        if (m_gridview.getCell(1, 1)->getMenuCount() != 3) {
            std::cout << "Case 26, menu count is incorrect " << m_gridview.getCell(1, 1)->getMenuCount() << "\n";
            return false;
        }
        break;
    case 27:
        m_gridview.getCell(1, 1)->getMenu(0)->connectOnMouseClicked(
                std::bind(&MainWindow::onMenuItem0Clicked, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), "Item0"
        );
        m_gridview.getCell(1, 1)->getMenu(2)->connectOnMouseClicked(
                std::bind(&MainWindow::onMenuItem2Clicked, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), "Item2"
        );
        break;
    case 28:
        m_gridview.getCell(1, 1)->removeMenu(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        break;
    case 29:
        std::this_thread::sleep_for(std::chrono::milliseconds(1250));
        if (m_gridview.getCell(1, 1)->getMenuCount() != 2) {
            std::cout << "Case 29, menu count is incorrect " << m_gridview.getCell(1, 1)->getMenuCount() << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(100));
            return false;
        }
        break;
    default:
        break;
    }
    return true;
}

void MainWindow::onMenuItem0Clicked(TgMouseType, float, float, const void *id)
{
    m_mutex.lock();
    if (strcmp(reinterpret_cast<const char *>(id), "Item0") == 0) {
        m_listMouseStateChange.push_back(MouseStateChange(0, HoverVisibleChangeState::MenuClicked));
        std::cout << "onMenuItem0Clicked correct ID\n";
    } else {
        std::cout << "onMenuItem0Clicked Incorrect ID\n";
    }
    m_mutex.unlock();
}

void MainWindow::onMenuItem2Clicked(TgMouseType, float, float, const void *id)
{
    m_mutex.lock();
    if (strcmp(reinterpret_cast<const char *>(id), "Item2") == 0) {
        m_listMouseStateChange.push_back(MouseStateChange(2, HoverVisibleChangeState::MenuClicked));
        std::cout << "onMenuItem2Clicked correct ID\n";
    } else {
        std::cout << "onMenuItem2Clicked Incorrect ID\n";
    }
    m_mutex.unlock();
}
