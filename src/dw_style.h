#ifndef __DW_STYLE_H__
#define __DW_STYLE_H__

/* fixed */
#include "dw_tooltip.h"
/*fixed */
#include "mgdconfig.h"
#ifdef _NOUNIX_
#include <common.h>
#include <gdi.h>
#else
#include <minigui/common.h>
#include <minigui/gdi.h>
#endif

#include "dw.h"

#define DW_STYLE_ALIGN_LEFT   1
#define DW_STYLE_ALIGN_RIGHT  2

typedef enum {
   DW_STYLE_BORDER_NONE,
   DW_STYLE_BORDER_HIDDEN,
   DW_STYLE_BORDER_DOTTED,
   DW_STYLE_BORDER_DASHED,
   DW_STYLE_BORDER_SOLID,
   DW_STYLE_BORDER_DOUBLE,
   DW_STYLE_BORDER_GROOVE,
   DW_STYLE_BORDER_RIDGE,
   DW_STYLE_BORDER_INSET,
   DW_STYLE_BORDER_OUTSET
} DwStyleBorderStyle;

typedef enum {
   DW_STYLE_TEXT_ALIGN_LEFT,
   DW_STYLE_TEXT_ALIGN_RIGHT,
   DW_STYLE_TEXT_ALIGN_CENTER,
   DW_STYLE_TEXT_ALIGN_JUSTIFY,
   DW_STYLE_TEXT_ALIGN_STRING
} DwStyleTextAlignType;

typedef enum {
   DW_STYLE_VALIGN_TOP,
   DW_STYLE_VALIGN_BOTTOM,
   DW_STYLE_VALIGN_MIDDLE,
   DW_STYLE_VALIGN_BASELINE,
   DW_STYLE_VALIGN_SUB,
   DW_STYLE_VALIGN_SUPER
} DwStyleVAlignType;

typedef enum {
   /* todo: incomplete */
   DW_STYLE_DISPLAY_BLOCK,
   DW_STYLE_DISPLAY_INLINE,
   DW_STYLE_DISPLAY_LIST_ITEM,
   DW_STYLE_DISPLAY_TABLE,
   DW_STYLE_DISPLAY_TABLE_ROW_GROUP,
   DW_STYLE_DISPLAY_TABLE_HEADER_GROUP,
   DW_STYLE_DISPLAY_TABLE_FOOTER_GROUP,
   DW_STYLE_DISPLAY_TABLE_ROW,
   DW_STYLE_DISPLAY_TABLE_CELL,
   DW_STYLE_DISPLAY_LAST
} DwStyleDisplayType;


typedef enum {
   DW_STYLE_LIST_STYLE_TYPE_DISC,
   DW_STYLE_LIST_STYLE_TYPE_CIRCLE,
   DW_STYLE_LIST_STYLE_TYPE_SQUARE,
   DW_STYLE_LIST_STYLE_TYPE_DECIMAL,
   DW_STYLE_LIST_STYLE_TYPE_DECIMAL_LEADING_ZERO,
   DW_STYLE_LIST_STYLE_TYPE_LOWER_ROMAN,
   DW_STYLE_LIST_STYLE_TYPE_UPPER_ROMAN,
   DW_STYLE_LIST_STYLE_TYPE_LOWER_GREEK,
   DW_STYLE_LIST_STYLE_TYPE_LOWER_ALPHA,
   DW_STYLE_LIST_STYLE_TYPE_LOWER_LATIN,
   DW_STYLE_LIST_STYLE_TYPE_UPPER_ALPHA,
   DW_STYLE_LIST_STYLE_TYPE_UPPER_LATIN,
   DW_STYLE_LIST_STYLE_TYPE_HEBREW,
   DW_STYLE_LIST_STYLE_TYPE_ARMENIAN,
   DW_STYLE_LIST_STYLE_TYPE_GEORGIAN,
   DW_STYLE_LIST_STYLE_TYPE_CJK_IDEOGRAPHIC,
   DW_STYLE_LIST_STYLE_TYPE_HIRAGANA,
   DW_STYLE_LIST_STYLE_TYPE_KATAKANA,
   DW_STYLE_LIST_STYLE_TYPE_HIRAGANA_IROHA,
   DW_STYLE_LIST_STYLE_TYPE_KATAKANA_IROHA,
   DW_STYLE_LIST_STYLE_TYPE_NONE
} DwStyleListStyleType;

typedef enum {
  DW_STYLE_FONT_STYLE_NORMAL,
  DW_STYLE_FONT_STYLE_ITALIC,
  DW_STYLE_FONT_STYLE_OBLIQUE
} DwStyleFontStyle;

