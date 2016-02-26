#include "rGUI.h"

namespace rGUI //InputField
{
    InputField::InputField(float x, float y, float width, float height, std::string font_file, Theme *thm, float FPS, bool bitmap_only)
    : Widget(x,y,width, height, thm, false), font_file(font_file), if_FPS(FPS), bmp_only(bitmap_only)
    {
        wd_extented_input = true;
        wd_type = wt_INPUTFIELD;
        font_height = wd_height - 4 - wd_thickness*2.0f;

        font = al_load_ttf_font(font_file.c_str(), font_height, 0);

        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", font_file.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }

        al_text = al_ustr_new("");
        text_x = wd_thickness + wd_added_thickness + 2;
        text_y = (wd_height - font_height)/2.0f;
        texty_shift = 0;

        wd_bitmap_only = true;
        wd_bmp = al_create_bitmap(wd_width, wd_height);
        if_help_ustr = al_ustr_new("");
    }

    InputField::InputField(float x, float y, float width, float height, std::string init_text,
                            std::string font_file, Theme *thm, float FPS, bool bitmap_only)
    : Widget(x,y,width, height, thm, false), font_file(font_file), if_FPS(FPS), bmp_only(bitmap_only), text(init_text)
    {
        wd_extented_input = true;
        wd_type = wt_INPUTFIELD;
        font_height = wd_height - 4 - wd_thickness*2.0f;

        font = al_load_ttf_font(font_file.c_str(), font_height, 0);

        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", font_file.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }

        al_text = al_ustr_new(init_text.c_str());
        bar_char_poz = text.size()-1;
        text_x = wd_thickness + wd_added_thickness + 2;
        text_y = (wd_height - font_height)/2.0f;
        texty_shift = 0;

        wd_bitmap_only = true;
        wd_bmp = al_create_bitmap(wd_width, wd_height);
        if_help_ustr = al_ustr_new("");
    }

    void InputField::recalculate_text()
    {
        /*font_height = wd_height - 4 - wd_thickness*2.0f;

        font = al_load_ttf_font(font_file.c_str(), font_height, 0);

        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", font_file.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }*/

        text_y = (wd_height - font_height)/2.0f;
        texty_shift = 0;
        text_x = wd_thickness + 2 - texty_shift;
    }

    InputField::~InputField()
    {
        if(font != nullptr)
            al_destroy_font(font);
        if(al_text != nullptr)
            al_ustr_free(al_text);
        if(if_help_ustr != nullptr)
            al_ustr_free(if_help_ustr);
    }

    int InputField::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
        wd_md->Input(ev, scalex, scaley);

        if( (wd_md->md_clicked == true && wd_md->md_mouse_on_it == false &&
             ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == wd_mouse_button) ||
            (al_key_down(keyboard_state, ALLEGRO_KEY_ENTER)) || (al_key_down(keyboard_state, ALLEGRO_KEY_ESCAPE)) )
        {
            wd_md->md_clicked = false;
        }
        else if(wd_md->md_clicked == true)
        {
            if(ev.type == ALLEGRO_EVENT_KEY_CHAR && ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && al_ustr_size(al_text) > 0 &&
               bar_char_poz-1 >= 0)
            {
                al_ustr_prev_get(al_text, &bar_char_poz);
                al_ustr_remove_chr(al_text, bar_char_poz);
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_CHAR && ev.keyboard.keycode == ALLEGRO_KEY_LEFT && bar_char_poz > 0)
            {
                al_ustr_prev_get(al_text, &bar_char_poz);
                if_a = 0;
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_CHAR && ev.keyboard.keycode == ALLEGRO_KEY_RIGHT && bar_char_poz < al_ustr_size(al_text))
            {
                al_ustr_get_next(al_text, &bar_char_poz);
                if_a = 0;
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_CHAR)
            {
                int unichar = ev.keyboard.unichar;
                if(al_ustr_size(al_text) < lenght_limit && unichar >= 32)
                {
                    bar_char_poz += al_ustr_insert_chr(al_text, bar_char_poz, unichar);
                    if_a = 0;
                }
            }
        }
        text = al_cstr_dup(al_text);
    }

    void InputField::Print()
    {
        al_ustr_truncate(if_help_ustr,0);
        if_help_ustr = al_ustr_dup_substr(al_text, 0, bar_char_poz);
        text_width = al_get_ustr_width(font, al_text);
        texty_shift = text_width - wd_width + bar_width + wd_thickness + wd_added_thickness + 6;
        texty_shift = (texty_shift > 0 ? texty_shift : 0);
        bar_x = (al_ustr_size(al_text) > 0 ? text_x + al_get_ustr_width(font, if_help_ustr) : text_x);
        texty_shift = (bar_x - texty_shift < 0 ?
                       bar_x - wd_thickness - wd_added_thickness - text_x: texty_shift);

        wd_PrintBegin();

        al_draw_filled_rounded_rectangle(0 + wd_thickness/2.0f, 0 + wd_thickness/2.0f,
                                         wd_width- wd_thickness/2.0f, wd_height- wd_thickness/2.0f,
                                         wd_roundx, wd_roundy, wd_c_background);

        al_draw_ustr(font, wd_c_text, text_x - texty_shift, text_y,0, al_text);
        if(wd_md->md_clicked == true)
        {
            al_draw_rounded_rectangle(0 + wd_thickness/2.0f + wd_added_thickness/2.0f + 1,
            0 + wd_thickness/2.0f + wd_added_thickness/2.0f,
            wd_width - wd_thickness/2.0f - wd_added_thickness/2.0f,
            wd_height- wd_thickness/2.0f - wd_added_thickness/2.0f -1,
            wd_roundx, wd_roundy, wd_c_outline, wd_thickness);

            if(if_a <= if_FPS)
            {
                al_draw_filled_rectangle(bar_x - texty_shift, text_y, bar_x - texty_shift + bar_width,
                text_y + font_height, wd_c_text);
            }
            if(if_a > if_FPS*2.0f)
            {
                if_a = 0;
            }
            if_a++;
        }

        al_draw_rounded_rectangle(0 + wd_thickness/2.0f, 0 + wd_thickness/2.0f,
                                  wd_width- wd_thickness/2.0f, wd_height- wd_thickness/2.0f,
                                  wd_roundx, wd_roundy, wd_c_outline, wd_thickness);

        wd_PrintEnd();
        if(bmp_only == false)
        {
            al_draw_bitmap(wd_bmp , wd_x1, wd_y1, 0);
        }
    }

    void InputField::Set_text(std::string t)
    {
        al_ustr_assign_cstr(al_text, t.c_str());
        bar_char_poz = 0;
        bar_char_poz = al_ustr_offset(al_text, 1000000);
    }

    std::string InputField::Get_text()
    {
        return al_cstr_dup(al_text);
    }

}
