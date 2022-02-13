/*!
 * \file
 * \brief file tg_glfw_input.h
 *
 * Handles inputs from glfw
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_GLFW_INPUT_H
#define TG_GLFW_INPUT_H

struct GLFWwindow;
#include "../event/tg_event_data.h"
#include "../common/time_difference.h"
#include <mutex>
#include <vector>

#define GLFW_REPEAT_CAN_CAUSE_EXTRA_PRESS_RELEASE_AFTER_REPEAT

struct TgGlfwInputModsKeyDown
{
    bool m_shiftLeft  : 1;  /**< left shift down or up */
    bool m_shiftRight : 1;  /**< right shift down or up */
    bool m_ctrlLeft   : 1;  /**< left ctrl down or up */
    bool m_ctrlRight  : 1;  /**< right ctrl down or up */
    bool m_altLeft    : 1;  /**< left alt down or up */
    bool m_altRight   : 1;  /**< right alt down or up */
};

class TgGlfwInput
{
public:
    TgGlfwInput();
    static TgGlfwInput *instance();
    static void clear();

    void setup(GLFWwindow *window);
private:
    double m_mousePositionX;
    double m_mousePositionY;
    TimeDifference m_startTime;
#ifdef GLFW_REPEAT_CAN_CAUSE_EXTRA_PRESS_RELEASE_AFTER_REPEAT
    TimeDifference m_lastRepeat;
    bool m_ignoreNextPress;
    bool m_ignoreNextRelease;
    bool m_previousWasRepeat;
#endif
    std::recursive_mutex m_mutex;
    std::vector<TgMouseType>m_listMouseTypeDown;
    TgGlfwInputModsKeyDown m_modsKeyDown;

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void character_callback(GLFWwindow* window, unsigned int codepoint);
    static void window_resize(GLFWwindow* window, int width, int height);

    void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
    void mousePress(GLFWwindow *window, TgMouseType mouseType);
    void mouseRelease(GLFWwindow *window, TgMouseType mouseType);
    void scrollMove(GLFWwindow *window, double xoffset, double yoffset);
    void keyboardCallback(GLFWwindow* window, int key, int action, int mods);

    bool getMousePressed(TgMouseType type);
    void clearMousePressed(TgMouseType type);
};

#endif //TG_GLFW_INPUT_H
