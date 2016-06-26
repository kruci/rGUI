#include "rGUI.h"

namespace rGUI //TextBox
{
    TextBox::TextBox(float x, float y, float width, float height, std::string texts,
          std::string font_file, float font_height, Theme *thm, int bitflags)
    :Widget(x,y,width,height, thm), text(texts), font_file(font_file)
    {
        wd_type = wt_TEXTBOX;
        font = al_load_ttf_font(font_file.c_str(), font_height, 0);
        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", font_file.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }
        text_height = al_get_font_ascent(font) + al_get_font_descent(font);
        mld = new ml_data;
        Set_flags(bitflags);
    }

    TextBox::TextBox(float x, float y, float width, float height, std::string texts,
          ALLEGRO_FONT *font, Theme *thm, int bitflags)
    :Widget(x,y,width,height, thm), text(texts), font_file(""), font(font)
    {
        wd_type = wt_TEXTBOX;
        delete_font = false;
        text_height = al_get_font_ascent(font) + al_get_font_descent(font);

        mld = new ml_data;
        Set_flags(bitflags);
    }

    TextBox::~TextBox()
    {
        if(delete_font == true && font != nullptr)
            al_destroy_font(font);
        if(mld != nullptr)
            delete mld;
    }

    void TextBox::Print()
    {
        wd_PrintBegin();
        if(wd_bf & bf_HAS_FRAME)
        {
            al_draw_filled_rounded_rectangle( wd_theme.added_thickness/2,
                                          wd_theme.added_thickness/2,
                                          wd_width + wd_theme.added_thickness/2,
                                          wd_height + wd_theme.added_thickness/2,
                                          wd_theme.roundx, wd_theme.roundy, wd_theme.c_background);
            al_draw_rounded_rectangle(wd_theme.added_thickness/2+ wd_theme.thickness/2,
                                  wd_theme.added_thickness/2+ wd_theme.thickness/2,
                                  wd_width + wd_theme.added_thickness/2 - wd_theme.thickness/2,
                                  wd_height + wd_theme.added_thickness/2 - wd_theme.thickness/2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_outline, wd_theme.thickness);
        }

        if(!(wd_bf & bf_RESIZE_CONTENT))
        {
            al_set_clipping_rectangle(wd_theme.added_thickness/2, wd_theme.added_thickness/2,
                                      wd_width + wd_theme.added_thickness/2, wd_height + wd_theme.added_thickness/2);
        }

        if((wd_bf & bf_CUSTOM_TEXT_DRAW) && (textdrawcallback != nullptr))
        {
            al_do_multiline_text(font, wd_width- wd_theme.thickness*2, text.c_str(), textdrawcallback, mld);
        }
        else if(!(wd_bf & bf_MULTILINE))
        {
             al_draw_text(font,wd_theme.c_text, text_x, text_y, print_flag, text.c_str());
        }
        else
        {
            al_draw_multiline_text(font, wd_theme.c_text, text_x, text_y, wd_width- wd_theme.thickness*2,
                                   text_height,print_flag, text.c_str());
        }

        if(!(wd_bf & bf_RESIZE_CONTENT))
        {
            al_set_clipping_rectangle(0, 0, wd_width + wd_theme.added_thickness/2, wd_height+ wd_theme.added_thickness/2);
        }


        if((wd_bf & bf_AS_BUTTON))
        {
            if(wd_md->md_mouse_on_it == true && (wd_bf & bf_HAS_FRAME))
            {
                 al_draw_rounded_rectangle(wd_theme.added_thickness/2 + wd_theme.thickness/2,
                                  wd_theme.added_thickness/2 + wd_theme.thickness/2,
                                  wd_width + wd_theme.added_thickness/2 -  wd_theme.thickness/2,
                                  wd_height + wd_theme.added_thickness/2-  wd_theme.thickness/2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_outline, wd_theme.thickness + wd_theme.added_thickness);
            }

            if(wd_md->md_clicking == true)
            {
                al_draw_filled_rounded_rectangle(wd_theme.added_thickness/2,
                                          wd_theme.added_thickness/2,
                                          wd_width + wd_theme.added_thickness/2,
                                          wd_height + wd_theme.added_thickness/2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_clicking);
            }
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

        text_width = al_get_text_width(font, text.c_str());

        if((text_width >= wd_width) && (wd_bf & bf_RESIZE_CONTENT) && (delete_font == true) && (!(wd_bf & bf_MULTILINE)))
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
        if(wd_bf & bf_MULTILINE)
        {
            mld->font = font;
            al_do_multiline_text(font, wd_width - wd_theme.thickness, text.c_str(), textcalccallback, mld);
            multiline_height = mld->lines * text_height;
            multiline_longest_text = mld->maxlinesize;
        }

        if((wd_bf & bf_RESIZE_WIDGET) && (!(wd_bf & bf_MULTILINE)))
        {
            wd_Change_coords(wd_x1,wd_y1,text_width + 2*wd_theme.thickness + 2, text_height + 2*wd_theme.thickness + 2);
            wd_md->Change_coords(wd_md->md_x1, wd_md->md_y1, text_width + 2*wd_theme.thickness + 2, text_height + 2*wd_theme.thickness + 2);
        }
        else if((!(wd_bf & bf_MULTILINE)) && ((wd_bf & bf_RESIZE_WIDGET_H) || (wd_bf & bf_RESIZE_WIDGET_W)))
        {
            float _w = wd_width, _h = wd_height;
            if((wd_bf & bf_RESIZE_WIDGET_H))
            {
                _h = text_height + 2*wd_theme.thickness + 2;
            }
            if((wd_bf & bf_RESIZE_WIDGET_W))
            {
                _w = text_width + 2*wd_theme.thickness + 2;
            }
            wd_Change_coords(wd_x1,wd_y1,_w, _h);
            wd_md->Change_coords(wd_md->md_x1, wd_md->md_y1, _w, _h);
        }
        else if((wd_bf & bf_RESIZE_WIDGET) && (wd_bf & bf_MULTILINE))
        {
            wd_Change_coords(    wd_x1,wd_y1 , multiline_longest_text + 2*wd_theme.thickness +2,
                                 mld->lines * text_height + 2*wd_theme.thickness +2);
            wd_md->Change_coords(wd_md->md_x1, wd_md->md_y1, multiline_longest_text + 2*wd_theme.thickness +2,
                                 mld->lines * text_height + 2*wd_theme.thickness+2);
        }
        else if(((wd_bf & bf_RESIZE_WIDGET_H) || (wd_bf & bf_RESIZE_WIDGET_W)) && (wd_bf & bf_MULTILINE))
        {
            float _w = wd_width, _h = wd_height;
            if((wd_bf & bf_RESIZE_WIDGET_H))
            {
                _h = mld->lines * text_height + 2*wd_theme.thickness +2;
            }
            if((wd_bf & bf_RESIZE_WIDGET_W))
            {
                _w = multiline_longest_text + 2*wd_theme.thickness +2;
            }

            wd_Change_coords(wd_x1,wd_y1,_w, _h);
            wd_md->Change_coords(wd_md->md_x1, wd_md->md_y1, _w, _h);
        }

        if((wd_bf & bf_HORIZONTAL_CENTER))
        {
            print_flag = ALLEGRO_ALIGN_CENTRE;
            text_x = wd_theme.added_thickness/2 + wd_width/2.0f;
        }
        else if((wd_bf & bf_RIGHT))
        {
            print_flag = ALLEGRO_ALIGN_RIGHT;
            text_x = wd_width + wd_theme.added_thickness/2 - wd_theme.thickness - 1;
        }
        else//((wd_bf & bf_LEFT))
        {
            print_flag = ALLEGRO_ALIGN_LEFT;
            text_x = wd_theme.added_thickness/2 + wd_theme.thickness + 1;
        }

        if((wd_bf & bf_MULTILINE))
        {
            if((wd_bf & bf_TOP))
            {
                text_y = wd_theme.added_thickness/2 + wd_theme.thickness + 1;
            }
            else if((wd_bf & bf_BOTOM))
            {
                text_y = wd_height + wd_theme.added_thickness/2 - multiline_height - wd_theme.thickness - 1;
            }
            else //((wd_bf & bf_VERTICAL_CENTER) == true)
            {
                text_y = wd_theme.added_thickness/2 + (wd_height/2) - (multiline_height/2);
            }
        }
        else
        {
            if((wd_bf & bf_TOP))
            {
                text_y = wd_theme.added_thickness/2 + wd_theme.thickness + 1;
            }
            else if((wd_bf & bf_BOTOM))
            {
                text_y = wd_height + wd_theme.added_thickness/2 - text_height - wd_theme.thickness - 1;
            }
            else //((wd_bf & bf_VERTICAL_CENTER) == true)
            {
                text_y = wd_theme.added_thickness/2 + (wd_height/2) - (text_height/2);
            }
        }
    }

    int TextBox::Get_flags()
    {
        return wd_bf;
    }

    void TextBox::Set_text(std::string t)
    {
        text = t;
        Set_flags(wd_bf);
    }

    std::string TextBox::Get_text()
    {
        return text;
    }
    void TextBox::Update_theme(Theme *thm)
    {
        wd_Update_theme(thm);
        Set_flags(wd_bf);
    }
}
