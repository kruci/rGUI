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
        bb_pw = width;
        bb_ph = height;
        Set_flags(bitflags);
    }

    BitmapButton::BitmapButton(float x, float y, float width, float height, ALLEGRO_BITMAP *image, Theme *thm, int bitflags)
    : Widget(x,y,width,height, thm) , bmp(image) , delete_bitmap(false)
    {
        wd_type = wt_BITMAPBUTTON;
        bb_orig_width  = al_get_bitmap_width(bmp);
        bb_orig_height = al_get_bitmap_height(bmp);
        bb_pw = width;
        bb_ph = height;
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

        if((wd_bf & bf_RESIZE_WIDGET) || ( (wd_bf & bf_RESIZE_WIDGET_H) && (wd_bf & bf_RESIZE_WIDGET_W)))
        {
            bb_pw = bb_orig_width;
            bb_ph = bb_orig_height;
        }
        else if(wd_bf & bf_RESIZE_WIDGET_H)
        {
            bb_pw = wd_width;
            bb_ph = bb_orig_height;
        }
        else if(wd_bf & bf_RESIZE_WIDGET_W)
        {
            bb_pw = bb_orig_width;
            bb_ph = wd_width;
        }
        else
        {
            bb_pw = wd_width;
            bb_ph = wd_height;
        }
        Change_coords(wd_x1,wd_y1, bb_pw, bb_ph);
        orig_x2 = orig_x1 +bb_pw;
        orig_y2 = orig_y1 +bb_ph;
    }

    void BitmapButton::Print()
    {
        wd_PrintBegin();

        al_draw_scaled_bitmap(bmp, 0, 0, bb_orig_width, bb_orig_height,
                              wd_theme.added_thickness/2, wd_theme.added_thickness/2, bb_pw, bb_ph, 0);

        if(wd_bf & bf_HAS_FRAME)
        {
            wd_Print_Frame();
        }

        if(wd_md->md_mouse_on_it == true)
        {
            if(wd_bf & bf_HAS_FRAME)
            {
                wd_Print_AddedThickness();
            }
            if(wd_md->md_clicking == true && !(wd_bf & bf_DISABLE_CLICKING_SHADOW))
            {
                al_draw_tinted_scaled_bitmap(bmp, wd_theme.c_clicking, 0, 0, bb_orig_width, bb_orig_height,
                              wd_theme.added_thickness/2, wd_theme.added_thickness/2, bb_pw, bb_ph, 0);
            }
        }
        wd_PrintEnd();
     }

}
