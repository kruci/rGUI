#include "rGUI.h"

namespace rGUI //CheckBox
{
    CheckBox::CheckBox(float x, float y, float width, float height, Theme *thm, bool is_selected)
    : Widget( x, y, width, height, thm, false), selected(is_selected)
    {
        wd_type = wt_CHECKBOX;
    }

    CheckBox::CheckBox(float width, float height, Theme *thm, bool is_selected)
    : Widget( 0, 0, width, height, thm, true), selected(is_selected)
    {
        wd_type = wt_CHECKBOX;
    }

    CheckBox::~CheckBox()
    {

    }

    int CheckBox::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
        int a = wd_md->Input(ev, scalex, scaley);

        if(wd_md->md_just_clicked == true)
        {
            selected = (selected == true ? false : true);
        }

        return a;
    }

    void CheckBox::Print()
    {
        wd_PrintBegin();

        al_draw_filled_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_roundx, wd_roundy, wd_c_background);
        al_draw_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_roundx, wd_roundy, wd_c_outline, wd_thickness);
        if(selected == true)
        {
            al_draw_filled_circle(wd_x1 + wd_width/2.0f, wd_y1 + wd_height/2.0f,
                                  (wd_width < wd_height ? (wd_width/8.0f)*3.0f -wd_thickness : (wd_height/8.0f)*3.0f -wd_thickness), wd_c_text);
        }

        if(wd_md->md_mouse_on_it == true)
        {
            al_draw_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_roundx, wd_roundy, wd_c_outline, wd_thickness + wd_added_thickness);
            if(wd_md->md_clicking == true)
            {
                al_draw_filled_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_roundx, wd_roundy, wd_c_clicking);
            }
        }

        wd_PrintEnd();
    }

}
