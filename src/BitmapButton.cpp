#include "rGUI.h"

namespace rGUI //BitmpaButton
{
    BitmapButton::BitmapButton(float x, float y, float width, float height, std::string image, Theme *thm, bool bitmaponly)
    : Widget(x,y,width,height, thm, bitmaponly) , delete_bitmap(true)
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
    }

    BitmapButton::BitmapButton(float x, float y, float width, float height, ALLEGRO_BITMAP *image, Theme *thm, bool bitmaponly)
    : Widget(x,y,width,height, thm, bitmaponly) , bmp(image) , delete_bitmap(false)
    {
        wd_type = wt_BITMAPBUTTON;
        bb_orig_width  = al_get_bitmap_width(bmp);
        bb_orig_height = al_get_bitmap_height(bmp);
    }

    BitmapButton::BitmapButton(float x, float y, std::string image, Theme *thm, bool bitmaponly)
    : Widget(x,y,1,1, thm, bitmaponly) , delete_bitmap(true)
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
        orig_x2 = orig_x1 + bb_orig_width;
        orig_y2 = orig_y1 + bb_orig_height;
        wd_Change_coords(x, y, bb_orig_width, bb_orig_height);
        wd_md->Change_coords_r(x, y, bb_orig_width, bb_orig_height);
    }

    BitmapButton::BitmapButton(float x, float y, ALLEGRO_BITMAP *image, Theme *thm , bool bitmaponly)
    : Widget(x,y,1,1, thm, bitmaponly) , bmp(image) , delete_bitmap(false)
    {
        wd_type = wt_BITMAPBUTTON;
        bb_orig_width  = al_get_bitmap_width(bmp);
        bb_orig_height = al_get_bitmap_height(bmp);
        orig_x2 = orig_x1 + bb_orig_width;
        orig_y2 = orig_y1 + bb_orig_height;
        wd_Change_coords(x, y, bb_orig_width, bb_orig_height);
        wd_md->Change_coords_r(x, y, bb_orig_width, bb_orig_height);
    }

    BitmapButton::~BitmapButton()
    {
        if(delete_bitmap == true && bmp != nullptr)
        {
            al_destroy_bitmap(bmp);
        }
    }

    int BitmapButton::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
        return wd_md->Input(ev, scalex, scaley);
    }

    void BitmapButton::Print()
    {
        wd_PrintBegin();

        al_draw_scaled_bitmap(bmp, 0, 0, bb_orig_width, bb_orig_height,
                              wd_x1, wd_y1, wd_width, wd_height, 0);

        if(wd_md->md_clicking == true)
        {
            al_draw_filled_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                        wd_theme.roundx, wd_theme.roundy, wd_theme.c_clicking);
        }
        wd_PrintEnd();
     }

}
