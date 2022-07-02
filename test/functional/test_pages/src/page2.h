#ifndef PAGE2_H
#define PAGE2_H

#include <item2d/tg_page.h>
#include <item2d/tg_button.h>
#include <item2d/tg_textfield.h>
class TgPages;

class Page2 : public TgPage
{
public:
    Page2(TgPages *parent);
    ~Page2();

    TgButton *getButton(int index);
    float getAllDrawHeight(int index);
    TgTextfield *getTextfield();
private:
    int m_centerTextIndex;
    TgPages *m_pages;

    TgButton m_buttonChange;

    TgButton m_buttonChangeSwitchTypeDirect;
    TgButton m_buttonChangeSwitchTypeFromTop;
    TgButton m_buttonChangeSwitchTypeFromBottom;
    TgButton m_buttonChangeSwitchTypeFromLeft;
    TgButton m_buttonChangeSwitchTypeFromRight;
    TgButton m_buttonChangeText;
    TgTextfield m_textFieldForTestCenter;
    std::vector<float> m_allDrawHeight;

    void onButtonClickPage0(TgMouseType type, float x, float y);
    void onButtonClickFromDirect(TgMouseType type, float x, float y);
    void onButtonClickFromTop(TgMouseType type, float x, float y);
    void onButtonClickFromBottom(TgMouseType type, float x, float y);
    void onButtonClickFromLeft(TgMouseType type, float x, float y);
    void onButtonClickFromRight(TgMouseType type, float x, float y);
    void onButtonChangeTextClick(TgMouseType type, float x, float y);
    void onPageVisibleChanged(bool visible);

    void onButtonChangeSelectedChanged(bool selected);
    void onButtonChangeSwitchTypeDirectSelectedChanged(bool selected);
    void onButtonChangeSwitchTypeFromTopChanged(bool selected);
    void onButtonChangeSwitchTypeFromBottomChanged(bool selected);
    void onButtonChangeSwitchTypeFromLeftChanged(bool selected);
    void onButtonChangeSwitchTypeFromRightChanged(bool selected);

    void setDrawHeight(int centerTextIndex, float value);
};

#endif

