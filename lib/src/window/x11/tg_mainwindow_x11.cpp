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
#include <thread>
#include "../tg_mainwindow_private.h"
#include "../../global/tg_global_log.h"
#include "../../global/tg_global_application.h"
#include "../../global/private/tg_global_wait_renderer.h"

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

TgMainWindowX11::TgMainWindowX11(TgMainWindowPrivate *mainWindowPrivate) :
    m_mainWindowPrivate(mainWindowPrivate),
    m_display(nullptr)
{
    XInitThreads();
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
    TgGlobalWaitRenderer::getInstance()->waitForRender();
    return true;
}

/*!
 * \brief TgMainWindowX11::renderEnd
 * end of rendering
 */
bool TgMainWindowX11::renderEnd()
{
    glXSwapBuffers (m_display, m_window);
    m_mutex.lock();
    if (m_threadIsRunning == TgListenerX11State::NotRunning) {
        m_threadIsRunning = TgListenerX11State::Running;
        m_mutex.unlock();
        std::thread([this]() {
            inputListener();
        }).detach();
    } else {
        m_mutex.unlock();
    }
    return true;
}

/*!
 * \brief TgMainWindowX11::waitForEnd
 * when application is ending, this function is called
 * and stops X11 listener thread nicely and waits for it
 */
void TgMainWindowX11::waitForEnd()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    if (m_threadIsRunning == TgListenerX11State::NotRunning) {
        m_mutex.unlock();
        TG_FUNCTION_END();
        return;
    }
    m_threadIsRunning = TgListenerX11State::EndRunning;
    m_mutex.unlock();
    while (1) {
        sendEndMsg();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        m_mutex.lock();
        if (m_threadIsRunning == TgListenerX11State::EndedRunning) {
            m_mutex.unlock();
            break;
        }
        m_mutex.unlock();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    TG_FUNCTION_END();
}

void TgMainWindowX11::sendEndMsg()
{
    std::thread([this]() {
        XKeyEvent event;
        event.display     = m_display;
        event.window      = m_window;
        event.root        = m_window;
        event.subwindow   = None;
        event.time        = CurrentTime;
        event.x           = 1;
        event.y           = 1;
        event.x_root      = 1;
        event.y_root      = 1;
        event.same_screen = True;
        event.keycode     = 68;
        event.state       = 0;
        event.type = KeyPress;
        XSendEvent(m_display, m_window, True, 0xfff, reinterpret_cast<XEvent *>(&event));
    }).detach();
}

int TgMainWindowX11::waitForMsg(int fd)
{
    fd_set fdSet;
    struct timeval tv;
    FD_ZERO(&fdSet);
    FD_SET(fd, &fdSet);
    tv.tv_usec = 100000;
    tv.tv_sec = 0;
    return select(fd+1, &fdSet, NULL, NULL, &tv);
}

/*!
 * \brief TgMainWindowX11::inputListener
 * listens input (mouse, key press/release and so on)
 */
void TgMainWindowX11::inputListener()
{
    TG_FUNCTION_BEGIN();
    XEvent xevent;
    TgEventData eventData;
    while (1) {
        m_mutex.lock();
        if (m_threadIsRunning != TgListenerX11State::Running) {
            m_mutex.unlock();
            break;
        }
        m_mutex.unlock();
        waitForMsg(ConnectionNumber(m_display));
        if (XCheckMaskEvent(m_display, StructureNotifyMask | PointerMotionMask | ButtonPressMask | Button1MotionMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | ResizeRedirectMask, &xevent)) {
        m_mutex.lock();
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
            case ConfigureNotify: {
                    XConfigureEvent confEvent = xevent.xconfigure;
                    eventData.m_type = TgEventType::EventTypeWindowResize;
                    eventData.m_event.m_windowResize.m_width = confEvent.width;
                    eventData.m_event.m_windowResize.m_height = confEvent.height;
                    m_mainWindowPrivate->addEvent(&eventData);
                }
                break;
            default:
                break;
        }
        m_mutex.unlock();
        }
    }
    m_mutex.lock();
    m_threadIsRunning = TgListenerX11State::EndedRunning;
    m_mutex.unlock();
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

#ifdef FUNCIONAL_TEST
Display *TgMainWindowX11::getDisplay()
{
    return m_display;
}
Window *TgMainWindowX11::getWindow()
{
    return &m_window;
}
#endif

