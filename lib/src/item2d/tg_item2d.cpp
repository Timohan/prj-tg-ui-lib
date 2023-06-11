/*!
 * \file
 * \brief file tg_item2d.cpp
 *
 * it holds general TgItem2d class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_item2d.h"
#include <string.h>
#include "private/item2d/tg_item2d_private.h"
#include "../global/tg_global_log.h"

/*!
 * \brief TgItem2d::TgItem2d
 *
 * constructor to use AnchorFollowParentSize
 *
 * \param parent item's parent
 */
TgItem2d::TgItem2d(TgItem2d *parent) :
    m_private(new TgItem2dPrivate(parent, this))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::TgItem2d
 *
 * constructor to use AnchorRelativeToParent
 *
 * \param parent item's parent
 * \param x item's relative position x (of parent)
 * \param y item's relative position x (of parent)
 * \param width item's width
 * \param height item's height
 */
TgItem2d::TgItem2d(TgItem2d *parent, float x, float y, float width, float height) :
    m_private(new TgItem2dPrivate(x, y, width, height, parent, this, false))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::TgItem2d
 *
 * constructor to use AnchorRelativeToParent
 * this is only called from top menu item creation
 *
 * \param parent item's parent
 * \param x item's relative position x (of parent)
 * \param y item's relative position x (of parent)
 * \param width item's width
 * \param height item's height
 * \param topMenu
 */
TgItem2d::TgItem2d(TgItem2d *parent, float x, float y, float width, float height, bool topMenu) :
    m_private(new TgItem2dPrivate(x, y, width, height, parent, this, topMenu))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgItem2d::~TgItem2d()
{
    TG_FUNCTION_BEGIN();
    if (m_private) {
        delete m_private;
    }
    TG_FUNCTION_END();
}

TgMenuItem *TgItem2d::addMenu(const char *text, const TgShortCut *shortCut)
{
    return m_private->addMenu(nullptr, text, shortCut);
}

/*!
 * \brief TgItem2d::getVisible
 *
 * get visible
 *
 * \return visible
 */
bool TgItem2d::getVisible()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getVisible();
}

/*!
 * \brief TgItem2d::setVisible
 *
 * sets this item visible/hidden
 *
 * \param visible
 */
