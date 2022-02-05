#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <item2d/tg_button.h>
#include <item2d/tg_image_part.h>

class MainWindow : public TgMainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();
private:
    TgButton m_buttonClose;
    TgButton m_buttonChangeTexture;
    TgButton m_buttonChangeCropPosition;
    TgButton m_buttonChangeAreaPosition;
    TgButton m_buttonChangeAreaType;
    TgImagePart m_imagePart;
    bool m_imageIndex;
    bool m_imageCropPosition;
    bool m_imageAreaPosition;

    void onButtonCloseClick(TgMouseType type, float x, float y);
    void onButtonChangeTextureClick(TgMouseType type, float x, float y);
    void onButtonChangeCropPositionClick(TgMouseType type, float x, float y);
    void onButtonChangeAreaPositionClick(TgMouseType type, float x, float y);
    void onButtonChangeTypePositionClick(TgMouseType type, float x, float y);
};

#endif
