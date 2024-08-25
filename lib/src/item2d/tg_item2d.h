/*!
 * \file
 * \brief file tg_item2d.h
 *
 * it holds general TgItem2d class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_ITEM_2D_H
#define TG_ITEM_2D_H

#include <vector>
#include <cstddef>
#include <functional>
#include <string>
#include "../event/tg_event_data.h"
class TgItem2dPrivate;
struct TgWindowInfo;
struct TgItem2dPrivateMessage;
class TgMenuItem;
struct TgShortCut;

/*!
 * \brief TgItem2dAnchor
 * anchor type of item
 */
enum TgItem2dAnchor {
    AnchorRelativeToParent = 0, /*!< item have it's own size but it's inside the parent item with relative position (using x/y/width/height) */
    AnchorFollowParentSize,     /*!< item follows the parent size and ignores x/y/width/height, however, size can be
                                 * modified with margin functions, for example setMargin() */
    AnchorRelativeToParentXyFollowParentSize, /*!< item have it's own xy, but follow parent item with size (using parent's width/height), this is used with TgPage */
    AnchorRelativeToParentXFollowParentSize,  /*!< item have it's own y, but follow parent item with size (using parent's width), this is used with TgMenuTop */
};

/*!
 * \brief TgTextfieldHorizontalAlign
 * text horizontal align
 */
enum TgTextfieldHorizontalAlign {
    AlignLeft = 0,  /*!< text to set horizontal left */
    AlignCenterH,   /*!< text to set horizontal center */
    AlignRight      /*!< text to set horizontal right */
};

/*!
 * \brief TgTextfieldVerticalAlign
 * text vertical align
 */
enum TgTextfieldVerticalAlign {
    AlignTop = 0,  /*!< text to set vertical top */
    AlignCenterV,  /*!< text to set vertical center */
    AlignBottom    /*!< text to set vertical bottom */
};

/*!
 * \brief TgTextFieldText
 * this is used with function setText(const std::vector<TgTextFieldText> &listText)
 * to set multicolor text
 */
struct TgTextFieldText
{
    std::string m_text;     /*!< utf8 text */
    uint8_t m_textColorR = 0;   /*!< m_text's red color */
    uint8_t m_textColorG = 0;   /*!< m_text's green color */
    uint8_t m_textColorB = 0;   /*!< m_text's blue color */
};

/*!
 * \brief TgTextFieldWordWrap
 *
 */
enum TgTextFieldWordWrap
{
    WordWrapBounded = 0, /*!< default, words will be wrapped to minimum of area */
    WordWrapOff,         /*!< only the line break '\n' will wrap the line */
    WordWrapOn,          /*!< lines are wrapped to ' ' (empty space) or '\n' if it's possible, if there is no space or '\n' at the line, then it's same wrapping as WordWrapBounded */
};

/*!
 * \brief TgItem2d
 * single item (widget) that all other items use to make their positioning
 */
class TgItem2d
{
public:
    explicit TgItem2d(TgItem2d *parent);
    explicit TgItem2d(TgItem2d *parent, float x, float y, float width, float height);
    virtual ~TgItem2d();

    virtual TgMenuItem *addMenu(const char *text, const TgShortCut *shortCut);

    bool getVisible();
    virtual void setVisible(bool visible);

    float getX();
    float getY();
    float getXonWindow();
    float getYonWindow();
    float getWidth();
    float getHeight();
    float getXminOnVisible();
    float getXmaxOnVisible(const TgWindowInfo *windowInfo);
    float getYminOnVisible();
    float getYmaxOnVisible(const TgWindowInfo *windowInfo);

    void setX(float x);
    void setY(float y);
    void setWidth(float width);
    void setHeight(float height);

    bool getPositionChanged();
    void setPositionChanged(bool positionChanged);

    TgItem2dAnchor getAnchorState() const;
    void setAnchorState(TgItem2dAnchor state);

    void setMargin(float left, float top, float right, float bottom);
    void setMarginLeft(float left);
    void setMarginTop(float top);
    void setMarginRight(float right);
    void setMarginBottom(float bottom);
    float getMarginLeft();
    float getMarginTop();
    float getMarginRight();
    float getMarginBottom();

    void connectOnResizeChanged(std::function<void(float x, float y, float width, float height)> resizeChanged);
    void disconnectOnResizeChanged();

    void connectOnVisibleChanged(std::function<void(bool visible)> visibleChanged);
    void disconnectOnVisibleChanged();

    void connectOnSelectedChanged(std::function<void(bool selected)> selectedChanged);
    void disconnectOnSelectedChanged();

    void connectOnEnabledChanged(std::function<void(bool)> enabledChanged);
    void disconnectOnEnabledChanged();

    bool getSelected();
    bool getCanSelect();
    bool getEnabled();
    void setSelected(bool selected);
    void setCanSelect(bool canSelect);
    void setEnabled(bool enabled);

    void setToTop();

    bool getUseRoundedPositionValues();
    void setUseRoundedPositionValues(bool useRoundedPositionValues);

    void setNextTabItem(TgItem2d *nextTabItem);
    void setPrevTabItem(TgItem2d *prevTabItem);

    virtual size_t getMenuCount();
    virtual TgMenuItem *getMenu(size_t index);
    virtual bool removeMenu(size_t i);

    void setTooltip(const char *text, TgTextfieldHorizontalAlign align = TgTextfieldHorizontalAlign::AlignCenterH);
    void deleteLater();
protected:
    virtual bool render(const TgWindowInfo *windowInfo);
    virtual void checkPositionValues();

    void renderChildren(const TgWindowInfo *windowInfo);
    virtual void checkPositionValuesChildren(const TgWindowInfo *windowInfo);

    virtual void onEnabledChanged(bool enabled);
    virtual void onVisibleChanged(bool visible);

    virtual TgEventResult handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo);
    TgEventResult handleEventsChildren(TgEventData *eventData, const TgWindowInfo *windowInfo);

    void checkOnResizeChanged();
    void checkOnResizeChangedOnChildren();
    virtual void handlePrivateMessage(const TgItem2dPrivateMessage *message);

private:
    TgItem2dPrivate *m_private;

    explicit TgItem2d(TgItem2d *parent, float x, float y, float width, float height, bool topMenu);

    void addChild(TgItem2d *child, bool topMenu);
    void sendMessageToChildren(const TgItem2dPrivateMessage *message);
    void sendMessageToChildrenFromBegin(const TgItem2dPrivateMessage *message);
    virtual void addNewEvent(TgEventData *event);

    void setAddMinMaxHeightOnVisible(float addMinHeightOnVisible, float addMaxHeightOnVisible);

    friend class TgItem2dPrivate;
    friend class TgItem2dVisible;
    friend class TgItem2dEnabled;
    friend class TgMainWindowTooltip;
    friend class TgMainWindowPrivate;
    friend class TgTextfieldPrivate;
    friend class TgButtonPrivate;
    friend class TgTexteditPrivate;
    friend class TgMouseCapturePrivate;
    friend class TgMouseCapture;
    friend class TgItem2dMenu;
    friend class TgMenuTopPrivate;
    friend class TgMainWindowMenu;
    friend class TgMenuItemPrivate;
    friend class TgMenuItem;
    friend class TgMenuTop;
    friend class TgGridView;
    friend class TgGridViewCell;
    friend class TgGridViewPrivate;
    friend class TgComboBoxPrivate;
    friend class TgImage;
    friend class TgImagePart;
    friend class TgRectangle;
    friend class TgTextfield;
    friend class TgSlider;
    friend class TgSliderPrivate;
    friend class TgPagesPrivate;

};

#endif // TG_ITEM_2D_H
