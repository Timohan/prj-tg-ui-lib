#ifndef TG_GLOBAL_DEFINES
#define TG_GLOBAL_DEFINES

#define IMAGES_PATH     "/usr/share/prj-tg-ui-lib/images"

/*!
 * \brief The Vertice struct
 *
 * 2D texture vertice point
 */
struct Vertice
{
    float x, y;
    float s, t;
};

#define TG_MENU_DEFAULT_HEIGHT                  24
#define TG_MENU_DEFAULT_SUB_MENU_ARROW_HEIGHT   12
#define TG_MENU_DEFAULT_SUB_MENU_ARROW_MARGIN   5

#ifndef MENU_OPEN_OPACITY_TIME
#define MENU_OPEN_OPACITY_TIME  200
#endif

#endif // TG_GLOBAL_DEFINES
