#include "rGUI.h"

namespace rGUI //CheckBox
{
    CheckBox::CheckBox(float x, float y, float width, float height, Theme *thm, bool is_selected)
    : Widget( x, y, width, height, thm), selected(is_selected)
    {
        wd_type = wt_CHECKBOX;
    }

    CheckBox::~CheckBox()
    {
        #ifdef DEST_PRINT
        std::cout << "CheckBox dtor"<<std::endl;
        #endif // DEST_PRINT
    }

    int CheckBox::Specific_Input(ALLEGRO_EVENT &ev)
    {
        int a = wd_md->Specific_Input(ev);

        if(wd_md->md_just_clicked == true)
        {
            selected = (selected == true ? false : true);
        }

        return a;
    }

    int CheckBox::Input()
    {
        return Specific_Input(event);
    }

    void CheckBox::Print()
    {
        wd_PrintBegin();

        wd_Print_Background();
        wd_Print_Frame();


        if(selected == true)
        {
            al_draw_filled_circle( (wd_theme.added_thickness + wd_width)/2.0f, (wd_theme.added_thickness + wd_height)/2.0f,
                                  (wd_width < wd_height ? (wd_width/8.0f)*3.0f -wd_theme.thickness : (wd_height/8.0f)*3.0f -wd_theme.thickness), wd_theme.c_text);
        }

        if(wd_md->md_mouse_on_it == true)
        {
            wd_Print_AddedThickness();

            if(wd_md->md_clicking == true && !(wd_bf & bf_DISABLE_CLICKING_SHADOW))
            {
                wd_Print_ClickingShadow();

            }
        }

        wd_PrintEnd();
    }

}
