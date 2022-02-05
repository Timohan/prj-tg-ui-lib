#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height) :
    TgMainWindow(width, height, "TestApp1"),
    m_buttonClose(this, 20, 20, 200, 50, "Close button"),
    m_buttonChangeTexture(this, 240, 20, 200, 50, "Change texture"),
    m_buttonChangeCropPosition(this, 480, 20, 200, 50, "Change crop position"),
    m_buttonChangeAreaPosition(this, 20, 80, 200, 50, "Change area position"),
    m_buttonChangeAreaType(this, 240, 80, 200, 50, "Change area type"),
    m_imagePart(this, 20, 180, 350, 300, "images/test0.png", 0.3f, 0.3f, 0.3f, 0.3f, 30, 30, 30, 30),
    m_imageIndex(false),
    m_imageCropPosition(false),
    m_imageAreaPosition(false)
{
    m_buttonClose.connectOnMouseClicked( std::bind(&MainWindow::onButtonCloseClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonChangeTexture.connectOnMouseClicked( std::bind(&MainWindow::onButtonChangeTextureClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonChangeCropPosition.connectOnMouseClicked( std::bind(&MainWindow::onButtonChangeCropPositionClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonChangeAreaPosition.connectOnMouseClicked( std::bind(&MainWindow::onButtonChangeAreaPositionClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonChangeAreaType.connectOnMouseClicked( std::bind(&MainWindow::onButtonChangeTypePositionClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
}

MainWindow::~MainWindow()
{
}

/*!
 * \brief MainWindow::onButtonCloseClick
 *
 * callback when button close is clicked
 */
void MainWindow::onButtonCloseClick(TgMouseType type, float x, float y)
{
    std::cout << "Closing the application\n";
    exit();
}

void MainWindow::onButtonChangeTextureClick(TgMouseType type, float x, float y)
{
    std::cout << "Change texture\n";
    m_imageIndex = !m_imageIndex;
    if (m_imageIndex) {
        m_imagePart.setImage("images/test1.png");
    } else {
        m_imagePart.setImage("images/test0.png");
    }
}

void MainWindow::onButtonChangeCropPositionClick(TgMouseType type, float x, float y)
{
    std::cout << "Change texture\n";
    m_imageCropPosition = !m_imageCropPosition;
    switch (m_imagePart.getType()) {
        case TgImagePartType::TgImagePartType_Part9:
            if (m_imageCropPosition) {
                m_imagePart.setImageCropPosition(0.2f, 0.2f, 0.2f, 0.2f);
            } else {
                m_imagePart.setImageCropPosition(0.3f, 0.3f, 0.3f, 0.3f);
            }
            break;
        case TgImagePartType::TgImagePartType_Part3_UpToDown:
            if (m_imageCropPosition) {
                m_imagePart.setImageCropPosition3TopToBottom(0.2f, 0.2f);
            } else {
                m_imagePart.setImageCropPosition3TopToBottom(0.3f, 0.3f);
            }
            break;
        default:
        case TgImagePartType::TgImagePartType_Part3_LeftToRight:
            if (m_imageCropPosition) {
                m_imagePart.setImageCropPosition3LeftToRight(0.2f, 0.2f);
            } else {
                m_imagePart.setImageCropPosition3LeftToRight(0.3f, 0.3f);
            }
            break;
    }
}

void MainWindow::onButtonChangeAreaPositionClick(TgMouseType type, float x, float y)
{
    std::cout << "Change area position\n";
    m_imageAreaPosition = !m_imageAreaPosition;
    switch (m_imagePart.getType()) {
        case TgImagePartType::TgImagePartType_Part9:
            if (m_imageAreaPosition) {
                m_imagePart.setImageAreaSize(15.0f, 15.0f, 15.0f, 15.0f);
            } else {
                m_imagePart.setImageAreaSize(30.0f, 30.0f, 30.0f, 30.0f);
            }
            break;
        case TgImagePartType::TgImagePartType_Part3_UpToDown:
            if (m_imageAreaPosition) {
                m_imagePart.setImageAreaSize3TopToBottom(15.0f, 15.0f);
            } else {
                m_imagePart.setImageAreaSize3TopToBottom(50.0f, 50.0f);
            }
            break;
        default:
        case TgImagePartType::TgImagePartType_Part3_LeftToRight:
            if (m_imageAreaPosition) {
                m_imagePart.setImageAreaSize3LeftToRight(15.0f, 15.0f);
            } else {
                m_imagePart.setImageAreaSize3LeftToRight(50.0f, 50.0f);
            }
            break;
    }
}

void MainWindow::onButtonChangeTypePositionClick(TgMouseType type, float x, float y)
{
    std::cout << "Change area type\n";
    if (m_imagePart.getType() == TgImagePartType::TgImagePartType_Part9) {
        m_imagePart.setType(TgImagePartType::TgImagePartType_Part3_LeftToRight);
    } else if (m_imagePart.getType() == TgImagePartType::TgImagePartType_Part3_LeftToRight) {
        m_imagePart.setType(TgImagePartType::TgImagePartType_Part3_UpToDown);
    } else {
        m_imagePart.setType(TgImagePartType::TgImagePartType_Part9);
    }
}

