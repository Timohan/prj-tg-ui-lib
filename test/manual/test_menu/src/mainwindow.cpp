#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "Test Menu App", width - 300, height - 300),
    m_menuTop(this),
    m_buttonClose(this, 20, 40, 200, 50, "Close button"),
    m_buttonTopRight(this, static_cast<float>(width)-220, 40, 200, 50, "Top Right"),
    m_buttonBottomLeft(this, 20, static_cast<float>(height)-60, 200, 50, "Bottom left"),
    m_buttonBottomRight(this, static_cast<float>(width)-220, static_cast<float>(height)-60, 200, 50, "Bottom right")
{
    TgMenuItem *fileItem = m_menuTop.addMenu("File");
    TgMenuItem *editItem = m_menuTop.addMenu("Edit");
    m_menuTop.addMenu("Selection");
    m_menuTop.addMenu("Help");

    m_buttonClose.connectOnMouseClicked( std::bind(&MainWindow::onButtonCloseClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );

    TgMenuItem *closeItem = m_buttonClose.addMenu("Move close button", nullptr);
    m_buttonClose.addMenu("Test menu 1", nullptr);
    TgMenuItem *testMenu2 = m_buttonClose.addMenu("Test menu 2", nullptr);
    testMenu2->addMenu("Test sub 0", nullptr);
    testMenu2->addMenu("Test sub 1", nullptr);
    testMenu2->addMenu("Test sub 2", nullptr);
    testMenu2->addMenu("Test sub 3", nullptr);
    testMenu2->addMenu("Test sub 4", nullptr);
    testMenu2->addMenu("Test sub 5", nullptr);

    closeItem->connectOnMouseClicked( std::bind(&MainWindow::onMenuItemClicked, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );

    m_newItem = fileItem->addMenu("New", nullptr);
    fileItem->addMenu("Save", nullptr);
    fileItem->addMenu("Open", nullptr);

    m_newItem->connectOnMouseClicked( std::bind(&MainWindow::onNewMenuItemClicked, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );

    m_newItem->addMenu("New sub 0", nullptr);
    m_newItem->addMenu("New sub 1", nullptr);
    m_newItem->addMenu("New sub 2", nullptr);
    m_newItem->addMenu("New sub 3", nullptr);
    TgMenuItem * tmpNewSub4 = m_newItem->addMenu("New sub 4", nullptr);
    tmpNewSub4->setVisible(false);
    m_newItem->addMenu("New sub 5", nullptr);

    editItem->addMenu("Undo", nullptr);
    editItem->addMenu("Redo", nullptr);

    TgMenuItem *newTopRight = m_buttonTopRight.addMenu("Top right 1", nullptr);
    m_buttonTopRight.addMenu("Top right 2", nullptr);

    TgShortCut shortCutBottomLeft;
    shortCutBottomLeft.m_ctrl = true;
    shortCutBottomLeft.m_alt = true;
    shortCutBottomLeft.m_shift = true;
    shortCutBottomLeft.m_key = 'F';
    TgMenuItem *newBottomLeft = m_buttonBottomLeft.addMenu("Bottom left 1", &shortCutBottomLeft);
    m_buttonBottomLeft.addMenu("Bottom left 2 qwdqw", nullptr);

    TgMenuItem *newBottomRight = m_buttonBottomRight.addMenu("Bottom right 1", nullptr);
    m_buttonBottomRight.addMenu("Bottom right 2", nullptr);

    newBottomRight->addMenu("Bottom right sub 0", nullptr);
    newBottomRight->addMenu("Bottom right sub 1", nullptr);
    newBottomRight->addMenu("Bottom right sub 2", nullptr);
    newBottomRight->addMenu("Bottom right sub 3", nullptr);
    m_newBottomRightSub = newBottomRight->addMenu("Bottom right sub 5", nullptr);

    m_newBottomRightSub->addMenu("Bottom right sub 0s", nullptr);
    m_newBottomRightSub->addMenu("Bottom right sub 1s", nullptr);
    m_newBottomRightSub->addMenu("Bottom right sub 2s", nullptr);
    m_newBottomRightSub->addMenu("Bottom right sub 3s qwdwqqdq", nullptr);
    shortCutBottomLeft.m_key = 'A';
    m_newBottomRightSub->addMenu("Bottom right sub 4s", &shortCutBottomLeft);
    TgMenuItem *newBottomRightSub5s = m_newBottomRightSub->addMenu("Bottom right sub 5s", nullptr);

    newTopRight->connectOnMouseClicked( std::bind(&MainWindow::onNewTopRightItemClicked, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    newBottomLeft->connectOnMouseClicked( std::bind(&MainWindow::onNewBottomLeftItemClicked, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    newBottomRight->connectOnMouseClicked( std::bind(&MainWindow::onNewBottomRightItemClicked, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    m_newBottomRightSub->connectOnMouseClicked( std::bind(&MainWindow::onNewBottomRightSub5ItemClicked, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    newBottomRightSub5s->connectOnMouseClicked( std::bind(&MainWindow::onNewBottomRightSub5sItemClicked, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
}

MainWindow::~MainWindow()
{
}

void MainWindow::onButtonCloseClick(TgMouseType type, float x, float y, const void *)
{
    if (type == TgMouseType::ButtonLeft) {
        std::cout << "Closing the application\n";
        exit();
    }
}

void MainWindow::onMenuItemClicked(TgMouseType type, float x, float y, const void *)
{
    std::cout << "Menu item clicked\n";
}

void MainWindow::onNewMenuItemClicked(TgMouseType type, float x, float y, const void *)
{
    std::cout << "New Menu item clicked\n";
}

void MainWindow::onNewTopRightItemClicked(TgMouseType type, float x, float y, const void *)
{
    std::cout << "TopRight Menu item clicked\n";
    m_newItem->setEnabled(!m_newItem->getEnabled());
}

void MainWindow::onNewBottomLeftItemClicked(TgMouseType type, float x, float y, const void *)
{
    std::cout << "BottomLeft Menu item clicked\n";
    TgMenuItem *addedMenu = m_menuTop.addMenu("New add");
    if (addedMenu) {
        std::cout << "Added new menu\n";
        addedMenu->addMenu("New addedMenu", nullptr);
    }
    if (m_newItem) {
        TgMenuItem *add = m_newItem->addMenu("New new time", nullptr);
        if (add) {
            add->connectOnMouseClicked( std::bind(&MainWindow::onNewBottomRight1ItemClicked, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
            std::cout << "Added sub new menu\n";
        }
    }
}

void MainWindow::onNewBottomRightItemClicked(TgMouseType type, float x, float y, const void *)
{
    std::cout << "BottomRight Menu item clicked\n";
    if (m_menuTop.getMenuCount() > 1) {
        if (m_menuTop.getMenuCount()-2 == 0) {
            m_newItem = nullptr;
        }
        m_menuTop.removeMenu(m_menuTop.getMenuCount()-2);
        std::cout << "Removing the menu -2\n";
    } else if (m_menuTop.getMenuCount() > 0) {
        m_menuTop.removeMenu(m_menuTop.getMenuCount()-1);
        std::cout << "Removing the last menu\n";
    } else {
        std::cout << "Removing the last menu not happening, count is 0\n";
    }
}

void MainWindow::onNewBottomRight1ItemClicked(TgMouseType type, float x, float y, const void *)
{
    std::cout << "BottomRight 1 item clicked\n";
}

void MainWindow::onNewBottomRightSub5ItemClicked(TgMouseType type, float x, float y, const void *)
{
    std::cout << "BottomRight Menu sub 5 item clicked\n";
    if (!m_newItem) {
        return;
    }
    if (m_newItem->getMenuCount() > 1) {
        m_newItem->removeMenu(m_newItem->getMenuCount()-2);
        std::cout << "New removing the menu -2\n";
    } else if (m_newItem->getMenuCount() > 0) {
        m_newItem->removeMenu(m_newItem->getMenuCount()-1);
        std::cout << "New removing the last menu\n";
    } else {
        std::cout << "New removing the last menu not happening, count is 0\n";
    }
}

void MainWindow::onNewBottomRightSub5sItemClicked(TgMouseType type, float x, float y, const void *)
{
    std::cout << "BottomRight Menu sub 5s item clicked\n";
}
