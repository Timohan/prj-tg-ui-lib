#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <item2d/tg_button.h>
#include <item2d/tg_pages.h>
#include "page0.h"
#include "page1.h"
#include "page2.h"

class MainWindow : public TgMainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();
private:
    TgPages m_pages;
    Page0 m_page0;
    Page1 m_page1;
    Page2 m_page2;

    void onButtonAddClick(TgMouseType type, float x, float y, const void *);
    void onButtonCloseClick(TgMouseType type, float x, float y, const void *);
    void onButtonAddedClick(TgMouseType type, float x, float y, const void *);
    void onButtonRemoveClick(TgMouseType type, float x, float y, const void *);
};

#endif