typedef enum {
   DW_STYLE_TEXT_DECORATION_UNDERLINE    = 1 << 0,
   DW_STYLE_TEXT_DECORATION_OVERLINE     = 1 << 1,
   DW_STYLE_TEXT_DECORATION_LINE_THROUGH = 1 << 2,
   DW_STYLE_TEXT_DECORATION_BLINK        = 1 << 3
} DwStyleTextDecoration;

typedef enum {
   DW_STYLE_WHITE_SPACE_NORMAL,
   DW_STYLE_WHITE_SPACE_PRE,
   DW_STYLE_WHITE_SPACE_NOWRAP
} DwStyleWhiteSpace;

typedef struct _DwStyle            DwStyle;
typedef struct _DwStyleFont        DwStyleFont;
typedef struct _DwStyleColor       DwStyleColor;
typedef struct _DwStyleBgImage     DwStyleBgImage;
typedef struct _DwStyleShadedColor DwStyleShadedColor;
typedef struct _DwStyleBox         DwStyleBox;
typedef struct _DwStyleRect        DwStyleRect;


/*
 * Lengths are int's. Absolute lengths are represented in the following way:
 *
 *    +---+ - - - - - - - - - -+---+---+
 *    | pixel value (integer)  | 0 | 1 |
 *    +---+ - - - - - - - - - -+---+---+
 *
 * Percentages:
 *
 *    +---+ - - - +---+---+- - - - - -+---+---+---+
 *    | integer part  | decimal fraction  | 1 | 0 |
 *    +---+ - - - +---+---+- - - - - -+---+---+---+
 *     n-1          18  17              2   1   0
 *
 *    | <------ fixed point value ------> |
 *
 * Relative lengths (only used in HTML):
 *
 *    +---+ - - - +---+---+- - - - - -+---+---+---+
 *    | integer part  | decimal fraction  | 1 | 1 |
 *    +---+ - - - +---+---+- - - - - -+---+---+---+
 *     n-1          18  17              2   1   0
 *
 *    | <------ fixed point value ------> |
 *
 * "auto" lenghths are represented as DW_STYLE_LENGTH_AUTO.
 */

typedef int DwStyleLength;

#define DW_STYLE_CREATE_ABS_LENGTH(n)  (((n) << 2) | 1)
#define DW_STYLE_CREATE_PER_LENGTH(v)  ( ( (int)((v) * (1 << 18)) & ~3 ) | 2 )
#define DW_STYLE_CREATE_REL_LENGTH(v)  ( ( (int)((v) * (1 << 18)) & ~3 ) | 3 )

#define DW_STYLE_IS_ABS_LENGTH(l)      (((l) & 3) == 1)
#define DW_STYLE_IS_PER_LENGTH(l)      (((l) & 3) == 2)
#define DW_STYLE_IS_REL_LENGTH(l)      (((l) & 3) == 3)

#define DW_STYLE_ABS_LENGTH_VAL(l)     ((l) >> 2)
#define DW_STYLE_PER_LENGTH_VAL(l)     ( ( (gfloat)((l) & ~3) ) / (1 << 18) )
#define DW_STYLE_REL_LENGTH_VAL(l)     ( ( (gfloat)((l) & ~3) ) / (1 << 18) )

#define DW_STYLE_LENGTH_AUTO           0


struct _DwStyleBox
{
   /* in future also percentages */
   gint32 top, right, bottom, left;
};

struct _DwStyleRect
{
   gint32 x, y;
   guint32 width, height;
};

struct _DwStyle
{
   int ref_count;

   DwStyleFont *font;
   DwStyleTextDecoration text_decoration;
   DwStyleColor *color, *background_color;
   DwStyleBgImage* background_image;

   DwStyleTextAlignType text_align;
   DwStyleVAlignType valign;
   char text_align_char; /* In future, strings will be supported. */

   gint32 border_spacing;
   DwStyleLength width, height;

   DwStyleBox margin, border_width, padding;
   struct { DwStyleShadedColor *top, *right, *bottom, *left; } border_color;
   struct { DwStyleBorderStyle top, right, bottom, left; } border_style;

   DwStyleDisplayType display;
   DwStyleWhiteSpace white_space;
   DwStyleListStyleType list_style_type;

   DwTooltip *x_tooltip; /*fixed*/

   int x_link;
};

struct _DwStyleFont
{
   int ref_count;

   char *name;
   gint32 size;
   int weight;
   char *charset;
   DwStyleFontStyle style;

   LOGFONT* font;
   int space_width;
   int x_height;
};


struct _DwStyleRGB
{
    guchar r, g, b;
};

