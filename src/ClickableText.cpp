#include "rGUI.h"

namespace rGUI //ClickableText
{
    ClickableText::ClickableText(float x, float y, std::string texts, std::string fontfile,
                                 int allegro_text_flag, float font_height,Theme *thm, bool bitmap_only)
    :Widget(x,y,1,font_height, thm, bitmap_only), text(texts), print_flag(allegro_text_flag), fsize(font_height)
    {
        font = al_load_ttf_font(fontfile.c_str(), fsize, 0);
        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", fontfile.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }

        wd_width = al_get_text_width(font, text.c_str());
        wd_md->Change_coords_r(wd_x1, wd_y1, wd_width, font_height);
        wd_c_background = al_map_rgba(0,0,0,0);
        wd_c_outline = al_map_rgba(0,0,0,0);
    }

    ClickableText::ClickableText(float x, float y, std::string texts, ALLEGRO_FONT *fnt,
                                 int allegro_text_flag,Theme *thm, bool bitmap_only)
    :Widget(x,y,1,1, thm, bitmap_only), text(texts), delete_font(false), print_flag(allegro_text_flag), font(fnt)
    {
        wd_width = al_get_text_width(font, text.c_str());
        wd_height = al_get_font_ascent(font);
        wd_md->Change_coords_r(wd_x1, wd_y1, wd_width, wd_height);
        wd_c_background = al_map_rgba(0,0,0,0);
        wd_c_outline = al_map_rgba(0,0,0,0);
    }

    ClickableText::~ClickableText()
    {
        if(delete_font == true && font == nullptr)
            al_destroy_font(font);
    }

    int ClickableText::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
        return wd_md->Input(ev, scalex, scaley);
    }

    void ClickableText::Print()
    {
        if(wd_bitmap_only == true)
        {
            al_set_target_bitmap(wd_bmp);
            al_clear_to_color(al_map_rgba(0,0,0,0));
        }

        al_draw_text(font,wd_c_text, wd_x1, wd_y1, print_flag, text.c_str());

        if(wd_md->md_mouse_on_it == true)
        {
            if(wd_md->md_clicking == true)
            {
                al_draw_text(font,al_map_rgba(0,0,0,150), wd_x1, wd_y1, print_flag, text.c_str());
            }
        }

        if(wd_bitmap_only == true)
        {
            al_set_target_backbuffer(al_get_current_display());
        }
    }

}
