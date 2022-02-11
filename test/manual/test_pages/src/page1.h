#ifndef PAGE1_H
#define PAGE1_H

#include <item2d/tg_page.h>
#include <item2d/tg_button.h>
class TgPages;

class Page1 : public TgPage
{
public:
    Page1(TgPages *parent);
    ~Page1();

private:
    TgPages *m_pages;

    TgButton m_buttonChange;
    TgButton m_buttonChangeSwitchTime;
    TgButton m_buttonChangeSwitchTimeToSecond;

    void onButtonClickPage0(TgMouseType type, float x, float y);

    void onButtonPageChangeTimeToDefault(TgMouseType type, float x, float y);
    void onButtonPageChangeTimeToSecond(TgMouseType type, float x, float y);
    void onPageVisibleChanged(bool visible);

    void onButtonChangeSelectedChanged(bool selected);
    void onButtonChangeSwitchTimeSelectedChanged(bool selected);
    void onButtonChangeSwitchTimeToSecondSelectedChanged(bool selected);

};

#endif
