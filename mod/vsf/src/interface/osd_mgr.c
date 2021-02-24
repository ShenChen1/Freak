#include "vsf/osd_mgr.h"
#include "ft2build.h"
#include "inc/cfg.h"
#include "inc/hal/rgn.h"
#include "inc/hal/sys.h"
#include "inc/hal/venc.h"
#include "inc/hal/vi.h"
#include "inc/hal/vpss.h"
#include "log.h"
#include "proto.h"
#include "ufifo.h"
#include FT_FREETYPE_H
typedef struct {
    proto_vsf_osd_t *info;
    vsf_font_cfg_t *font;
    FT_Library ft_library;
    FT_Face ft_face;
} vsf_osd_mgr_priv_t;

static vsf_osd_mgr_t *s_mgr = NULL;

static int __vsf_osd_destroy(vsf_osd_mgr_t *self)
{
    // never destroy
    return 0;
}

static void __draw_char(vsf_rgn_bitmap_t *src, FT_Bitmap *dst, proto_point_t *point, uint32_t color)
{
    FT_Int i, j, p, q;
    FT_Int x_max = point->x + dst->width;
    FT_Int y_max = point->y + dst->rows;

    /* for simplicity, we assume that `bitmap->pixel_mode' */
    /* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */

    for (i = point->x, p = 0; i < x_max; i++, p++) {
        for (j = point->y, q = 0; j < y_max; j++, q++) {
            if (i < 0 || j < 0 || i >= src->u32Width || j >= src->u32Height) {
                continue;
            }

            if (dst->buffer[q * dst->width + p]) {
                ((uint16_t *)src->pData)[j * src->u32Stride / 2 + i] = argb8888_1555(color);
            }
        }
    }
}

