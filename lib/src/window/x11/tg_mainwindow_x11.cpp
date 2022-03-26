/*!
 * \file
 * \brief file tg_mainwindow_x11.cpp
 *
 * main window (x11) class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_mainwindow_x11.h"
#include <X11/keysym.h>
#include "../tg_mainwindow_private.h"
#include "../../global/tg_global_log.h"

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

TgMainWindowX11::TgMainWindowX11(TgMainWindowPrivate *mainWindowPrivate) :
    m_mainWindowPrivate(mainWindowPrivate),
    m_display(nullptr)
{
}

TgMainWindowX11::~TgMainWindowX11()
{
    XCloseDisplay(m_display);
}

/*!
 * \brief TgMainWindowX11::initWindow
 * \param windowTitle window's title
 * \param info window info
 */
int TgMainWindowX11::initWindow(const char *windowTitle, const TgWindowInfo *info)
{
    TG_FUNCTION_BEGIN();
    GLXFBConfig* fbConfic;
    int fbcCount;
    int bestFbcIndex = -1;
    int worstFbcIndex = -1;
    int bestFbcNumberSamples = -1;
    int worstFbcNumSamp = 999;
    int samplesBuffer;
    int samples;
    int i;
    XVisualInfo *visualInfo;
    XSetWindowAttributes setWindowAttrb;

    m_display = XOpenDisplay(nullptr);

    if (!m_display) {
        TG_ERROR_LOG("Cannot open the display");
        TG_FUNCTION_END();
        return EXIT_FAILURE;
    }

    const static int visualAttrb[] = {
        GLX_X_RENDERABLE,   true,
        GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
        GLX_RENDER_TYPE,    GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
        GLX_RED_SIZE,       8,
        GLX_GREEN_SIZE,     8,
        GLX_BLUE_SIZE,      8,
        GLX_ALPHA_SIZE,     8,
        GLX_DEPTH_SIZE,     24,
        GLX_STENCIL_SIZE,   8,
        GLX_DOUBLEBUFFER,   true,
        None
    };

    int contextAttrbs[] = {
       GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
       GLX_CONTEXT_MINOR_VERSION_ARB, 0,
       None
   };

    fbConfic = glXChooseFBConfig(m_display, DefaultScreen(m_display), visualAttrb, &fbcCount);
    if (!fbConfic) {
        TG_ERROR_LOG("glXChooseFBConfig failed");
        TG_FUNCTION_END();
        return EXIT_FAILURE;
    }

    for (i=0;i<fbcCount;i++) {
        visualInfo = glXGetVisualFromFBConfig(m_display, fbConfic[i]);
        if (visualInfo) {
            glXGetFBConfigAttrib(m_display, fbConfic[i], GLX_SAMPLE_BUFFERS, &samplesBuffer);
            glXGetFBConfigAttrib(m_display, fbConfic[i], GLX_SAMPLES, &samples);

            if (bestFbcIndex < 0 || (samplesBuffer && samples > bestFbcNumberSamples)) {
                bestFbcIndex = i;
                bestFbcNumberSamples = samples;
            }
            if (worstFbcIndex < 0 || !samplesBuffer || samples < worstFbcNumSamp) {
                worstFbcIndex = i;
                worstFbcNumSamp = samples;
            }
        }
        XFree(visualInfo);
    }

    visualInfo = glXGetVisualFromFBConfig(m_display, fbConfic[bestFbcIndex]);

    setWindowAttrb.colormap = m_colorMap = XCreateColormap(m_display,
                                           RootWindow(m_display, visualInfo->screen),
                                           visualInfo->visual, AllocNone);
    setWindowAttrb.background_pixmap = None;
    setWindowAttrb.border_pixel = 0;
    setWindowAttrb.event_mask = StructureNotifyMask | PointerMotionMask | ButtonPressMask | Button1MotionMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask;

    m_window = XCreateWindow(m_display, RootWindow( m_display, visualInfo->screen),
                             0, 0, info->m_windowWidth, info->m_windowHeight, 0,
                             visualInfo->depth, InputOutput, visualInfo->visual,
                             CWBorderPixel|CWColormap|CWEventMask, &setWindowAttrb);
    if (!m_window) {
        TG_ERROR_LOG("XCreateWindow failed");
        TG_FUNCTION_END();
        return EXIT_FAILURE;
    }

    XFree(visualInfo);
    XStoreName(m_display, m_window, windowTitle);
    XSizeHints hints;
    hints.flags = PMinSize | PMaxSize;
    hints.min_width = info->m_minWidth;
    hints.min_height = info->m_minHeight;
    hints.max_width = info->m_maxWidth;
    hints.max_height = info->m_maxHeight;
    XSetWMNormalHints(m_display, m_window, &hints);

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = nullptr;
    glXCreateContextAttribsARB = reinterpret_cast<glXCreateContextAttribsARBProc>(
                glXGetProcAddressARB(
                reinterpret_cast<const GLubyte *>( "glXCreateContextAttribsARB") ));

    m_context = glXCreateContextAttribsARB(m_display, fbConfic[bestFbcIndex], nullptr,
                                           true, contextAttrbs);

    XSync(m_display, false);
    XFree(fbConfic);
    glXMakeCurrent(m_display, m_window, m_context);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        TG_ERROR_LOG("glewInit() failed");
        TG_FUNCTION_END();
        return EXIT_FAILURE;
    }

	XClearWindow(m_display, m_window);
	XMapRaised(m_display, m_window);
    TG_FUNCTION_END();
    return EXIT_SUCCESS;
}