void TgItem2d::setVisible(bool visible)
{
    TG_FUNCTION_BEGIN();
    m_private->setVisible(visible);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::renderChildren
 *
 * renders children items
 * \param windowInfo
 */
void TgItem2d::renderChildren(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    m_private->renderChildren(windowInfo);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::checkPositionValuesChildren
 *
 * check position values to children
 * \param windowInfo
 */
void TgItem2d::checkPositionValuesChildren(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    m_private->checkPositionValuesChildren(windowInfo);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::render
 *
 * rendering functionality (this is empty)
 * because this is virtual function
 * \param windowInfo
 */
void TgItem2d::render(const TgWindowInfo *)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::checkPositionValues
 *
 * check position values to children (this is empty)
 * because this is virtual function
 * \param windowInfo
 */
void TgItem2d::checkPositionValues()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::sendMessageToChildren
 *
 * sends message to all children
 *
 * \param message message
 */
void TgItem2d::sendMessageToChildren(const TgItem2dPrivateMessage *message)
{
    TG_FUNCTION_BEGIN();
    m_private->sendMessageToChildren(message);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::handleEvent
 *
 * handle the single event
 *
 * \param eventData
 * \param windowInfo
 * \return event result
 */
TgEventResult TgItem2d::handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    m_private->handleEventToolTip(eventData, windowInfo);

    if (((eventData->m_type == TgEventType::EventTypeCharacterCallback 
        && eventData->m_event.m_keyEvent.m_pressReleaseKey == TgPressReleaseKey::PressReleaseKey_NormalKey)
        || eventData->m_type == TgEventType::EventTypeSelectNextItem
        || eventData->m_type == TgEventType::EventTypeSelectLastItem
        || eventData->m_type == TgEventType::EventTypeSelectFirstItem)
            && !getVisible()) {
        TG_FUNCTION_END();
        return TgEventResult::EventResultNotCompleted;
    }
    TgEventResult ret = TgEventResult::EventResultNotCompleted;
    if (m_private->handleEventSelected(eventData, ret)) {
        TG_FUNCTION_END();
        return ret;
    }
    if (eventData->m_type == TgEventType::EventTypeSetItem2dToTop
        && eventData->m_event.m_setItem2dToTop.m_parent == this) {
        m_private->setToTop(eventData->m_event.m_setItem2dToTop.m_child);
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    }
    TG_FUNCTION_END();
    return TgEventResult::EventResultNotCompleted;
}

/*!
 * \brief TgItem2d::handleEventsChildren
 *
 * handles the events for children
 *
 * \param eventData
 * \param windowInfo
 * \return event result
 */
TgEventResult TgItem2d::handleEventsChildren(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->handleEventsChildren(eventData, windowInfo);
}

/*!
 * \brief TgItem2d::addChild
 *
 * add child
 * \param child
 * \param topMenu if true, then this is top menu
 */
void TgItem2d::addChild(TgItem2d *child, bool topMenu)
{
    TG_FUNCTION_BEGIN();
    m_private->addChild(child, topMenu);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::getX
 *
 * get relative position X on parent
 * if you need to know real X position on window,
 * use getXonWindow()
 * \return relative position X
 */
float TgItem2d::getX()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getX();
}

/*!
 * \brief TgItem2d::getY
 *
 * get relative position Y on parent
 * if you need to know real Y position on window,
 * use getYonWindow()
 * \return relative position Y
 */
float TgItem2d::getY()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getY();
}

/*!
 * \brief TgItem2d::getWidth
 *
 * get real width
 *
 * \return width
 */
float TgItem2d::getWidth()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getWidth();
}

/*!
 * \brief TgItem2d::getHeight
 *
 * get real height
 *
 * \return height
 */
float TgItem2d::getHeight()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getHeight();
}

/*!
 * \brief TgItem2d::getXonWindow
 *
 * get real position X (on window)
 * \return real position X (on window)
 */
float TgItem2d::getXonWindow()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getXonWindow();
}

/*!
 * \brief TgItem2d::getYonWindow
 *
 * get real position Y (on window)
 * \return real position Y (on window)
 */
float TgItem2d::getYonWindow()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getYonWindow();
}

/*!
 * \brief TgItem2d::getXminOnVisible
 *
 * get min X position on visible area
 *
 * \return min x position
 */
float TgItem2d::getXminOnVisible()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getXminOnVisible();
}

/*!
 * \brief TgItem2d::getXmaxOnVisible
 *
 * get max X position on visible area
 *
 * \param windowInfo
 * \return max x position
 */
float TgItem2d::getXmaxOnVisible(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getXmaxOnVisible(windowInfo);
}

/*!
 * \brief TgItem2d::setAddMinMaxHeightOnVisible
 *
 * set addon values to calculate min/max height for visible
 * area, this is used with font drawing, so bottom and top
 * areas will be fully visible for example for character j
 *
 * \param addMinHeightOnVisible
 * \param addMaxHeightOnVisible
 */
void TgItem2d::setAddMinMaxHeightOnVisible(
    float addMinHeightOnVisible,
    float addMaxHeightOnVisible)
{
    TG_FUNCTION_BEGIN();
    m_private->setAddMinMaxHeightOnVisible(addMinHeightOnVisible, addMaxHeightOnVisible);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::getYminOnVisible
 *
 * get min Y position on visible area
 *
 * \return min y position
 */
float TgItem2d::getYminOnVisible()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getYminOnVisible();
}

/*!
 * \brief TgItem2d::getYmaxOnVisible
 *
 * get max Y position on visible area
 *
 * \param windowInfo
 * \return max y position
 */
float TgItem2d::getYmaxOnVisible(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getYmaxOnVisible(windowInfo);
}

/*!
 * \brief TgItem2d::setWidth
 *
 * set width for item
 * \param width
 */
