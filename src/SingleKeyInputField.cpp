#include "rGUI.h"

namespace rGUI //SingleKeyInputField
{
    SingleKeyInputField::SingleKeyInputField(float x, float y, float width, float height,
                                             std::string font_file, Theme *thm)
    : InputField(x,y,width, height, font_file,thm,1)
    {
        text = "";
        al_key = 999;
    }

    SingleKeyInputField::SingleKeyInputField(float x, float y, float width, float height, int init_key, std::string font_file,
                        Theme *thm)
    : InputField(x,y,width, height, font_file,thm,1), al_key(init_key)
    {
        text = al_keycode_to_name(init_key);
    }

    SingleKeyInputField::~SingleKeyInputField()
    {

    }

    int SingleKeyInputField::Input()
    {
        return Specific_Input(event);
    }

    int SingleKeyInputField::Specific_Input(ALLEGRO_EVENT &ev)
    {
        wd_md->Specific_Input(ev);

        if( (wd_md->md_clicked == true && wd_md->md_mouse_on_it == false &&
             ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == wd_md->md_mouse_button) ||
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
        texty_shift = text_width - wd_width + bar_width + wd_theme.thickness + wd_theme.added_thickness + 6;
        texty_shift = (texty_shift > 0 ? texty_shift : 0);

        wd_PrintBegin();


        al_draw_filled_rounded_rectangle( wd_theme.added_thickness/2+1,
                                          wd_theme.added_thickness/2,
                                          wd_width + wd_theme.added_thickness/2,
                                          wd_height + wd_theme.added_thickness/2-1,
                                          wd_theme.roundx, wd_theme.roundy, wd_theme.c_background);

        al_set_clipping_rectangle(wd_theme.added_thickness/2,wd_theme.added_thickness/2,
                                  wd_width + wd_theme.added_thickness/2, wd_height + wd_theme.added_thickness/2);

        al_draw_text(font, wd_theme.c_text, wd_theme.added_thickness/2 + text_x - texty_shift, wd_theme.added_thickness/2 +text_y,0, text.c_str());

        al_set_clipping_rectangle(0,0,
                                  wd_width + wd_theme.added_thickness, wd_height + wd_theme.added_thickness);

        al_draw_rounded_rectangle(wd_theme.added_thickness/2+ wd_theme.thickness/2+1,
                                  wd_theme.added_thickness/2+ wd_theme.thickness/2,
                                  wd_width + wd_theme.added_thickness/2 - wd_theme.thickness/2,
                                  wd_height + wd_theme.added_thickness/2 - wd_theme.thickness/2-1,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_outline, wd_theme.thickness);

        if(wd_md->md_clicked == true)
        {
            al_draw_rounded_rectangle(wd_theme.added_thickness/2+ wd_theme.thickness/2+1,
                                  wd_theme.added_thickness/2+ wd_theme.thickness/2,
                                  wd_width + wd_theme.added_thickness/2 - wd_theme.thickness/2,
                                  wd_height + wd_theme.added_thickness/2 - wd_theme.thickness/2-1,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_outline, wd_theme.added_thickness);
        }

        wd_PrintEnd();
    }

}
