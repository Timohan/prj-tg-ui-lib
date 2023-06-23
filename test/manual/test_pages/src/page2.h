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

    void onButtonClickPage0(TgMouseType type, float x, float y, const void *);
    void onButtonClickFromDirect(TgMouseType type, float x, float y, const void *);
    void onButtonClickFromTop(TgMouseType type, float x, float y, const void *);
    void onButtonClickFromBottom(TgMouseType type, float x, float y, const void *);
    void onButtonClickFromLeft(TgMouseType type, float x, float y, const void *);
    void onButtonClickFromRight(TgMouseType type, float x, float y, const void *);
    void onPageVisibleChanged(bool visible);

    void onButtonChangeSelectedChanged(bool selected);
    void onButtonChangeSwitchTypeDirectSelectedChanged(bool selected);
    void onButtonChangeSwitchTypeFromTopChanged(bool selected);
    void onButtonChangeSwitchTypeFromBottomChanged(bool selected);
    void onButtonChangeSwitchTypeFromLeftChanged(bool selected);
    void onButtonChangeSwitchTypeFromRightChanged(bool selected);
};

#endif

