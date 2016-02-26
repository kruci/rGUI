#include "rGUI.h"

namespace rGUI //ClickableText
{
    ClickableText::ClickableText(float x, float y, std::string texts, std::string fontfile,
                                 int allegro_text_flag, float font_height,Theme *thm, bool bitmap_only)
    :Widget(x,y,1,font_height, thm, bitmap_only), text(texts), print_flag(allegro_text_flag), fsize(font_height)
    {
        wd_type = wt_CLICKABLETEXT;
        font = al_load_ttf_font(fontfile.c_str(), fsize, 0);
        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", fontfile.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }

        wd_width = al_get_text_width(font, text.c_str());
        wd_height = al_get_font_ascent(font);
        wd_Change_coords_r(wd_x1, wd_y1, wd_width, wd_height);
        wd_md->Change_coords_r(wd_x1, wd_y1, wd_width, wd_height);
        wd_c_background = al_map_rgba(0,0,0,0);
        wd_c_outline = al_map_rgba(0,0,0,0);

        orig_x2 = orig_x1 + wd_width;
        orig_y2 = orig_y1 + wd_height;
        if(bitmap_only == true)
        {
            wd_x1 = 0;
            wd_y1 = 0;
            wd_x2 = wd_width;
            wd_y2 = wd_height;
        }
    }

    ClickableText::ClickableText(float x, float y, std::string texts, ALLEGRO_FONT *fnt,
                                 int allegro_text_flag,Theme *thm, bool bitmap_only)
    :Widget(x,y,1,1, thm, bitmap_only), text(texts), delete_font(false), print_flag(allegro_text_flag), font(fnt)
    {
        wd_type = wt_CLICKABLETEXT;
        wd_width = al_get_text_width(font, text.c_str());
        wd_height = al_get_font_ascent(font);
        wd_Change_coords_r(wd_x1, wd_y1, wd_width, wd_height);
        wd_md->Change_coords_r(wd_x1, wd_y1, wd_width, wd_height);
        wd_c_background = al_map_rgba(0,0,0,0);
        wd_c_outline = al_map_rgba(0,0,0,0);

        orig_x2 = orig_x1 + wd_width;
        orig_y2 = orig_y1 + wd_height;

        if(bitmap_only == true)
        {
            wd_x1 = 0;
            wd_y1 = 0;
            wd_x2 = wd_width;
            wd_y2 = wd_height;
        }
    }

    ClickableText::~ClickableText()
    {
        if(delete_font == true && font != nullptr)
            al_destroy_font(font);
    }

    int ClickableText::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
        return wd_md->Input(ev, scalex, scaley);
    }

    void ClickableText::Print()
    {
        wd_PrintBegin();
        al_draw_text(font,wd_c_text, wd_x1, wd_y1, print_flag, text.c_str());

        if(wd_md->md_mouse_on_it == true && disable_clicking_shadow == false)
        {
            if(wd_md->md_clicking == true)
            {
                al_draw_text(font,wd_c_clicking, wd_x1, wd_y1, print_flag, text.c_str());
            }
        }

        wd_PrintEnd();
    }

    void ClickableText::Set_Print_flag(int flag)
    {
        print_flag = flag;
    }

    int ClickableText::Get_Print_flag()
    {
        return print_flag;
    }
}
