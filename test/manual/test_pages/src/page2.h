#ifndef PAGE2_H
#define PAGE2_H

#include <item2d/tg_page.h>
#include <item2d/tg_button.h>
class TgPages;

class Page2 : public TgPage
{
public:
    Page2(TgPages *parent);
    ~Page2();

private:
    TgPages *m_pages;

    TgButton m_buttonChange;

    TgButton m_buttonChangeSwitchTypeDirect;
    TgButton m_buttonChangeSwitchTypeFromTop;
    TgButton m_buttonChangeSwitchTypeFromBottom;
    TgButton m_buttonChangeSwitchTypeFromLeft;
    TgButton m_buttonChangeSwitchTypeFromRight;

    void onButtonClickPage0(TgMouseType type, float x, float y);
    void onButtonClickFromDirect(TgMouseType type, float x, float y);
    void onButtonClickFromTop(TgMouseType type, float x, float y);
    void onButtonClickFromBottom(TgMouseType type, float x, float y);
    void onButtonClickFromLeft(TgMouseType type, float x, float y);
    void onButtonClickFromRight(TgMouseType type, float x, float y);
    void onPageVisibleChanged(bool visible);
};

#endif
