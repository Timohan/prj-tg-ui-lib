#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <item2d/tg_button.h>
#include <item2d/tg_combo_box.h>

class MainWindow : public TgMainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();
private:
    TgButton m_buttonClose;
    TgButton m_buttonAddComboBoxItem;
    TgButton m_buttonRemoveComboBoxItem;
    TgComboBox m_combobox;
    TgComboBox *m_comboboxSecondary = nullptr;
    int m_currentItemIndex = 0;

    void onButtonCloseClick(TgMouseType type, float x, float y, const void *id);
    void onButtonAddClick(TgMouseType type, float x, float y, const void *id);
    void onButtonRemoveClick(TgMouseType type, float x, float y, const void *id);
    void onComboBoxItemChanged(size_t index);
};

#endif
