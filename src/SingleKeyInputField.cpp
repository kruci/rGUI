#include "rGUI.h"

namespace rGUI //SingleKeyInputField
{
    SingleKeyInputField::SingleKeyInputField(float x, float y, float width, float height,
                                             std::string font_file, Theme *thm, bool bitmap_only)
    : InputField(x,y,width, height, font_file,thm,1, bitmap_only)
    {
        text = "";
        al_key = 999;
    }

    SingleKeyInputField::SingleKeyInputField(float x, float y, float width, float height, int init_key, std::string font_file,
                        Theme *thm, bool bitmap_only)
    : InputField(x,y,width, height, font_file,thm,1, bitmap_only), al_key(init_key)
    {
        text = al_keycode_to_name(init_key);
    }

    SingleKeyInputField::~SingleKeyInputField()
    {

    }

    int SingleKeyInputField::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
        wd_md->Input(ev, scalex, scaley);

        if( (wd_md->md_clicked == true && wd_md->md_mouse_on_it == false &&
             ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == wd_mouse_button) ||
            (al_key_down(keyboard_state, ALLEGRO_KEY_ENTER)) || (al_key_down(keyboard_state, ALLEGRO_KEY_ESCAPE)) )
        {
            wd_md->md_clicked = false;
        }
        else if(wd_md->md_clicked == true && ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if(ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
            {
                al_key = 999;
            }
            else
            {
                al_key = ev.keyboard.keycode;
            }
        }
        if(al_key == 999)
        {
            text = "";
        }
        else
        {
            text = al_keycode_to_name(al_key);
        }
    }

    void SingleKeyInputField::Print()
    {
        text_width = al_get_text_width(font, text.c_str());
        texty_shift = text_width - wd_width + bar_width + wd_thickness + wd_added_thickness + 6;
        texty_shift = (texty_shift > 0 ? texty_shift : 0);

        wd_PrintBegin();


        al_draw_filled_rounded_rectangle(0 + wd_thickness/2.0f, 0 + wd_thickness/2.0f,
                                         wd_width- wd_thickness/2.0f, wd_height- wd_thickness/2.0f,
                                         wd_roundx, wd_roundy, wd_c_background);
        al_draw_rounded_rectangle(0 + wd_thickness/2.0f, 0 + wd_thickness/2.0f,
                                  wd_width- wd_thickness/2.0f, wd_height- wd_thickness/2.0f,
                                  wd_roundx, wd_roundy, wd_c_outline, wd_thickness);
        al_draw_text(font, wd_c_text, text_x - texty_shift, text_y,0, text.c_str());
        if(wd_md->md_clicked == true)
        {
            al_draw_rounded_rectangle(0 + wd_thickness/2.0f + wd_added_thickness/2.0f + 1,
                                      0 + wd_thickness/2.0f + wd_added_thickness/2.0f,
                                      wd_width - wd_thickness/2.0f - wd_added_thickness/2.0f,
                                      wd_height- wd_thickness/2.0f - wd_added_thickness/2.0f -1,
                                      wd_roundx, wd_roundy, wd_c_outline, wd_thickness);
        }

        wd_PrintEnd();
        if(bmp_only == false)
        {
            al_draw_bitmap(wd_bmp , wd_x1, wd_y1, 0);
        }
    }

}
