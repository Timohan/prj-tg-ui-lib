/*!
 * \file
 * \brief file tg_glfw_input.cpp
 *
 * Handles inputs from glfw
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_glfw_input.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include "../../global/tg_global_log.h"
#include "../../global/tg_global_application.h"
#include "../../event/tg_events.h"

static TgGlfwInput *m_TgGlfwInput = nullptr;

TgGlfwInput *TgGlfwInput::instance()
{
    TG_FUNCTION_BEGIN();
    if (!m_TgGlfwInput) {
        m_TgGlfwInput = new TgGlfwInput();
    }
    TG_FUNCTION_END();
    return m_TgGlfwInput;
}

TgGlfwInput::TgGlfwInput() :
    m_mousePositionX(-1),
    m_mousePositionY(-1)
#ifdef GLFW_REPEAT_CAN_CAUSE_EXTRA_PRESS_RELEASE_AFTER_REPEAT
    , m_ignoreNextPress(false),
    m_ignoreNextRelease(false),
    m_previousWasRepeat(false)
#endif
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlfwInput::clear
 *
 * clears glfw input
 */
void TgGlfwInput::clear()
{
    TG_FUNCTION_BEGIN();
    if (TgGlfwInput::instance()) {
        delete TgGlfwInput::instance();
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlfwInput::cursor_position_callback
 *
 * static function - mouse position callback from glfw
 *
 * \param window mainwindow's window
 * \param xpos x position
 * \param ypos y position
 */
void TgGlfwInput::cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    TG_FUNCTION_BEGIN();
    TgGlfwInput::instance()->cursorPositionCallback(window, xpos, ypos);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlfwInput::mouse_button_callback
 *
 * static function - mouse click/release callback from glfw
 *
 * \param window mainwindow's window
 * \param button button type
 * \param action action type
 */
void TgGlfwInput::mouse_button_callback(GLFWwindow *window, int button, int action, int)
{
    TG_FUNCTION_BEGIN();
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        TgGlfwInput::instance()->mousePress(window, TgMouseType::ButtonLeft);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        TgGlfwInput::instance()->mouseRelease(window, TgMouseType::ButtonLeft);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        TgGlfwInput::instance()->mousePress(window, TgMouseType::ButtonRight);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        TgGlfwInput::instance()->mouseRelease(window, TgMouseType::ButtonRight);
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        TgGlfwInput::instance()->mousePress(window, TgMouseType::ButtonMiddle);
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
        TgGlfwInput::instance()->mouseRelease(window, TgMouseType::ButtonMiddle);
    }
    if (button == GLFW_MOUSE_BUTTON_LAST && action == GLFW_PRESS) {
        TgGlfwInput::instance()->mousePress(window, TgMouseType::ButtonBack);
    }
    if (button == GLFW_MOUSE_BUTTON_LAST && action == GLFW_RELEASE) {
        TgGlfwInput::instance()->mouseRelease(window, TgMouseType::ButtonBack);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlfwInput::scroll_callback
 *
 * static function - callback from glfw
 *
 * \param window mainwindow's window
 * \param xoffset moving x scroll position
 * \param yoffset moving y scroll position
 */
void TgGlfwInput::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    TG_FUNCTION_BEGIN();
    TgGlfwInput::instance()->scrollMove(window, xoffset, yoffset);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlfwInput::scrollMove
 *
 * function -  callback from glfw
 *
 * \param window mainwindow's window
 * \param xoffset moving x scroll position
 * \param yoffset moving y scroll position
 */
void TgGlfwInput::scrollMove(GLFWwindow *window, double xoffset, double yoffset)
{
    TG_FUNCTION_BEGIN();
    if (m_mousePositionX >= 0) {
        TgEventData eventData;
        eventData.m_type = TgEventType::EventTypeMouseScrollMove;
        eventData.m_event.m_mouseEvent.m_x = m_mousePositionX;
        eventData.m_event.m_mouseEvent.m_y = m_mousePositionY;
        eventData.m_event.m_mouseEvent.m_scroll_move_x = xoffset;
        eventData.m_event.m_mouseEvent.m_scroll_move_y = yoffset;
        eventData.m_event.m_mouseEvent.m_time = m_startTime.elapsedTimeFromBegin();
        eventData.m_event.m_mouseEvent.m_mouseType = TgMouseType::NoButton;
        TgGlobalApplication::getInstance()->addEvent(window, &eventData);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlfwInput::cursorPositionCallback
 *
 * cursor position callback from glfw (cursor_position_callback)
 *
 * \param window mainwindow's window
 * \param xpos x position
 * \param ypos y position
 */
void TgGlfwInput::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    TG_FUNCTION_BEGIN();
    m_mousePositionX = xpos;
    m_mousePositionY = ypos;
    TgEventData eventData;
    eventData.m_type = TgEventType::EventTypeMouseMove;
    eventData.m_event.m_mouseEvent.m_x = m_mousePositionX;
    eventData.m_event.m_mouseEvent.m_y = m_mousePositionY;
    eventData.m_event.m_mouseEvent.m_time = m_startTime.elapsedTimeFromBegin();
    eventData.m_event.m_mouseEvent.m_mouseType = TgMouseType::NoButton;
    TgGlobalApplication::getInstance()->addEvent(window, &eventData);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlfwInput::getMousePressed
 *
 * \param type button type
 * \return true if this this mouse type was is pressed down
 */
bool TgGlfwInput::getMousePressed(TgMouseType type)
{
    return std::find(m_listMouseTypeDown.begin(), m_listMouseTypeDown.end(), type) != m_listMouseTypeDown.end();
}

/*!
 * \brief TgGlfwInput::clearMousePressed
 *
 * clears mouse pressed for mouse type
 * \param type
 *
 */
void TgGlfwInput::clearMousePressed(TgMouseType type)
{
    std::vector<TgMouseType>::iterator it = std::find(m_listMouseTypeDown.begin(), m_listMouseTypeDown.end(), type);
    m_listMouseTypeDown.erase(it);
}

/*!
 * \brief TgGlfwInput::mousePress
 *
 * button press
 *
 * \param window mainwindow's window
 * \param mouseType mouse type
 */
void TgGlfwInput::mousePress(GLFWwindow *window, TgMouseType mouseType)
{
    TG_FUNCTION_BEGIN();
    if (m_mousePositionX >= 0) {
        if (!getMousePressed(mouseType)) {
            m_listMouseTypeDown.push_back(mouseType);
        }
        TgEventData eventData;
        eventData.m_type = TgEventType::EventTypeMousePress;
        eventData.m_event.m_mouseEvent.m_x = m_mousePositionX;
        eventData.m_event.m_mouseEvent.m_y = m_mousePositionY;
        eventData.m_event.m_mouseEvent.m_time = m_startTime.elapsedTimeFromBegin();
        eventData.m_event.m_mouseEvent.m_mouseType = mouseType;
        TgGlobalApplication::getInstance()->addEvent(window, &eventData);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlfwInput::mouseRelease
 *
 * button release
 *
 * \param window mainwindow's window
 * \param mouseType mouse type
 */
void TgGlfwInput::mouseRelease(GLFWwindow *window, TgMouseType mouseType)
{
    TG_FUNCTION_BEGIN();
    if (getMousePressed(mouseType)) {
        clearMousePressed(mouseType);
        TgEventData eventData;
        eventData.m_type = TgEventType::EventTypeMouseRelease;
        eventData.m_event.m_mouseEvent.m_x = m_mousePositionX;
        eventData.m_event.m_mouseEvent.m_y = m_mousePositionY;
        eventData.m_event.m_mouseEvent.m_time = m_startTime.elapsedTimeFromBegin();
        eventData.m_event.m_mouseEvent.m_mouseType = mouseType;
        TgGlobalApplication::getInstance()->addEvent(window, &eventData);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlfwInput::keyboard_callback
 *
 * keyboard callback from glfw
 *
 * \param window mainwindow's window
 * \param key key
 * \param scancode scancode
 * \param action action
 * \param mods mods
 */
void TgGlfwInput::keyboard_callback(GLFWwindow* window,
                                    int key,
                                    int /* scancode */,
                                    int action,
                                    int mods
                                   )
{
    TG_FUNCTION_BEGIN();
    TgGlfwInput::instance()->keyboardCallback(window, key, action, mods);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlfwInput::keyboardCallback
 *
 * keyboard callback
 *
 * \param window mainwindow's window
 * \param key key
 * \param action action
 */
void TgGlfwInput::keyboardCallback(GLFWwindow* window, int key, int action, int mods)
{
    TG_FUNCTION_BEGIN();
    TgEventData eventData;
    m_mutex.lock();
    switch (action) {
        case GLFW_RELEASE:
            eventData.m_type = TgEventType::EventTypeKeyRelease;
#ifdef GLFW_REPEAT_CAN_CAUSE_EXTRA_PRESS_RELEASE_AFTER_REPEAT
            if (m_ignoreNextRelease && m_lastRepeat.elapsedTimeFromBegin() < 0.05) {
                m_ignoreNextRelease = false;
                m_mutex.unlock();
                TG_FUNCTION_END();
                return;
            }
            if (m_previousWasRepeat) {
                m_ignoreNextPress = true;
                m_ignoreNextRelease = true;
                m_previousWasRepeat = false;
            } else {
                m_ignoreNextRelease = false;
            }
#endif
            break;
        case GLFW_PRESS:
            eventData.m_type = TgEventType::EventTypeKeyPress;
#ifdef GLFW_REPEAT_CAN_CAUSE_EXTRA_PRESS_RELEASE_AFTER_REPEAT
            if (m_ignoreNextPress && m_lastRepeat.elapsedTimeFromBegin() < 0.05) {
                m_ignoreNextPress = false;
                m_mutex.unlock();
                TG_FUNCTION_END();
                return;
            }
            m_ignoreNextPress = false;
#endif
            break;
        case GLFW_REPEAT:
            eventData.m_type = TgEventType::EventTypeKeyRepeat;
#ifdef GLFW_REPEAT_CAN_CAUSE_EXTRA_PRESS_RELEASE_AFTER_REPEAT
            m_previousWasRepeat = true;
            m_lastRepeat.resetTimer();
#endif
            break;
        default:
            m_mutex.unlock();
            TG_FUNCTION_END();
            return;
    }

    eventData.m_event.m_keyEvent.m_pressReleaseKey = TgPressReleaseKey::PressReleaseKey_NormalKey;
    eventData.m_event.m_keyEvent.m_previousItem2d = nullptr;
    eventData.m_event.m_keyEvent.m_pressModsKeyDown = TgPressModsKeyDown::PressModsKeyDown_NoKey;
    if (mods & GLFW_MOD_SHIFT) {
        eventData.m_event.m_keyEvent.m_pressModsKeyDown |= static_cast<int>(TgPressModsKeyDown::PressModsKeyDown_Shift);
    }
    if (mods & GLFW_MOD_ALT) {
        eventData.m_event.m_keyEvent.m_pressModsKeyDown |= static_cast<int>(TgPressModsKeyDown::PressModsKeyDown_Alt);
    }
    if (mods & GLFW_MOD_CONTROL) {
        eventData.m_event.m_keyEvent.m_pressModsKeyDown |= static_cast<int>(TgPressModsKeyDown::PressModsKeyDown_Ctrl);
    }
    switch (key) {
        case GLFW_KEY_LEFT_SHIFT:
            if (action == GLFW_RELEASE) {
                m_modsKeyDown.m_shiftLeft = false;
            } else {
                m_modsKeyDown.m_shiftLeft = true;
            }
            break;
        case GLFW_KEY_RIGHT_SHIFT:
            if (action == GLFW_RELEASE) {
                m_modsKeyDown.m_shiftRight = false;
            } else {
                m_modsKeyDown.m_shiftRight = true;
            }
            break;
        case GLFW_KEY_LEFT_ALT:
            if (action == GLFW_RELEASE) {
                m_modsKeyDown.m_altLeft = false;
            } else {
                m_modsKeyDown.m_altLeft = true;
            }
            break;
        case GLFW_KEY_RIGHT_ALT:
            if (action == GLFW_RELEASE) {
                m_modsKeyDown.m_altRight = false;
            } else {
                m_modsKeyDown.m_altRight = true;
            }
            break;
        case GLFW_KEY_LEFT_CONTROL:
            if (action == GLFW_RELEASE) {
                m_modsKeyDown.m_ctrlLeft = false;
            } else {
                m_modsKeyDown.m_ctrlLeft = true;
            }
            break;
        case GLFW_KEY_RIGHT_CONTROL:
            if (action == GLFW_RELEASE) {
                m_modsKeyDown.m_ctrlRight = false;
            } else {
                m_modsKeyDown.m_ctrlRight = true;
            }
            break;
        case GLFW_KEY_LEFT:
            eventData.m_event.m_keyEvent.m_key = 0;
            eventData.m_event.m_keyEvent.m_pressReleaseKey = TgPressReleaseKey::PressReleaseKey_Key_Left;
            TgGlobalApplication::getInstance()->addEvent(window, &eventData);
            return;
        case GLFW_KEY_RIGHT:
            eventData.m_event.m_keyEvent.m_key = 0;
            eventData.m_event.m_keyEvent.m_pressReleaseKey = TgPressReleaseKey::PressReleaseKey_Key_Right;
            TgGlobalApplication::getInstance()->addEvent(window, &eventData);
            return;
        case GLFW_KEY_UP:
            eventData.m_event.m_keyEvent.m_key = 0;
            eventData.m_event.m_keyEvent.m_pressReleaseKey = TgPressReleaseKey::PressReleaseKey_Key_Up;
            TgGlobalApplication::getInstance()->addEvent(window, &eventData);
            return;
        case GLFW_KEY_DOWN:
            eventData.m_event.m_keyEvent.m_key = 0;
            eventData.m_event.m_keyEvent.m_pressReleaseKey = TgPressReleaseKey::PressReleaseKey_Key_Down;
            TgGlobalApplication::getInstance()->addEvent(window, &eventData);
            return;
        case GLFW_KEY_BACKSPACE:
            eventData.m_event.m_keyEvent.m_key = 0;
            eventData.m_event.m_keyEvent.m_pressReleaseKey = TgPressReleaseKey::PressReleaseKey_Key_Backspace;
            TgGlobalApplication::getInstance()->addEvent(window, &eventData);
            return;
        case GLFW_KEY_DELETE:
            eventData.m_event.m_keyEvent.m_key = 0;
            eventData.m_event.m_keyEvent.m_pressReleaseKey = TgPressReleaseKey::PressReleaseKey_Key_Delete;
            TgGlobalApplication::getInstance()->addEvent(window, &eventData);
            return;
        case GLFW_KEY_TAB:
            eventData.m_event.m_keyEvent.m_key = static_cast<uint32_t>('\t');
            TgGlobalApplication::getInstance()->addEvent(window, &eventData);
            if (eventData.m_type == TgEventType::EventTypeKeyRelease) {
                eventData.m_type = TgEventType::EventTypeCharacterCallback;
                eventData.m_event.m_keyEvent.m_previousItem2d = nullptr;
                eventData.m_event.m_keyEvent.m_pressReleaseKey = TgPressReleaseKey::PressReleaseKey_NormalKey;
                TgGlobalApplication::getInstance()->addEvent(window, &eventData);
            }
            m_mutex.unlock();
            return;
        case GLFW_KEY_ENTER:
            eventData.m_event.m_keyEvent.m_key = static_cast<uint32_t>('\n');
            TgGlobalApplication::getInstance()->addEvent(window, &eventData);
            if (eventData.m_type == TgEventType::EventTypeKeyRelease) {
                character_callback(window, static_cast<unsigned int>('\n'));
            }
        break;
        case GLFW_KEY_KP_ENTER:
            eventData.m_event.m_keyEvent.m_key = 0;
            eventData.m_event.m_keyEvent.m_pressReleaseKey = TgPressReleaseKey::PressReleaseKey_KP_Enter;
            TgGlobalApplication::getInstance()->addEvent(window, &eventData);
            if (eventData.m_type == TgEventType::EventTypeKeyRelease) {
                character_callback(window, static_cast<unsigned int>('\n'));
            }
        break;
        default:
            eventData.m_event.m_keyEvent.m_key = static_cast<uint32_t>(key);
            TgGlobalApplication::getInstance()->addEvent(window, &eventData);
        break;
    }
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlfwInput::character_callback
 *
 * character callback from glfw
 *
 * \param window mainwindow's window
 * \param codepoint codepoint
 */
void TgGlfwInput::character_callback(GLFWwindow* window, unsigned int codepoint)
{
    TG_FUNCTION_BEGIN();
    TgEventData eventData;
    eventData.m_type = TgEventType::EventTypeCharacterCallback;
    eventData.m_event.m_keyEvent.m_previousItem2d = nullptr;
    eventData.m_event.m_keyEvent.m_pressReleaseKey = TgPressReleaseKey::PressReleaseKey_NormalKey;
    eventData.m_event.m_keyEvent.m_key = static_cast<uint32_t>(codepoint);
    TgGlobalApplication::getInstance()->addEvent(window, &eventData);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlfwInput::window_resize
 *
 * window resize callback from glfw
 *
 * \param window mainwindow's window
 * \param width screen width
 * \param height screen height
 */
void TgGlfwInput::window_resize(GLFWwindow* window, int width, int height)
{
    TG_FUNCTION_BEGIN();
    TgEventData eventData;
    eventData.m_type = TgEventType::EventTypeWindowResize;
    eventData.m_event.m_windowResize.m_width = width;
    eventData.m_event.m_windowResize.m_height = height;
    TgGlobalApplication::getInstance()->addEvent(window, &eventData);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlfwInput::setup
 *
 * setup callbacks for this window
 *
 * \param window mainwindow's window
 */
void TgGlfwInput::setup(GLFWwindow *window)
{
    TG_FUNCTION_BEGIN();
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetWindowSizeCallback(window, window_resize);
    TG_FUNCTION_END();
}