/*!
 * \brief TgMainWindowX11::setupViewForRender
 */
bool TgMainWindowX11::setupViewForRender()
{
    return true;
}

/*!
 * \brief TgMainWindowX11::renderEnd
 * end of rendering
 * \param info current window info
 */
bool TgMainWindowX11::renderEnd(const TgWindowInfo *info)
{
    glXSwapBuffers (m_display, m_window);
    inputListener(info);
    return true;
}

/*!
 * \brief TgMainWindowX11::inputListener
 * listens input (mouse, key press/release and so on)
 * \param info current window info
 */
void TgMainWindowX11::inputListener(const TgWindowInfo *info)
{
    TG_FUNCTION_BEGIN();
    XEvent xevent;
    TgEventData eventData;

    XWindowAttributes attr;
    XGetWindowAttributes(m_display, m_window, &attr);
    if (attr.width != info->m_windowWidth
        || attr.height != info->m_windowHeight) {
        eventData.m_type = TgEventType::EventTypeWindowResize;
        eventData.m_event.m_windowResize.m_width = attr.width;
        eventData.m_event.m_windowResize.m_height = attr.height;
        m_mainWindowPrivate->addEvent(&eventData);
    }
    while (1) {
        if (XCheckMaskEvent(m_display, PointerMotionMask | ButtonPressMask | Button1MotionMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | ResizeRedirectMask, &xevent)) {
            switch (xevent.type) {
                case MotionNotify:
                    eventData.m_type = TgEventType::EventTypeMouseMove;
                    eventData.m_event.m_mouseEvent.m_x = xevent.xmotion.x;
                    eventData.m_event.m_mouseEvent.m_y = xevent.xmotion.y;
                    eventData.m_event.m_mouseEvent.m_time = m_startTime.elapsedTimeFromBegin();
                    eventData.m_event.m_mouseEvent.m_mouseType = TgMouseType::NoButton;
                    m_mainWindowPrivate->addEvent(&eventData);
                    break;
                case ButtonPress:
                    eventData.m_event.m_mouseEvent.m_mouseType = getButtonType(xevent.xbutton.button);
                    if (eventData.m_event.m_mouseEvent.m_mouseType == TgMouseType::NoButton) {
                        break;
                    }
                    eventData.m_type = TgEventType::EventTypeMousePress;
                    eventData.m_event.m_mouseEvent.m_x = xevent.xmotion.x;
                    eventData.m_event.m_mouseEvent.m_y = xevent.xmotion.y;
                    eventData.m_event.m_mouseEvent.m_time = m_startTime.elapsedTimeFromBegin();
                    m_mainWindowPrivate->addEvent(&eventData);
                    break;
                case ButtonRelease:
                    eventData.m_event.m_mouseEvent.m_mouseType = getButtonType(xevent.xbutton.button);
                    if (eventData.m_event.m_mouseEvent.m_mouseType == TgMouseType::NoButton) {
                        break;
                    }
                    eventData.m_type = TgEventType::EventTypeMouseRelease;
                    eventData.m_event.m_mouseEvent.m_x = xevent.xmotion.x;
                    eventData.m_event.m_mouseEvent.m_y = xevent.xmotion.y;
                    eventData.m_event.m_mouseEvent.m_time = m_startTime.elapsedTimeFromBegin();
                    m_mainWindowPrivate->addEvent(&eventData);
                    break;
                case KeyPress:
                    if (keyCodeToEventDataPress(m_display, xevent.xkey.keycode, eventData)) {
                        m_mainWindowPrivate->addEvent(&eventData);
                    }
                    break;
                case KeyRelease:
                    if (keyCodeToEventDataRelease(m_display, xevent.xkey.keycode, eventData)) {
                        m_mainWindowPrivate->addEvent(&eventData);
                    }
                    break;
                default:
                    break;
            }
        } else {
            break;
        }
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindowX11::getButtonType
 * get mouse button type from button index
 * \param button [in]
 * \return TgMouseType
 */
TgMouseType TgMainWindowX11::getButtonType(unsigned int button)
{
    switch (button) {
        case 1: return TgMouseType::ButtonLeft;
        case 2: return TgMouseType::ButtonMiddle;
        case 3: return TgMouseType::ButtonRight;
        default: break;
    }
    return TgMouseType::NoButton;
}
