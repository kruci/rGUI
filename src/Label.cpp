#include "rGUI.h"

namespace rGUI //ClickableText
{
    Label::Label(float x, float y, float width, float height, std::string texts,
          std::string font_file, int allegro_text_flag,Theme *thm, bool bitmap_only)
    :Widget(x,y,width,height, thm, bitmap_only), text(texts), print_flag(allegro_text_flag), font_file(font_file)
    {
        recal_f_w = true;
        text_height =(wd_height / 5.0f) * 3.8f;
        font = al_load_ttf_font(font_file.c_str(), text_height, 0);
        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", font_file.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }
        recalculate_text();
    }

    Label::Label(float x, float y, float width, std::string texts,
          ALLEGRO_FONT *font,int allegro_text_flag,Theme *thm, bool bitmap_only)
    :Widget(x,y,width, al_get_font_line_height(font), thm, bitmap_only), text(texts), print_flag(allegro_text_flag),
    font_file(""), font(font)
    {
        delete_font = false;
        text_height = al_get_font_ascent(font) + al_get_font_descent(font);//font->height;
        recalculate_text();
    }

    Label::Label(float x, float y, float width, float height, std::string texts,
          std::string font_file, float font_height,int allegro_text_flag,Theme *thm, bool bitmap_only)
    :Widget(x,y,width,height, thm, bitmap_only), text(texts), print_flag(allegro_text_flag), font_file(font_file)
    {
        multiline = true;
        text_height = font_height;
        font = al_load_ttf_font(font_file.c_str(), text_height, 0);
        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", font_file.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }
        recalculate_text();
    }

    Label::Label(float x, float y, float width, float height, std::string texts,
          ALLEGRO_FONT *font, int allegro_text_flag,Theme *thm, bool bitmap_only)
    :Widget(x,y,width, height, thm, bitmap_only), text(texts), print_flag(allegro_text_flag),
    font_file(""), font(font)
    {
        multiline = true;
        delete_font = false;
        text_height = al_get_font_ascent(font) + al_get_font_descent(font);
        recalculate_text();
    }


    void Label::recalsulate_text_flag_poz()
    {
        if(print_flag == ALLEGRO_ALIGN_CENTRE)
        {
            text_x = wd_x1 + wd_width/2.0f;
            text_y = wd_y1 + (wd_height - text_height)/2.0f;
            wd_md->Change_coords(text_x - text_width/2.0f, text_y - text_height/2.0f, text_width, text_height);
        }
        else if(print_flag == ALLEGRO_ALIGN_RIGHT)
        {
            text_x = wd_x2;
            text_y = wd_y1 + (wd_height - text_height)/2.0f;
            wd_md->Change_coords(wd_x2 - text_width , text_y , text_width, text_height);
        }
        else // left
        {
            text_x = wd_x1 + 4;
            text_y = wd_y1 + (wd_height - text_height) / 2.0f;
            wd_md->Change_coords_r(text_x , text_y , text_width, text_height);
        }

        if(multiline == true)
        {
            wd_md->Change_coords_r(wd_x1 , wd_y1 , wd_width, wd_height);
        }
    }

    void Label::recalculate_text()
    {
        text_width = al_get_text_width(font, text.c_str());

        if(text_width >= wd_width && recal_f_w == true)
        {
            text_height = text_height * (wd_width/text_width);
            al_destroy_font(font);
            font = al_load_ttf_font(font_file.c_str(),text_height,0);
            text_width = al_get_text_width(font, text.c_str());
        }
        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", font_file.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }

        recalsulate_text_flag_poz();
        if(wd_bitmap_only == true)
        {
            text_x -= wd_x1;
            text_y -= wd_y1;
        }
    }

    Label::~Label()
    {
        if(delete_font == true && font != nullptr)
            al_destroy_font(font);
    }

    int Label::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
        return wd_md->Input(ev, scalex, scaley);
    }

    void Label::Print()
    {
        wd_PrintBegin();
        al_draw_filled_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_roundx, wd_roundy, wd_c_background);
        al_draw_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_roundx, wd_roundy, wd_c_outline, wd_thickness);
        /*   Mouse detector box
        al_draw_rounded_rectangle(wd_md->md_x1, wd_md->md_y1, wd_md->md_x2, wd_md->md_y2,
                                wd_roundx, wd_roundy, wd_c_outline, wd_thickness);*/

        if(multiline == false)
        {
             al_draw_text(font,wd_c_text, text_x, text_y, print_flag, text.c_str());
        }
        else
        {
            al_draw_multiline_text(font, wd_c_text, text_x, wd_y1 + 4, wd_width,
                                   text_height,print_flag, text.c_str());
        }
        wd_PrintEnd();
    }

    void Label::Change_coords(float x1, float y1, float width, float height)
    {
        wd_Change_coords_r(x1,y1,width, height);
        recalculate_text();
    }

    void Label::Change_coords_r(float &x1, float &y1, float &width, float &height)
    {
        wd_Change_coords_r(x1,y1,width, height);
        recalculate_text();
    }

    void Label::Set_Print_flag(int flag)
    {
        print_flag = flag;
        recalsulate_text_flag_poz();
    }

    int Label::Get_Print_flag()
    {
        return print_flag;
    }
}
