#include "rGUI.h"

namespace rGUI //BitmpaButton
{
    BitmapButton::BitmapButton(float x, float y, float width, float height, std::string image, Theme *thm, int bitflags)
    : Widget(x,y,width,height, thm) , delete_bitmap(true)
    {
        wd_type = wt_BITMAPBUTTON;
        bmp = al_load_bitmap(image.c_str());
        if(bmp == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load image!", image.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }
        bb_orig_width  = al_get_bitmap_width(bmp);
        bb_orig_height = al_get_bitmap_height(bmp);

        Set_flags(bitflags);
    }

    BitmapButton::BitmapButton(float x, float y, float width, float height, ALLEGRO_BITMAP *image, Theme *thm, int bitflags)
    : Widget(x,y,width,height, thm) , bmp(image) , delete_bitmap(false)
    {
        wd_type = wt_BITMAPBUTTON;
        bb_orig_width  = al_get_bitmap_width(bmp);
        bb_orig_height = al_get_bitmap_height(bmp);
        Set_flags(bitflags);
    }

    BitmapButton::~BitmapButton()
    {
        if(delete_bitmap == true && bmp != nullptr)
        {
            al_destroy_bitmap(bmp);
        }
    }

    void BitmapButton::Set_flags(int flags)
    {
        wd_bf = flags;

        if(wd_bf & bf_RESIZE_WIDGET)
        {
            wd_CreateBitmap(bb_orig_width, bb_orig_height);
        }
    }

    void BitmapButton::Print()
    {
        wd_PrintBegin();
        al_draw_scaled_bitmap(bmp, 0, 0, bb_orig_width, bb_orig_height,
                              wd_theme.added_thickness/2, wd_theme.added_thickness/2, wd_width, wd_height, 0);

        if(wd_bf & bf_HAS_FRAME)
        {
            al_draw_rounded_rectangle(wd_theme.added_thickness/2, wd_theme.added_thickness/2,
                                wd_width+wd_theme.added_thickness/2, wd_height+wd_theme.added_thickness/2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_outline, wd_theme.thickness);
        }

        if(wd_md->md_mouse_on_it == true)
        {
            if(wd_bf & bf_HAS_FRAME)
            {
                al_draw_rounded_rectangle(wd_theme.added_thickness/2 + wd_theme.thickness/2,
                                  wd_theme.added_thickness/2 + wd_theme.thickness/2,
                                  wd_width + wd_theme.added_thickness/2 -  wd_theme.thickness/2,
                                  wd_height + wd_theme.added_thickness/2-  wd_theme.thickness/2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_outline, wd_theme.thickness + wd_theme.added_thickness);
            }
            if(wd_md->md_clicking == true)
            {
                al_draw_tinted_scaled_bitmap(bmp, wd_theme.c_clicking, 0, 0, bb_orig_width, bb_orig_height,
                              wd_theme.added_thickness/2, wd_theme.added_thickness/2, wd_width, wd_height, 0);
            }
        }

        wd_PrintEnd();
     }

}
