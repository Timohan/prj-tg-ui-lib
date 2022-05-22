#ifndef PAGE3_H
#define PAGE3_H

#include <item2d/tg_page.h>
#include <item2d/tg_button.h>
#include <item2d/tg_textedit.h>
class TgPages;

class Page3 : public TgPage
{
public:
    Page3(TgPages *parent);
    ~Page3();

    TgButton *getButton(int index);
    TgTextedit *getTextEdit();

private:
    TgPages *m_pages;

    TgButton m_buttonChangePage0;
    TgButton m_buttonChangeText;
    TgTextedit m_textEdit;

    void onButtonClickPage0(TgMouseType type, float x, float y);
    void onButtonClickChangeText(TgMouseType type, float x, float y);
};

#endif

