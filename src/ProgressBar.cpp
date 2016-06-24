#include "rGUI.h"

namespace rGUI //ProgressBar
{
    ProgressBar::ProgressBar(float x, float y, float width, float height, int initial_val_from_0_to_100, Theme *thm, bool bitmaponly)
        :Widget(x,y,width, height, thm, bitmaponly), value(initial_val_from_0_to_100)
    {

    }

    ProgressBar::~ProgressBar()
    {

    }

    int ProgressBar::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
        return wd_md->Input(ev, scalex, scaley);
    }

    void ProgressBar::Print()
    {
        wd_PrintBegin();

        if(value < 0)
        {
            value = 0;
        }
        else if(value >100)
        {
            value = 100;
        }

        al_draw_filled_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_background);
        al_draw_rounded_rectangle(wd_x1 + wd_theme.thickness/2.0, wd_y1 + wd_theme.thickness/2.0,
                                  wd_x2 - wd_theme.thickness/2.0, wd_y2 - wd_theme.thickness/2.0,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_outline, wd_theme.thickness);
        if(value > 0)
        {
             al_draw_filled_rounded_rectangle(wd_x1 + wd_theme.thickness +1, wd_y1 + wd_theme.thickness +1,
                            (wd_x1 + wd_theme.thickness +1.0f)+ ((wd_x2 - wd_theme.thickness -1.0f)-(wd_x1 + wd_theme.thickness +1))*((float)value/100.0f),
                            wd_y2 - wd_theme.thickness -1, wd_theme.roundx, wd_theme.roundy, wd_theme.c_text);
        }

        wd_PrintEnd();
    }

    void ProgressBar::Set_value(int val)
    {
        value = val;
    }

    int ProgressBar::Get_value()
    {
        return value;
    }
}