static int __draw_text(vsf_rgn_bitmap_t *bitmap, proto_vsf_osd_text_t *text, int set)
{
    int n, num_chars;
    FT_Error error;
    vsf_osd_mgr_priv_t *priv = s_mgr->priv;

    num_chars = strlen(text->text);
    /* set character size */
    error = FT_Set_Pixel_Sizes(priv->ft_face, 0, text->size);
    assert(!error);
    FT_GlyphSlot slot = priv->ft_face->glyph;

    /* set up matrix */
    FT_Matrix matrix; /* transformation matrix */
    matrix.xx = (FT_Fixed)(1 * 0x10000L);
    matrix.xy = (FT_Fixed)(0 * 0x10000L);
    matrix.yx = (FT_Fixed)(0 * 0x10000L);
    matrix.yy = (FT_Fixed)(1 * 0x10000L);

    /* the pen position in 26.6 cartesian space coordinates; */
    /* start at (x,y) relative to the upper left corner  */
    FT_Vector pen; /* untransformed origin  */
    proto_point_t point;
    point.x = range(text->point.x, 0, 8191) * bitmap->u32Width / 8192;
    point.y = range(text->point.y, 0, 8191) * bitmap->u32Height / 8192;
    pen.x   = point.x * 64;
    pen.y   = (bitmap->u32Height - point.y) * 64;

    for (n = 0; n < num_chars; n++) {
        /* set transformation */
        FT_Set_Transform(priv->ft_face, &matrix, &pen);

        /* load glyph image into the slot (erase previous one) */
        error = FT_Load_Char(priv->ft_face, text->text[n], FT_LOAD_RENDER);
        if (error) {
            /* ignore errors */
            continue;
        }

        /* now, draw to our target surface (convert position) */
        point.x = slot->bitmap_left;
        point.y = bitmap->u32Height - slot->bitmap_top;
        __draw_char(bitmap, &slot->bitmap, &point, set ? text->color : 0);

        /* increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }

    return 0;
}

static int __draw_objs(vsf_rgn_bitmap_t *bitmap, proto_vsf_osd_objs_t *objs)
{
    int n, i;
    uint16_t color = argb8888_1555(0x01FF0000);

    memset(bitmap->pData, 0, bitmap->u32Height * bitmap->u32Stride);
    for (n = 0; n < objs->num; n++) {
        proto_rect_t rect = objs->rects[n];

        rect.x = range(rect.x, 0, 8191) * bitmap->u32Width / 8192;
        rect.y = range(rect.y, 0, 8191) * bitmap->u32Height / 8192;
        rect.w = range(rect.w, 0, 8191 - rect.x) * bitmap->u32Width / 8192;
        rect.h = range(rect.h, 0, 8191 - rect.y) * bitmap->u32Height / 8192;

        tracef("[%d]: %d %d %d %d", n, rect.x, rect.y, rect.w, rect.h);
        if (rect.w * rect.h == 0) {
            continue;
        }

        uint16_t *box = &((uint16_t *)bitmap->pData)[rect.y * bitmap->u32Stride / 2 + rect.x];
        for (i = 0; i < rect.w; i++) {
            box[0 * bitmap->u32Stride / 2 + i]            = color;
            box[1 * bitmap->u32Stride / 2 + i]            = color;
            box[2 * bitmap->u32Stride / 2 + i]            = color;
            box[3 * bitmap->u32Stride / 2 + i]            = color;
            box[(rect.h - 1) * bitmap->u32Stride / 2 + i] = color;
            box[(rect.h - 2) * bitmap->u32Stride / 2 + i] = color;
            box[(rect.h - 3) * bitmap->u32Stride / 2 + i] = color;
            box[(rect.h - 4) * bitmap->u32Stride / 2 + i] = color;
        }

        for (i = 0; i < rect.h; i++) {
            box[i * bitmap->u32Stride / 2 + 0]            = color;
            box[i * bitmap->u32Stride / 2 + 1]            = color;
            box[i * bitmap->u32Stride / 2 + 2]            = color;
            box[i * bitmap->u32Stride / 2 + 3]            = color;
            box[i * bitmap->u32Stride / 2 + (rect.w - 1)] = color;
            box[i * bitmap->u32Stride / 2 + (rect.w - 2)] = color;
            box[i * bitmap->u32Stride / 2 + (rect.w - 3)] = color;
            box[i * bitmap->u32Stride / 2 + (rect.w - 4)] = color;
        }
    }

    return 0;
}

static int __vsf_osd_ctrl_text(void *rgn, void *args)
{
    proto_vsf_osd_cfg_t *cfg = args;
    vsf_osd_mgr_priv_t *priv = s_mgr->priv;

    __draw_text(rgn, &priv->info->cfgs[cfg->id].info.text, 0);
    return __draw_text(rgn, &cfg->info.text, cfg->enable);
}

static int __vsf_osd_ctrl_objs(void *rgn, void *args)
{
    return __draw_objs(rgn, args);
}

static int __vsf_osd_ctrl(vsf_osd_mgr_t *self, proto_vsf_osd_cfg_t *cfg)
{
    int id;
    vsf_osd_mgr_t *mgr       = self;
    vsf_osd_mgr_priv_t *priv = mgr->priv;
    vsf_rgn_cfg_t config     = {};

    config.enable = cfg->enable;
    config.chn    = priv->info->caps[cfg->id].chn;
    config.subchn = priv->info->caps[cfg->id].subchn;

    if (!strncmp(cfg->info.condition, "mask", sizeof("mask"))) {
        id                 = cfg->id;
        config.type        = VSF_RGN_COVER;
        config.layer       = 1;
        config.cover.color = cfg->info.mask.color;
        memcpy(config.cover.points, cfg->info.mask.points, 4 * sizeof(proto_point_t));
    } else if (!strncmp(cfg->info.condition, "text", sizeof("text"))) {
        id                 = VSF_OSD_MAX + config.chn * VSF_STREAM_MAX / VSF_CHN_MAX + config.subchn;
        config.type        = VSF_RGN_BITMAP;
        config.layer       = 0;
        config.bitmap.args = cfg;
        config.bitmap.proc = __vsf_osd_ctrl_text;
    } else if (!strncmp(cfg->info.condition, "objs", sizeof("objs"))) {
        id                 = VSF_OSD_MAX + config.chn * VSF_STREAM_MAX / VSF_CHN_MAX + config.subchn;
        config.type        = VSF_RGN_BITMAP;
        config.layer       = 0;
        config.bitmap.args = cfg;
        config.bitmap.proc = __vsf_osd_ctrl_objs;
    } else {
        errorf("osd type error: %s", cfg->info.condition);
        return 0;
    }

    vsf_rgn_t *rgn = VSF_createRgn(id);
    if (rgn && rgn->ctrl) {
        return rgn->ctrl(rgn, &config);
    }

    return 0;
}

static int __vsf_osd_get(vsf_osd_mgr_t *self, proto_vsf_osd_cfg_t *cfg)
{
    vsf_osd_mgr_t *mgr       = self;
    vsf_osd_mgr_priv_t *priv = mgr->priv;

    *cfg = priv->info->cfgs[cfg->id];
    return 0;
}

static int __vsf_osd_cap(vsf_osd_mgr_t *self, proto_vsf_osd_cap_t *cap)
{
    vsf_osd_mgr_t *mgr       = self;
    vsf_osd_mgr_priv_t *priv = mgr->priv;

    *cap = priv->info->caps[cap->id];
    return 0;
}

static int __vsf_osd_num(vsf_osd_mgr_t *self)
{
    vsf_osd_mgr_t *mgr       = self;
    vsf_osd_mgr_priv_t *priv = mgr->priv;

    return priv->info->num.num;
}

vsf_osd_mgr_t *vsf_createOsdMgr()
{
    vsf_osd_mgr_t *mgr       = s_mgr;
    vsf_osd_mgr_priv_t *priv = NULL;

    if (mgr) {
        return mgr;
    }

    priv = malloc(sizeof(vsf_osd_mgr_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(vsf_osd_mgr_priv_t));
    priv->info = cfg_get_member(osd);
    priv->font = cfg_get_member(font);
    FT_Error error;
    error = FT_Init_FreeType(&priv->ft_library);
    assert(!error);
    error = FT_New_Face(priv->ft_library, priv->font->path, 0, &priv->ft_face);
    assert(!error);

    mgr = malloc(sizeof(vsf_osd_mgr_t));
    assert(mgr);
    memset(mgr, 0, sizeof(vsf_osd_mgr_t));
    mgr->priv    = priv;
    mgr->destroy = __vsf_osd_destroy;
    mgr->set     = __vsf_osd_ctrl;
    mgr->get     = __vsf_osd_get;
    mgr->cap     = __vsf_osd_cap;
    mgr->num     = __vsf_osd_num;
    mgr->tgr     = __vsf_osd_ctrl;

    s_mgr = mgr;
    return mgr;
}