struct _DwStyleColor
{
   int ref_count;
   int color_val;
   RGB color, inverse_color;
   gal_pixel pixel, inverse_pixel;
};

typedef enum {
    IMAGE_NOREADY,
    IMAGE_LOADING,
    IMAGE_READY
}Sbi_State;

struct _DwStyleBgImage
{
   int ref_count;
   int draw_bgimg_idle;
   void* image; /*a dw_image widget */
   HDC hdc;
   Sbi_State state;  
   BITMAP * bmp;
   RECT rc;
   HWND hwnd;
};


struct _DwStyleShadedColor
{
   int ref_count;
   int color_val;
   RGB color, inverse_color, color_dark, color_light;
   gal_pixel pixel, inverse_pixel, pixel_dark, pixel_light;
};


void a_Dw_style_init    (void);
void a_Dw_style_freeall (void);

void                a_Dw_style_init_values        (DwStyle *style_attrs, HWND wnd);
void                a_Dw_style_reset_values       (DwStyle *style_attrs);

DwStyle*            a_Dw_style_new                (DwStyle *style_attrs, HWND wnd);
DwStyleFont*        a_Dw_style_font_new           (DwStyleFont *font_attrs);
DwStyleFont*        a_Dw_style_font_new_from_list (DwStyleFont *font_attrs,
                                                   gchar *default_family,
                                                   gchar *default_charset);
void                a_Dw_style_change_font         (DwStyle *style, 
                                                    DwStyleFont *new_font);

DwStyleColor*       a_Dw_style_color_new          (int color_val, HWND wnd);
DwStyleShadedColor* a_Dw_style_shaded_color_new   (int color_val, HWND wnd);

gboolean            a_Dw_style_size_diffs         (DwStyle *style1,
                                                   DwStyle *style2);

#define a_Dw_style_ref(style)   ((style)->ref_count++)
#define a_Dw_style_unref(style) if (--((style)->ref_count) == 0) \
                                   Dw_style_remove (style)


/* Don't use this function directly! */
void Dw_style_remove (DwStyle *style);

#define a_Dw_style_box_set_val(box, val) \
   ((box)->top = (box)->right = (box)->bottom = (box)->left = (val))
#define a_Dw_style_box_set_border_color(style, val) \
   ((style)->border_color.top = (style)->border_color.right = \
    (style)->border_color.bottom = (style)->border_color.left = (val))
#define a_Dw_style_box_set_border_style(style, val) \
   ((style)->border_style.top = (style)->border_style.right = \
    (style)->border_style.bottom = (style)->border_style.left = (val))

/* For use of widgets */
#define DW_INFINITY 1000000 /* random */

void p_Dw_style_draw_border     (HDC hdc,
                                 DwRectangle *area,
                                 gint32 vx,
                                 gint32 vy,
                                 gint32 x,
                                 gint32 y,
                                 gint32 width,
                                 gint32 height,
                                 DwStyle *style,
                                 gboolean inverse);
void p_Dw_style_draw_background (HDC hdc,
                                 DwRectangle *area,
                                 gint32 vx,
                                 gint32 vy,
                                 gint32 x,
                                 gint32 y,
                                 gint32 width,
                                 gint32 height,
                                 DwStyle *style,
                                 gboolean inverse);

void p_Dw_style_draw_background_image (HDC hdc,
                                     DwRectangle *area,
                                     gint32 vx,
                                     gint32 vy,
                                     gint32 x,
                                     gint32 y,
                                     gint32 width,
                                     gint32 height,
                                     DwStyle *style,
                                     void* data);

void a_Dw_style_numtostr         (int num,
                                  char *buf,
                                  int buflen,
                                  DwStyleListStyleType list_style_tyle);

#define p_Dw_style_box_offset_x(style)    ((style)->margin.left + \
                                           (style)->border_width.left + \
                                           (style)->padding.left)
#define p_Dw_style_box_rest_width(style)  ((style)->margin.right + \
                                           (style)->border_width.right + \
                                           (style)->padding.right)
#define p_Dw_style_box_diff_width(style)  (p_Dw_style_box_offset_x(style) + \
                                           p_Dw_style_box_rest_width(style))

#define p_Dw_style_box_offset_y(style)    ((style)->margin.top + \
                                           (style)->border_width.top + \
                                           (style)->padding.top)
#define p_Dw_style_box_rest_height(style) ((style)->margin.bottom + \
                                           (style)->border_width.bottom + \
                                           (style)->padding.bottom)
#define p_Dw_style_box_diff_height(style) (p_Dw_style_box_offset_y(style) + \
                                           p_Dw_style_box_rest_height(style))

#endif /* __DW_STYLE_H__ */
