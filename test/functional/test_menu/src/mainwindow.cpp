#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "Test Menu", width-200, height-200, width+200, height+200),
    m_menuTop(this),
    m_button0(this, 5, 15, 20, 20, 0),
    m_button1(this, static_cast<float>(width)-25, 15, 20, 20, 1),
    m_button2(this, 5, static_cast<float>(height)-30, 20, 20, 2),
    m_button3(this, static_cast<float>(width)-25, static_cast<float>(height)-30, 20, 20, 3)
{
    m_fileItem = m_menuTop.addMenu("File");
    m_editItem = m_menuTop.addMenu("Edit");
    m_selectionItem = m_menuTop.addMenu("Selection");
    m_helpItem = m_menuTop.addMenu("Help");

    TgShortCut shortCut;
    shortCut.m_ctrl = true;
    shortCut.m_key = 'N';
    TgMenuItem *newFile = m_fileItem->addMenu("New", &shortCut);
    newFile->connectOnMouseClicked( std::bind(&MainWindow::onMenuNewClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    shortCut.m_alt = true;
    TgMenuItem *saveFile = m_fileItem->addMenu("Save", &shortCut);
    saveFile->connectOnMouseClicked( std::bind(&MainWindow::onMenuSaveClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    shortCut.m_shift = true;
    shortCut.m_alt = false;
    TgMenuItem *openFile = m_fileItem->addMenu("Open", &shortCut);
    openFile->connectOnMouseClicked( std::bind(&MainWindow::onMenuOpenClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_fileItem->addMenu("Open recent", nullptr);

    m_editItem->addMenu("Undo", nullptr);
    m_editItem->addMenu("Redo", nullptr);
    m_editItem->addMenu("Cut", nullptr);
    shortCut.m_shift = false;
    shortCut.m_key = 'c';
    TgMenuItem *copyFile = m_editItem->addMenu("Copy", &shortCut);
    copyFile->connectOnMouseClicked( std::bind(&MainWindow::onMenuCopyClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_editItem->addMenu("Paste", nullptr);

    m_selectionItem->addMenu("Select all", nullptr);
    m_expandMenu = m_selectionItem->addMenu("Expand", nullptr);

    shortCut.m_ctrl = false;
    shortCut.m_shift = true;
    shortCut.m_alt = false;
    shortCut.m_key = 'e';
    m_larger = m_expandMenu->addMenu("Larger", &shortCut);
    m_larger->connectOnMouseClicked( std::bind(&MainWindow::onMenuLargerClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_smaller = m_expandMenu->addMenu("Smaller", nullptr);
    m_somethingElse = m_expandMenu->addMenu("Something else", nullptr);

    m_helpItem->addMenu("About", nullptr);

    m_button1.addMenu("trm", nullptr);
    m_button1.addMenu("Top right Menu 1", nullptr);
    m_button1.addMenu("Top right Menu 2", nullptr);

    m_topLeft0 = m_button0.addMenu("top left menu", nullptr);
    m_topLeft1 = m_button0.addMenu("Top left Menu 1", nullptr);
    m_topLeft2 = m_button0.addMenu("Top left Menu 2", nullptr);
    m_topLeft2->connectOnMouseClicked( std::bind(&MainWindow::onMenuTopLeft2Click, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );

    m_topLeft20 = m_topLeft2->addMenu("top left menu", nullptr);
    m_topLeft21 = m_topLeft2->addMenu("Top left Menu 1", nullptr);
    shortCut.m_ctrl = false;
    shortCut.m_shift = true;
    shortCut.m_alt = false;
    shortCut.m_key = 'f';
    m_topLeft22 = m_topLeft2->addMenu("Top left Menu 2", &shortCut);
    m_topLeft22->connectOnMouseClicked( std::bind(&MainWindow::onMenuTopLeft22Click, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
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

void MainWindow::addMenuClicked(size_t index, int x, int y)
{
    m_mutex.lock();
    std::cout << "MainWindow::addMenuClicked " << index <<  " " << x << "/" << y << "\n";
    m_listMouseStateChange.push_back(MouseStateChange(index, HoverVisibleChangeState::MouseMenuClicked));
    m_listMouseStateChange.back().m_x = x;
    m_listMouseStateChange.back().m_y = y;
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

Button *MainWindow::getButton(size_t i)
{
    switch (i) {
        case 0:
            return &m_button0;
        case 1:
            return &m_button1;
        case 2:
            return &m_button2;
        case 3:
            return &m_button3;
        default:
            break;
    }
    return nullptr;
}

TgItem2d *MainWindow::getItem2d(size_t i)
{
    switch (i) {
        case 0:
            return &m_button0;
        case 1:
            return &m_button1;
        case 2:
            return &m_button2;
        case 3:
            return &m_button3;
        case 7:
            return m_larger;
        case 8:
            return m_expandMenu;
        case 9:
            return m_smaller;
        case 10:
            return m_somethingElse;
        case 11:
            return m_topLeft0;
        case 12:
            return m_topLeft1;
        case 13:
            return m_topLeft2;
        case 14:
            return m_topLeft20;
        case 15:
            return m_topLeft21;
        case 16:
            return m_topLeft22;
        case 17:
            return m_fileItem;
        case 18:
            return m_editItem;
        case 19:
            return m_selectionItem;
        case 20:
            return m_helpItem;
        default:
            break;
    }
    return nullptr;
}

void MainWindow::removeMenu(size_t i, size_t indexToRemove)
{
    switch (i) {
        case 1:
            m_menuTop.removeMenu(indexToRemove);
            break;
        case 2:
            m_expandMenu->removeMenu(indexToRemove);
            break;
        case 3:
            m_button1.removeMenu(indexToRemove);
        default:
            break;
    }
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
        case HoverVisibleChangeState::MouseMenuClicked:
            return "MouseMenuClicked";
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

void MainWindow::onMenuNewClick(TgMouseType type, float x, float y)
{
    (void)type;
    addMenuClicked(0, static_cast<int>(x), static_cast<int>(y));
}

void MainWindow::onMenuSaveClick(TgMouseType type, float x, float y)
{
    (void)type;
    addMenuClicked(1, static_cast<int>(x), static_cast<int>(y));
}

void MainWindow::onMenuOpenClick(TgMouseType type, float x, float y)
{
    (void)type;
    addMenuClicked(2, static_cast<int>(x), static_cast<int>(y));
}

void MainWindow::onMenuBottomRight2Click(TgMouseType type, float x, float y)
{
    (void)type;
    addMenuClicked(4, static_cast<int>(x), static_cast<int>(y));
}

void MainWindow::onMenu2ndBottomRight1Click(TgMouseType type, float x, float y)
{
    (void)type;
    addMenuClicked(6, static_cast<int>(x), static_cast<int>(y));
    m_bottomRightMenu->addMenu("2nd bottom right menu new line", nullptr);
}

void MainWindow::onMenu2ndBottomRight3Click(TgMouseType type, float x, float y)
{
    (void)type;
    addMenuClicked(5, static_cast<int>(x), static_cast<int>(y));
}

void MainWindow::onMenuLargerClick(TgMouseType type, float x, float y)
{
    (void)type;
    addMenuClicked(7, static_cast<int>(x), static_cast<int>(y));
}

void MainWindow::onMenuTopLeft22Click(TgMouseType type, float x, float y)
{
    (void)type;
    addMenuClicked(8, static_cast<int>(x), static_cast<int>(y));
}

void MainWindow::onMenuTopLeft2Click(TgMouseType type, float x, float y)
{
    (void)type;
    addMenuClicked(9, static_cast<int>(x), static_cast<int>(y));
}

void MainWindow::onMenuBottomRight1Click(TgMouseType type, float x, float y)
{
    (void)type;
    addMenuClicked(5, static_cast<int>(x), static_cast<int>(y));
    if (m_bottomRightMenu
        && m_bottomRightMenu->getMenuCount() == 0) {
        m_bottomRightMenu->addMenu("2nd bottom right menu 0", nullptr);
        TgMenuItem *tmp1 = m_bottomRightMenu->addMenu("2nd bottom right menu 1", nullptr);
        tmp1->connectOnMouseClicked( std::bind(&MainWindow::onMenu2ndBottomRight1Click, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
        m_bottomRightMenu->addMenu("2nd bottom right menu 2", nullptr);
        TgShortCut shortCut;
        shortCut.m_ctrl = true;
        shortCut.m_alt = true;
        shortCut.m_shift = false;
        shortCut.m_key = 'b';
        TgMenuItem *tmp3 = m_bottomRightMenu->addMenu("2nd bottom right menu 3", &shortCut);
        tmp3->connectOnMouseClicked( std::bind(&MainWindow::onMenu2ndBottomRight3Click, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    }
}

void MainWindow::onMenuCopyClick(TgMouseType type, float x, float y)
{
    (void)type;
    addMenuClicked(3, static_cast<int>(x), static_cast<int>(y));
    if (m_button3.getMenuCount() == 0) {
        TgShortCut shortCut;
        m_bottomRightMenu = m_button3.addMenu("bottom right menu", nullptr);
        TgMenuItem *tmp1 = m_button3.addMenu("bottom right Menu 1", nullptr);
        tmp1->connectOnMouseClicked( std::bind(&MainWindow::onMenuBottomRight1Click, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
        shortCut.m_ctrl = true;
        shortCut.m_alt = true;
        shortCut.m_shift = true;
        shortCut.m_key = 'b';
        TgMenuItem *tmp = m_button3.addMenu("bottom right Menu 2", &shortCut);
        tmp->connectOnMouseClicked( std::bind(&MainWindow::onMenuBottomRight2Click, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    }
}
