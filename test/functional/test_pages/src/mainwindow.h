#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <item2d/tg_button.h>
#include <item2d/tg_pages.h>
#include "page0.h"
#include "page1.h"
#include "page2.h"
#include "page3.h"

class TgTextedit;

class MainWindow : public TgMainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();

    TgPages *getPages();
    TgButton *getButton(int page, int index);
    TgTextfield *getTextfield();
    TgTextedit *getTextEdit();
private:
    TgPages m_pages;
    Page0 m_page0;
    Page1 m_page1;
    Page2 m_page2;
    Page3 m_page3;

    void onButtonAddClick(TgMouseType type, float x, float y);
    void onButtonCloseClick(TgMouseType type, float x, float y);
    void onButtonAddedClick(TgMouseType type, float x, float y);
    void onButtonRemoveClick(TgMouseType type, float x, float y);
};

#endif
