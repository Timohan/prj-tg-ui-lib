#ifndef PAGE0_H
#define PAGE0_H

#include <item2d/tg_page.h>
#include <item2d/tg_button.h>
#include <item2d/tg_textfield.h>
class TgPages;
class MainWindow;

class Page0 : public TgPage
{
public:
    Page0(TgPages *parent, MainWindow *mainWindow);
    ~Page0();

private:
    TgPages *m_pages;
    MainWindow *m_mainWindow;

    TgButton m_buttonChangeToPage1;
    TgButton m_buttonChangeToPage2;
    TgButton m_buttonClose;
    TgTextfield m_textFieldForTestBottomRight;
    TgTextfield m_textFieldForTestTopLeft;

    void onButtonClickPage1(TgMouseType type, float x, float y);
    void onButtonClickPage2(TgMouseType type, float x, float y);
    void onButtonCloseClick(TgMouseType type, float x, float y);
    void onPageVisibleChanged(bool visible);
    void onButtonCloseVisibleChanged(bool visible);

    void onButtonSelectedChangeToPage1Changed(bool selected);
    void onButtonSelectedChangeToPage2Changed(bool selected);
    void onButtonSelectedCloseChanged(bool selected);
};

#endif
