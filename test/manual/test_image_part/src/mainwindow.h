#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <window/tg_mainwindow.h>
#include <item2d/tg_button.h>
#include <item2d/tg_image_part.h>
#include <item2d/tg_image.h>

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
    TgButton m_buttonChangeRightImagePixels;
    TgImagePart m_imagePart;
    TgImage m_image;
    bool m_imageIndex;
    bool m_imageCropPosition;
    bool m_imageAreaPosition;
    uint32_t m_rightImagePixelsX;

    void onButtonCloseClick(TgMouseType type, float x, float y, const void *id);
    void onButtonChangeTextureClick(TgMouseType type, float x, float y, const void *id);
    void onButtonChangeCropPositionClick(TgMouseType type, float x, float y, const void *id);
    void onButtonChangeAreaPositionClick(TgMouseType type, float x, float y, const void *id);
    void onButtonChangeTypePositionClick(TgMouseType type, float x, float y, const void *id);
    void onButtonChangeRightImagePixels(TgMouseType type, float x, float y, const void *id);
};

#endif
