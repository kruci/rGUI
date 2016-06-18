#include "rGUI.h"

namespace rGUI //TextBox
{
    TextBox::TextBox(float x, float y, float width, float height, std::string texts,
          std::string font_file, float font_height, Theme *thm, int bitflags)
    :Widget(x,y,width,height, thm, (bitflags & rg_BITMAP_ONLY)), text(texts), font_file(font_file)
    {
        wd_type = wt_TEXTBOX;
        font = al_load_ttf_font(font_file.c_str(), font_height, 0);
        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", font_file.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }
        text_height = al_get_font_ascent(font) + al_get_font_descent(font);

        Set_flags(bitflags);
    }

    TextBox::TextBox(float x, float y, float width, float height, std::string texts,
          ALLEGRO_FONT *font, Theme *thm, int bitflags)
    :Widget(x,y,width,height, thm, (bitflags & rg_BITMAP_ONLY)), text(texts), font_file(""), font(font)
    {
        wd_type = wt_TEXTBOX;
        delete_font = false;
        text_height = al_get_font_ascent(font) + al_get_font_descent(font);

        Set_flags(bitflags);
    }

    TextBox::~TextBox()
    {
        if(delete_font == true && font != nullptr)
            al_destroy_font(font);
    }

    int TextBox::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
        return wd_md->Input(ev, scalex, scaley);
    }

    void TextBox::Print()
    {
        wd_PrintBegin();
        al_draw_filled_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_roundx, wd_roundy, wd_c_background);
        al_draw_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_roundx, wd_roundy, wd_c_outline, wd_thickness);
        /*   Mouse detector box
        al_draw_rounded_rectangle(wd_md->md_x1, wd_md->md_y1, wd_md->md_x2, wd_md->md_y2,
                                wd_roundx, wd_roundy, wd_c_outline, wd_thickness);*/

        if(!(wd_bf & rg_MULTILINE))
        {
             al_draw_text(font,wd_c_text, text_x, text_y, print_flag, text.c_str());
        }
        else
        {
            al_draw_multiline_text(font, wd_c_text, text_x, text_y/*wd_y1 + wd_thickness + 1*/, wd_width,
                                   text_height,print_flag, text.c_str());
        }
        wd_PrintEnd();
    }

    void TextBox::Change_coords(float x1, float y1, float width, float height)
    {
        wd_Change_coords_r(x1,y1,width, height);
        wd_md->Change_coords_r(x1, y1, width, height);
        Set_flags(wd_bf);
    }

    void TextBox::Change_coords_r(float &x1, float &y1, float &width, float &height)
    {
        wd_Change_coords_r(x1,y1,width, height);
        wd_md->Change_coords_r(x1, y1, width, height);
        Set_flags(wd_bf);
    }

    void TextBox::Change_print_coords(float x1, float y1, float width, float height)
    {
        wd_Change_coords_r(x1,y1,width, height);
        Set_flags(wd_bf);
    }

    void TextBox::Change_print_coords_r(float &x1, float &y1, float &width, float &height)
    {
        wd_Change_coords_r(x1,y1,width, height);
        Set_flags(wd_bf);
    }

    void TextBox::Set_flags(int flags)
    {
        wd_bf = flags;
        wd_bitmap_only = (wd_bf & rg_BITMAP_ONLY);

        text_width = al_get_text_width(font, text.c_str());

        if((text_width >= wd_width) && (wd_bf & rg_RESIZE_TEXT) && (delete_font == true) && (!(wd_bf & rg_MULTILINE)))
        {
            text_height = text_height * (wd_width/text_width);
            al_destroy_font(font);
            font = al_load_ttf_font(font_file.c_str(),text_height,0);
            if(font == nullptr)
            {
                al_show_native_message_box(NULL, "Error", "Failed to load font!", font_file.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
            }
            text_width = al_get_text_width(font, text.c_str());
        }
        text_height = al_get_font_ascent(font) + al_get_font_descent(font);
        if(wd_bf & rg_MULTILINE)
        {
            mld.font = font;
            al_do_multiline_text(font, wd_width, text.c_str(), _multilinecb, &mld);
            multiline_height = mld.lines * text_height;
            multiline_longest_text = mld.maxlinesize;
            //std::cout << multiline_longest_text << "  " << mld.longesttext.c_str() << std::endl;
        }

        if((wd_bf & rg_RESIZE_FRAME) && (!(wd_bf & rg_MULTILINE)))
        {
            wd_Change_coords(wd_x1,wd_y1,text_width + 2*wd_thickness + 2, text_height + 2*wd_thickness + 2);
            wd_md->Change_coords(wd_md->md_x1, wd_md->md_y1, text_width + 2*wd_thickness + 2, text_height + 2*wd_thickness + 2);
        }
        else if((!(wd_bf & rg_MULTILINE)) && ((wd_bf & rg_RESIZE_FRAME_H) || (wd_bf & rg_RESIZE_FRAME_W)))
        {
            float _w = wd_width, _h = wd_height;
            if((wd_bf & rg_RESIZE_FRAME_H))
            {
                _h = text_height + 2*wd_thickness + 2;
            }
            if((wd_bf & rg_RESIZE_FRAME_W))
            {
                _w = text_width + 2*wd_thickness + 2;
            }
            wd_Change_coords(wd_x1,wd_y1,_w, _h);
            wd_md->Change_coords(wd_md->md_x1, wd_md->md_y1, _w, _h);
        }
        else if((wd_bf & rg_RESIZE_FRAME) && (wd_bf & rg_MULTILINE))
        {
            wd_Change_coords(    wd_x1,wd_y1 , multiline_longest_text + 2*wd_thickness +2,
                                 mld.lines * text_height + 2*wd_thickness +2);
            wd_md->Change_coords(wd_md->md_x1, wd_md->md_y1, multiline_longest_text + 2*wd_thickness +2,
                                 mld.lines * text_height + 2*wd_thickness+2);
        }
        else if(((wd_bf & rg_RESIZE_FRAME_H) || (wd_bf & rg_RESIZE_FRAME_W)) && (wd_bf & rg_MULTILINE))
        {
            float _w = wd_width, _h = wd_height;
            if((wd_bf & rg_RESIZE_FRAME_H))
            {
                _h = mld.lines * text_height + 2*wd_thickness +2;
            }
            if((wd_bf & rg_RESIZE_FRAME_W))
            {
                _w = multiline_longest_text + 2*wd_thickness +2;
            }

            wd_Change_coords(wd_x1,wd_y1,_w, _h);
            wd_md->Change_coords(wd_md->md_x1, wd_md->md_y1, _w, _h);
        }

        if((wd_bf & rg_HORIZONTAL_CENTER))
        {
            print_flag = ALLEGRO_ALIGN_CENTRE;
            text_x = wd_x1 + wd_width/2.0f;
        }
        else if((wd_bf & rg_RIGHT))
        {
            print_flag = ALLEGRO_ALIGN_RIGHT;
            text_x = wd_x2 - wd_thickness - 1;
        }
        else//((wd_bf & rg_LEFT))
        {
            print_flag = ALLEGRO_ALIGN_LEFT;
            text_x = wd_x1 + wd_thickness + 1;
        }

        if((wd_bf & rg_MULTILINE))
        {
            if((wd_bf & rg_TOP))
            {
                text_y = wd_y1 + wd_thickness + 1;
            }
            else if((wd_bf & rg_BOTOM))
            {
                text_y = wd_y2 - multiline_height - wd_thickness - 1;
            }
            else //((wd_bf & rg_VERTICAL_CENTER) == true)
            {
                text_y = wd_y1 + (wd_height/2) - (multiline_height/2);
            }
        }
        else
        {
            if((wd_bf & rg_TOP))
            {
                text_y = wd_y1 + wd_thickness + 1;
            }
            else if((wd_bf & rg_BOTOM))
            {
                text_y = wd_y2 - text_height - wd_thickness - 1;
            }
            else //((wd_bf & rg_VERTICAL_CENTER) == true)
            {
                text_y = wd_y1 + (wd_height/2) - (text_height/2);
            }
        }

        if((wd_bf & rg_BITMAP_ONLY))
        {
            text_x -= wd_x1;
            text_y -= wd_y1;
        }
    }

    int TextBox::Get_flags()
    {
        return wd_bf;
    }
}
