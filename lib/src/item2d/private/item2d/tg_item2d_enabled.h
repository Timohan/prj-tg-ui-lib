/*!
 * \file
 * \brief file tg_item2d_enabled.h
 *
 * it holds enabled values for TgItem2dPrivate
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_ITEM_2D_PRIVATE_ENABLED_H
#define TG_ITEM_2D_PRIVATE_ENABLED_H

#include <functional>
#include <mutex>

enum TgItem2dEnabledState
{
    TgItem2dEnabled = 0,
    TgItem2dDisabled,
    TgItem2dEnabledButParentDisabled
};

class TgItem2d;
class TgItem2dPrivate;

class TgItem2dEnabled
{
public:
    TgItem2dEnabled(TgItem2d *parent, TgItem2dPrivate *currentItem2dPrivate);

    bool getEnabled();
    void setEnabled(bool enabled);
    bool parentEnabledChanged(bool enabled);

    void connectOnEnabledChanged(std::function<void(bool)> enabledChanged);
    void disconnectOnEnabledChanged();
    void setRequireRecheckEnabledChangeToChildren(bool require);
protected:
    void reCheckChildrenEnabled();

private:
    TgItem2d *m_parent;
    TgItem2dPrivate *m_currentItem2dPrivate;
    TgItem2dEnabledState m_enabledState;
    bool m_requireRecheckEnabledChangeToChildren;
    std::mutex m_mutex;

    std::function<void(bool enabled)> f_enabledChanged;
};

#endif // TG_ITEM_2D_PRIVATE_ENABLED_H
