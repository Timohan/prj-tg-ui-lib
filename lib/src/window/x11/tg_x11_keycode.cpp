/*!
 * \file
 * \brief file tg_x11_keycode.cpp
 *
 * main window (x11) keycode to keysym class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_x11_keycode.h"
#include <X11/XKBlib.h>
#include "../../global/tg_global_log.h"

TgX11KeyCode::TgX11KeyCode() :
    m_modsKeyDown({false, false, false, false, false, false})
{
}

/*!
 * \brief TgX11KeyCode::keyCodeToKeySym
 * generates key sym from key press
 * \param display [in]
 * \param keyCode [in]
 * \param eventMask [in]
 * \return KeySym
 */
KeySym TgX11KeyCode::keyCodeToKeySym(Display *display, unsigned int keyCode, unsigned int eventMask)
{
    TG_FUNCTION_BEGIN();
    KeySym ret = NoSymbol;
    XkbDescPtr keyboardDesc = XkbGetMap(display, XkbAllClientInfoMask, XkbUseCoreKbd);
    if (!keyboardDesc) {
        return NoSymbol;
    }
    unsigned char groupInfo = XkbKeyGroupInfo(keyboardDesc, keyCode);
    unsigned int numGroups = XkbKeyNumGroups(keyboardDesc, keyCode);
    unsigned int group = 0;

    switch (XkbOutOfRangeGroupAction(groupInfo)) {
        case XkbRedirectIntoRange:
            group = XkbOutOfRangeGroupInfo(groupInfo);
            if (group >= numGroups) {
                group = 0;
            }
            break;

        case XkbClampIntoRange:
            group = numGroups - 1;
            break;

        case XkbWrapIntoRange:
        default:
            if (numGroups != 0) {
                group %= numGroups;
            }
        break;
    }

    XkbKeyTypePtr keyType = XkbKeyKeyType(keyboardDesc, keyCode, group);
    unsigned int activeMods = eventMask & keyType->mods.mask;

    int i, level = 0;
    for (i = 0; i < keyType->map_count; i++) {
        if (keyType->map[i].active && keyType->map[i].mods.mask == activeMods) {
            level = keyType->map[i].level;
        }
    }

    ret = XkbKeySymEntry(keyboardDesc, keyCode, level, group);
    XkbFreeClientMap(keyboardDesc, XkbAllClientInfoMask, true);
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgX11KeyCode::keyCodeToEventDataPress
 * generates event data from key press
 * \param display [in]
 * \param keyCode [in]
 * \param data [out]
 * \return true if event data is filled and it can be send
 */
bool TgX11KeyCode::keyCodeToEventDataPress(Display *display, unsigned int keyCode, TgEventData &data)
{
    TG_FUNCTION_BEGIN();
    unsigned int eventMask = generateEventMask();
    KeySym keySym = keyCodeToKeySym(display, keyCode, eventMask);

    data.m_event.m_keyEvent.m_key = 0;
    data.m_event.m_keyEvent.m_pressReleaseKey = TgPressReleaseKey::PressReleaseKey_NormalKey;
    data.m_event.m_keyEvent.m_previousItem2d = nullptr;
    data.m_type = TgEventType::EventTypeKeyPress;
    switch (keySym) {
        case XK_Tab:
        case XK_ISO_Left_Tab:
            data.m_event.m_keyEvent.m_key = static_cast<uint32_t>('\t');
            data.m_type = TgEventType::EventTypeCharacterCallback;
            break;
        case XK_Return:
            data.m_event.m_keyEvent.m_key = static_cast<uint32_t>('\n');
            break;
        case XK_KP_Enter:
            data.m_event.m_keyEvent.m_key = static_cast<uint32_t>('\n');
            data.m_event.m_keyEvent.m_pressReleaseKey = TgPressReleaseKey::PressReleaseKey_KP_Enter;
            break;
        case XK_Shift_L:
            m_modsKeyDown.m_shiftLeft = true;
            break;
        case XK_Shift_R:
            m_modsKeyDown.m_shiftRight = true;
            break;
        case XK_ISO_Level3_Shift:
            m_modsKeyDown.m_altGr = true;
            break;
        default:
            data.m_event.m_keyEvent.m_key = static_cast<uint32_t>(keySym);
            break;
    }
    data.m_event.m_keyEvent.m_pressModsKeyDown = generateModsKeyDown();
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgX11KeyCode::keyCodeToEventDataRelease
 * generates event data from key release
 * \param display [in]
 * \param keyCode [in]
 * \param data [out]
 * \return true if event data is filled and it can be send
 */
bool TgX11KeyCode::keyCodeToEventDataRelease(Display *display, unsigned int keyCode, TgEventData &data)
{
    TG_FUNCTION_BEGIN();
    unsigned int eventMask = generateEventMask();
    KeySym keySym = keyCodeToKeySym(display, keyCode, eventMask);

    data.m_event.m_keyEvent.m_key = 0;
    data.m_event.m_keyEvent.m_pressReleaseKey = TgPressReleaseKey::PressReleaseKey_NormalKey;
    data.m_event.m_keyEvent.m_previousItem2d = nullptr;
    data.m_type = TgEventType::EventTypeKeyRelease;
    switch (keySym) {
        case XK_Tab:
        case XK_ISO_Left_Tab:
            data.m_event.m_keyEvent.m_key = static_cast<uint32_t>('\t');
            break;
        case XK_Return:
            data.m_event.m_keyEvent.m_key = static_cast<uint32_t>('\n');
            break;
        case XK_KP_Enter:
            data.m_event.m_keyEvent.m_key = static_cast<uint32_t>('\n');
            data.m_event.m_keyEvent.m_pressReleaseKey = TgPressReleaseKey::PressReleaseKey_KP_Enter;
            break;
        case XK_Shift_L:
            m_modsKeyDown.m_shiftLeft = false;
            break;
        case XK_Shift_R:
            m_modsKeyDown.m_shiftRight = false;
            break;
        case XK_ISO_Level3_Shift:
            m_modsKeyDown.m_altGr = false;
            break;
        default:
            data.m_event.m_keyEvent.m_key = static_cast<uint32_t>(keySym);
            break;
    }
    data.m_event.m_keyEvent.m_pressModsKeyDown = generateModsKeyDown();
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgX11KeyCode::generateModsKeyDown
 * generates mods key down for m_pressModsKeyDown (eventData)
 * \return mods key down
 */
int TgX11KeyCode::generateModsKeyDown()
{
    int ret = 0;
    if (m_modsKeyDown.m_shiftLeft || m_modsKeyDown.m_shiftRight) {
        ret |= TgPressModsKeyDown::PressModsKeyDown_Shift;
    }
    if (m_modsKeyDown.m_ctrlLeft || m_modsKeyDown.m_ctrlRight) {
        ret |= TgPressModsKeyDown::PressModsKeyDown_Ctrl;
    }
    if (m_modsKeyDown.m_altGr) {
        ret |= TgPressModsKeyDown::PressModsKeyDown_AltGr;
    }
    if (m_modsKeyDown.m_alt) {
        ret |= TgPressModsKeyDown::PressModsKeyDown_Alt;
    }
    return ret;
}

/*!
 * \brief TgX11KeyCode::generateEventMask
 * generates event mask for function keyCodeToKeySym
 * \return eventMask
 */
unsigned int TgX11KeyCode::generateEventMask()
{
    int ret = 0;
    if (m_modsKeyDown.m_shiftLeft || m_modsKeyDown.m_shiftRight) {
        ret |= ShiftMask;
    }
    if (m_modsKeyDown.m_ctrlLeft || m_modsKeyDown.m_ctrlRight) {
        ret |= ControlMask;
    }
    if (m_modsKeyDown.m_altGr) {
        ret |= Mod5Mask;
    }
    return ret;
}