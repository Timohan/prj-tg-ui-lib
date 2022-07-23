#ifndef MOUSE_CAPTURE_AREA_H
#define MOUSE_CAPTURE_AREA_H

#include <window/tg_mainwindow.h>
#include <item2d/tg_mouse_capture.h>
#include <item2d/tg_rectangle.h>
class MainWindow;

class MouseCaptureArea : public TgMouseCapture
{
public:
    MouseCaptureArea(MainWindow *mainWindow, float x, float y, float width, float height, size_t index);
    MouseCaptureArea(MainWindow *mainWindow, TgItem2d *parent, float x, float y, float width, float height, size_t index);

protected:
    virtual void onHoverChanged(bool hover) override;
    virtual void onVisibleChanged(bool visible) override;
    virtual void onEnabledChanged(bool enabled) override;
    virtual void onMouseMove(bool inArea, float x, float y) override;
    virtual void onMouseReleased(TgMouseType button, bool inArea, float x, float y) override;
    virtual void onMousePressed(TgMouseType button, float x, float y) override;
    virtual void onMouseClicked(TgMouseType button, float x, float y) override;
private:
    MainWindow *m_mainWindow;
    size_t m_index;
    TgRectangle m_background;

};

#endif
