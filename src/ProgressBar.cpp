#include "rGUI.h"

namespace rGUI //ProgressBar
{
    ProgressBar::ProgressBar(float x, float y, float width, float height, int initial_val_from_0_to_100, Theme *thm)
        :Widget(x,y,width, height, thm), value(initial_val_from_0_to_100)
    {
        wd_type = wt_PROGRESSBAR;
    }

    ProgressBar::~ProgressBar()
    {
        #ifdef DEST_PRINT
        std::cout << "ProgressBar dtor"<<std::endl;
        #endif // DEST_PRINT
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

        wd_Print_Background();
        wd_Print_Frame();

        if(value > 0)
        {
             al_draw_filled_rounded_rectangle(wd_theme.added_thickness/2 + wd_theme.thickness +1,
                                              wd_theme.added_thickness/2 + wd_theme.thickness +1,
                            (wd_theme.added_thickness/2 + wd_theme.thickness +1.0f)+ ((wd_theme.added_thickness/2 + wd_width - wd_theme.thickness -1.0f)-(wd_theme.added_thickness/2 + wd_theme.thickness +1))*((float)value/100.0f),
                            wd_theme.added_thickness/2 + wd_height - wd_theme.thickness -1 ,
                            wd_theme.roundx, wd_theme.roundy, wd_theme.c_text);
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