void TgItem2d::setWidth(float width)
{
    TG_FUNCTION_BEGIN();
    m_private->setWidth(width);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::setHeight
 *
 * set height for item
 * \param height
 */
void TgItem2d::setHeight(float height)
{
    TG_FUNCTION_BEGIN();
    m_private->setHeight(height);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::setX
 *
 * set x position
 * \param x
 */
void TgItem2d::setX(float x)
{
    TG_FUNCTION_BEGIN();
    m_private->setX(x);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::setY
 *
 * set y position
 * \param y
 */
void TgItem2d::setY(float y)
{
    TG_FUNCTION_BEGIN();
    m_private->setY(y);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::getPositionChanged
 *
 * \return get position change and transform
 * (or vertices) re-do is required
 */
bool TgItem2d::getPositionChanged()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getPositionChanged();
}

/*!
 * \brief TgItem2d::setPositionChanged
 *
 * \param positionChanged true - set position change and transform
 * (or vertices) re-do is required
 */
void TgItem2d::setPositionChanged(bool positionChanged)
{
    TG_FUNCTION_BEGIN();
    m_private->setPositionChanged(positionChanged);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::getAnchorState
 *
 * \return item's anchor state
 */
TgItem2dAnchor TgItem2d::getAnchorState() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getAnchorState();
}

/*!
 * \brief TgItem2d::setAnchorState
 *
 * \param state new item's anchor state
 */
void TgItem2d::setAnchorState(TgItem2dAnchor state)
{
    TG_FUNCTION_BEGIN();
    m_private->setAnchorState(state);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::setMargin
 *
 * set margin left, top, right and bottom values
 * \param left
 * \param top
 * \param right
 * \param bottom
 */
void TgItem2d::setMargin(float left, float top, float right, float bottom)
{
    TG_FUNCTION_BEGIN();
    m_private->setMarginLeft(left);
    m_private->setMarginTop(top);
    m_private->setMarginRight(right);
    m_private->setMarginBottom(bottom);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::setMarginLeft
 *
 * set margin left
 * \param left
 */
void TgItem2d::setMarginLeft(float left)
{
    TG_FUNCTION_BEGIN();
    m_private->setMarginLeft(left);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::setMarginTop
 *
 * set margin top
 * \param top
 */
void TgItem2d::setMarginTop(float top)
{
    TG_FUNCTION_BEGIN();
    m_private->setMarginTop(top);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::setMarginRight
 *
 * set margin right
 * \param right
 */
void TgItem2d::setMarginRight(float right)
{
    TG_FUNCTION_BEGIN();
    m_private->setMarginRight(right);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::setMarginBottom
 *
 * set margin bottom
 * \param bottom
 */
void TgItem2d::setMarginBottom(float bottom)
{
    TG_FUNCTION_BEGIN();
    m_private->setMarginBottom(bottom);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::connectOnResizeChanged
 *
 * set connect resize changed callback
 * \param resizeChanged callback of resize changed
 */
void TgItem2d::connectOnResizeChanged(std::function<void(float x, float y, float width, float height)> resizeChanged)
{
    TG_FUNCTION_BEGIN();
    m_private->connectOnResizeChanged(resizeChanged);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::disconnectOnResizeChanged
 *
 * disconnect resize changed callback
 */
void TgItem2d::disconnectOnResizeChanged()
{
    TG_FUNCTION_BEGIN();
    m_private->disconnectOnResizeChanged();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::connectOnVisibleChanged
 *
 * set connect visible changed callback
 * \param visibleChanged callback of visible changed
 */
void TgItem2d::connectOnVisibleChanged(std::function<void(bool)> visibleChanged)
{
    TG_FUNCTION_BEGIN();
    m_private->connectOnVisibleChanged(visibleChanged);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::disconnectOnVisibleChanged
 *
 * disconnect visible changed callback
 */
void TgItem2d::disconnectOnVisibleChanged()
{
    TG_FUNCTION_BEGIN();
    m_private->disconnectOnVisibleChanged();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::connectOnEnabledChanged
 *
 * set connect enabled changed callback
 * \param enabledChanged callback of enabled changed
 */
void TgItem2d::connectOnEnabledChanged(std::function<void(bool)> enabledChanged)
{
    TG_FUNCTION_BEGIN();
    m_private->connectOnEnabledChanged(enabledChanged);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::disconnectOnEnabledChanged
 *
 * disconnect enabled changed callback
 */
void TgItem2d::disconnectOnEnabledChanged()
{
    TG_FUNCTION_BEGIN();
    m_private->disconnectOnEnabledChanged();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::connectOnSelectedChanged
 *
 * set connect selected changed callback
 * \param selectedChanged callback of selected changed
 */
void TgItem2d::connectOnSelectedChanged(std::function<void(bool selected)> selectedChanged)
{
    TG_FUNCTION_BEGIN();
    m_private->connectOnSelectedChanged(selectedChanged);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::disconnectOnSelectedChanged
 *
 * disconnect selected changed callback
 */
void TgItem2d::disconnectOnSelectedChanged()
{
    TG_FUNCTION_BEGIN();
    m_private->disconnectOnSelectedChanged();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::checkOnResizeChanged
 *
 * checks if position or size is changed, and sends
 * possible resizeChanged callback
 */
void TgItem2d::checkOnResizeChanged()
{
    TG_FUNCTION_BEGIN();
    m_private->checkOnResizeChanged();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::checkOnResizeChangedOnChildren
 *
 * checks if position or size is changed on children
 */
void TgItem2d::checkOnResizeChangedOnChildren()
{
    TG_FUNCTION_BEGIN();
    m_private->checkOnResizeChangedOnChildren();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::getSelected
 *
 * \return is selected
 */
bool TgItem2d::getSelected()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getSelected();
}

/*!
 * \brief TgItem2d::getCanSelect
 *
 * \return can item be selected
 */
bool TgItem2d::getCanSelect()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getCanSelect();
}

/*!
 * \brief TgItem2d::getEnabled
 *
 * \return is enabled
 */
bool TgItem2d::getEnabled()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getEnabled();
}

/*!
 * \brief TgItem2d::setSelected
 *
 * \param selected sets selected true/false
 */
void TgItem2d::setSelected(bool selected)
{
    TG_FUNCTION_BEGIN();
    m_private->setSelected(selected);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::setCanSelect
 *
 * \param canSelect sets "can be selected" true/false
 */
void TgItem2d::setCanSelect(bool canSelect)
{
    TG_FUNCTION_BEGIN();
    m_private->setCanSelect(canSelect);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::setEnabled
 *
 * \param enabled sets enabled true/false
 */
void TgItem2d::setEnabled(bool enabled)
{
    TG_FUNCTION_BEGIN();
    m_private->setEnabled(enabled);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::onEnabledChanged
 *
 * virtual function when enabled changed
 *
 * \param enabled enabled true/false
 */
void TgItem2d::onEnabledChanged(bool enabled)
{
    TG_FUNCTION_BEGIN();
    (void)enabled;
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::onVisibleChanged
 *
 * virtual function when visible changed
 *
 * \param visible visible true/false
 */
void TgItem2d::onVisibleChanged(bool visible)
{
    TG_FUNCTION_BEGIN();
    (void)visible;
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::sendMessageToChildrenFromBegin
 *
 * sends message to all items
 *
 * \param message
 */
void TgItem2d::sendMessageToChildrenFromBegin(const TgItem2dPrivateMessage *message)
{
    TG_FUNCTION_BEGIN();
    handlePrivateMessage(message);
    m_private->sendMessageToChildrenFromBegin(message);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::setToTop
 *
 * changes this child to top (last on the list of children)
 *
 * \param child
 */
void TgItem2d::setToTop()
{
    TG_FUNCTION_BEGIN();
    TgEventData event;
    event.m_type = TgEventType::EventTypeSetItem2dToTop;
    event.m_event.m_setItem2dToTop.m_parent = m_private->m_parent;
    event.m_event.m_setItem2dToTop.m_child = this;
    addNewEvent(&event);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindow::addNewEvent
 *
 * add new event to window list
 *
 * \param event
 */
void TgItem2d::addNewEvent(TgEventData *event)
{
    TG_FUNCTION_BEGIN();
    if (m_private->m_parent) {
        m_private->m_parent->addNewEvent(event);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::handlePrivateMessage
 *
 * virtual function to handle private function
 *
 */
void TgItem2d::handlePrivateMessage(const TgItem2dPrivateMessage *)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::getUseRoundedPositionValues
 * * if true, this item is using rounded values for positional
 * values (x, y, width, height)
 *
 * if true, and x is set to be 64.2, it will be set to 64 (default is true)
 * if false, and x is set to be 64.2, it will be set to 64.2
 *
 * \return true if this item is using rounded values
 */
bool TgItem2d::getUseRoundedPositionValues()
{
    return m_private->getUseRoundedPositionValues();
}

/*!
 * \brief TgItem2d::setUseRoundedPositionValues
 *
 * if true, this item is using rounded values for positional
 * values (x, y, width, height)
 *
 * if true, and x is set to be 64.2, it will be set to 64 (default is true)
 * if false, and x is set to be 64.2, it will be set to 64.2
 *
 * \param useRoundedPositionValues to set if this item is using rounded values
 * or not
 */
void TgItem2d::setUseRoundedPositionValues(bool useRoundedPositionValues)
{
    m_private->setUseRoundedPositionValues(useRoundedPositionValues);
}

/*!
 * \brief TgItem2d::setNextTabItem
 *
 * set item to be selected with tab
 *
 * \param nextTabItem item to be selected with tab
 * NOTE: this item must be selectable
 */
void TgItem2d::setNextTabItem(TgItem2d *nextTabItem)
{
    TG_FUNCTION_BEGIN();
    m_private->setNextTabItem(nextTabItem);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::setPrevTabItem
 *
 * set item to be selected with shift+tab
 *
 * \param prevTabItem item to be selected with shift+tab
 * NOTE: this item must be selectable
 */
void TgItem2d::setPrevTabItem(TgItem2d *prevTabItem)
{
    TG_FUNCTION_BEGIN();
    m_private->setPrevTabItem(prevTabItem);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2d::getMarginLeft
 *
 * \return left margin
 */
float TgItem2d::getMarginLeft()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getMarginLeft();
}

/*!
 * \brief TgItem2d::getMarginTop
 *
 * \return top margin
 */
float TgItem2d::getMarginTop()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getMarginTop();
}

/*!
 * \brief TgItem2d::getMarginRight
 *
 * \return right margin
 */
float TgItem2d::getMarginRight()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getMarginRight();
}

/*!
 * \brief TgItem2d::getMarginBottom
 *
 * \return bottom margin
 */
float TgItem2d::getMarginBottom()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getMarginBottom();
}

/*!
 * \brief TgItem2d::getMenuCount
 *
 * \return count of menu items
*/
size_t TgItem2d::getMenuCount()
{
    return m_private->getMenuCount();
}

/*!
 * \brief TgItem2d::getMenu
 *
 * \param index
 * \return pointer of menu item
 * if index >= list of menu items, then return nullptr
 */
TgMenuItem *TgItem2d::getMenu(size_t index)
{
    return m_private->getMenu(index);
}

/*!
 * \brief TgItem2d::removeMenu
 *
 * \param i index of sub menu
 * \return true if sub menu was removed
 */
bool TgItem2d::removeMenu(size_t i)
{
    return m_private->removeSubMenu(i);
}

/*!
 * \brief TgItem2d::setTooltip
 *
 * set tooltip text
 *
 * \return text
 */
void TgItem2d::setTooltip(const char *text)
{
    m_private->setTooltip(text);
}

/*!
 * \brief TgItem2d::deleteLater
 *
 * set item to delete later
 * - use this to delete items
 */
void TgItem2d::deleteLater()
{
    m_private->deleteLater();
}
