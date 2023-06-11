/*!
 * \file
 * \brief file tg_event_data.h
 *
 * Event data
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_EVENT_DATA_H
#define TG_EVENT_DATA_H

#include <stdint.h>

class TgItem2d;

/*!
 * \brief TgEventResult
 * event result that is returned handleEvent()
 */
enum TgEventResult
{
    EventResultNotCompleted = 0,    /**< event is not completed handleEvent() function */
    EventResultCompleted            /**< event is completed in handleEvent() function */
};

/*!
 * \brief TgEventType
 * defines the main event type
 */
enum TgEventType {
    EventTypeMousePress = 0,        /**< mouse button is pressed */
    EventTypeMouseRelease,          /**< mouse button is released */
    EventTypeMouseMove,             /**< mouse button is moved */
    EventTypeMouseMoveForMenuParent,/**< mouse button is moved - but for parent menu only */
    EventTypeMouseMoveForSubMenu,   /**< mouse button is moved - checks if any sub menu needs to be opened or closed */
    EventTypeMouseMoveResend,       /**< sending previous mouse button move coordinates */
    EventTypeMouseScrollMove,       /**< mouse button scroll move */
    EventTypeKeyPress,              /**< keyboard press */
    EventTypeKeyRepeat,             /**< keyboard repeat (key is pressed down long time and during this time, number of repeat callbacks happens) */
    EventTypeKeyRelease,            /**< keyboard released */
    EventTypeCharacterCallback,     /**< keyboard character, defines the difference between capital and lower case characters */
    EventTypeCharacterCallbackShortCut,     /**< keyboard character, defines the difference between capital and lower case characters, this is only used for shortcuts, such as menu items */
    EventTypeSelectFirstItem,       /**< changes (select) to next item (tab key press) */
    EventTypeSelectNextItem,        /**< changes (select) to next item (tab key press) */
    EventTypeSelectLastItem,        /**< changes (select) to last item (shift-tab key press) */
    EventTypeWindowResize,          /**< when window resize this event is called */
    EventTypeSetItem2dToTop,        /**< when this item is set to top of parent, this is called */
    EventTypeDeleteLater,           /**< Handle delete later */
};

/*!
 * \brief TgMouseType
 * mouse button type on m_mouseEvent
 */
enum TgMouseType
{
    NoButton = 0,     /**< no button, this can be used on scroll move */
    ButtonLeft,       /**< left mouse button */
    ButtonRight,      /**< right mouse button */
    ButtonMiddle,     /**< middle mouse button */
    ButtonBack = 8,   /**< back mouse button */
};

/*!
 * \brief TgPressReleaseKey
 * this can define the actual keyboard press key
 * for example, normal enter is PressReleaseKey_NormalKey and m_key = '\n'
 * but keypad enter is PressReleaseKey_KP_Enter
 */
enum TgPressReleaseKey
{
    PressReleaseKey_NormalKey = 0, /**< normal key */
    PressReleaseKey_KP_Enter,      /**< keypad enter */
    PressReleaseKey_Key_Left,      /**< arrow key left */
    PressReleaseKey_Key_Right,     /**< arrow key right */
    PressReleaseKey_Key_Up,        /**< arrow key up */
    PressReleaseKey_Key_Down,      /**< arrow key down */
    PressReleaseKey_Key_Backspace, /**< key backspace */
    PressReleaseKey_Key_Delete,    /**< key del */
};

/*!
 * \brief TgPressModsKeyDown
 * This will define if any mods key is down (shift, alt, ctrl...)
 */
enum TgPressModsKeyDown
{
    PressModsKeyDown_NoKey    = 0,             /**< No mods key down */
    PressModsKeyDown_Shift    = (0x1 << 1),    /**< shift mods key down */
    PressModsKeyDown_Alt      = (0x1 << 2),    /**< alt mods key down */
    PressModsKeyDown_Ctrl     = (0x1 << 3),    /**< ctrl mods key down */
    PressModsKeyDown_AltGr    = (0x1 << 4),    /**< alt gr mods key down */
};

/*!
 * \brief TgEventData
 * Event data, when events are called, this is the struct
 * that contains all information for event
 */
struct TgEventData
{
    TgEventType m_type;                             /**< event type, this defines which union is used */
    union {
        struct {
            double m_x;                             /**< current x position of mouse cursor */
            double m_y;                             /**< current y position of mouse cursor */
            double m_scroll_move_x;                 /**< current x position of mouse scroll move */
            double m_scroll_move_y;                 /**< current y position of mouse scroll move */
            double m_time;                          /**< time when mouse event was first time set */
            TgMouseType m_mouseType;                /**< mouse event type */
            TgItem2d *m_newItem;                    /**< this must be filled by the item in handleEvent
                                                     * function when item2d locks itself to using mouse press
                                                     * and then mouse move and mouse release in handleEvent
                                                     * knows which item is using the mouse press and mouse move
                                                     * events */
            TgItem2d *m_currentMouseDownItem;       /**< this will be filled in mouse move and mouse release event
                                                     * types. it's pointer to item that is locked by the mouse press
                                                     * - on mouse move it can be nullptr if no item is locked */
            bool m_releaseWoCallback;                /**< if true, when this is release, but not send click (nor swipe or similar) callback, but it sends release cb */
        } m_mouseEvent;
        struct {
            uint32_t m_key;                         /**< key */
            TgPressReleaseKey m_pressReleaseKey;    /**< if this is PressReleaseKey_NormalKey, then only m_key is used */
            int m_pressModsKeyDown;                 /**< Sets which mod(s) key are down */
            TgItem2d *m_previousItem2d;              /**< item of item2d */
        } m_keyEvent;
        struct {
            TgItem2d *m_previousItem2d;             /**< item of current selected item2d */
            TgItem2d *m_nextItem2d;                 /**< item of next selected item2d */
        } m_selectLastItem;
        struct {
            int32_t m_width;                        /**< new width of application */
            int32_t m_height;                       /**< new height of application */
        } m_windowResize;
        struct {
            TgItem2d *m_parent;                     /**< parent item of item that will be set to top */
            TgItem2d *m_child;                      /**< this item will be top (on parent item) */
        } m_setItem2dToTop;
    } m_event;                                      /**< event (union) */
};

#endif //TG_EVENT_DATA_H
