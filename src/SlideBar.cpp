#include "rGUI.h"

namespace rGUI //SlideBars
{
    SlideBar::SlideBar(float x, float y, float width, float height, int minval, int maxval,
                       Theme *thm, int bitflags)
    : Widget(x,y, width, height, thm), val_min(minval),
    val_max(maxval), value(minval), values(maxval - minval + 1)
    {
        wd_bf = bitflags;
        wd_extented_input = true;
        wd_type = wt_SLIDEBAR;
        sb_mouse_z = mouse_state->z;
        if((wd_bf & bf_HORIZONTAL))
        {
            line_thickness = height/3.0f;
            slide_height = height;
            slide_width = slide_height/9.0f *5.0f;
            line_x1 = wd_theme.added_thickness/2 + slide_width/2.0f;
            line_y1 = wd_theme.added_thickness/2 + height/2.0f - line_thickness/2.0f;
            line_x2 = wd_theme.added_thickness/2 + width - slide_width/2.0f;
            line_y2 = wd_theme.added_thickness/2 + height/2.0f + line_thickness/2.0f;
        }
        else
        {
            line_thickness = width/3.0f;
            slide_height = width/9.0f *5.0f;
            slide_width = width;
            line_x1 = wd_theme.added_thickness/2 + width/2.0f - line_thickness/2.0f;
            line_y1 = wd_theme.added_thickness/2 + slide_height/2.0f;
            line_x2 = wd_theme.added_thickness/2 + width/2.0f + line_thickness/2.0f;
            line_y2 = wd_theme.added_thickness/2 + height - slide_height/2.0f;
        }
        line_width = abs(line_x1 - line_x2);
        line_height = abs(line_y1 - line_y2);

        sb_calculate_slide_poz();

        /*if((wd_bf & bf_HORIZONTAL))
        {
            wd_md->Change_coords(wd_x1, wd_y1, wd_width, wd_height);
        }
        else
        {
            wd_md->Change_coords(wd_x1, wd_y1, wd_width, wd_height);
        }*/
    }

    SlideBar::~SlideBar()
    {

    }

    void SlideBar::sb_calculate_slide_poz()
    {
        if((wd_bf & bf_HORIZONTAL))
        {
            poz = line_x1 + (line_width/(float)values)* (float)(value - val_min);
            slide_x1 = poz - slide_width/2.0f;
            slide_x2 = poz + slide_width/2.0f;
            slide_y1 = line_y1 + line_thickness/2.0f - slide_height/2.0f;
            slide_y2 = line_y1 + line_thickness/2.0f + slide_height/2.0f;
        }
        else
        {
            poz = line_y1 + (line_height/(float)values) * (float)(value - val_min);
            slide_x1 = wd_theme.added_thickness/2;
            slide_x2 = wd_width + wd_theme.added_thickness/2;
            slide_y1 = poz - slide_height/2.0f;
            slide_y2 = poz + slide_height/2.0f;
        }
        sb_recalculate_slide_poz = false;
    }

    void SlideBar::sb_calculate_line_poz()
    {
        if((wd_bf & bf_HORIZONTAL))
        {
            line_thickness = wd_height/3.0f;
            slide_height = wd_height;
            slide_width = slide_height/9.0f *5.0f;
            line_x1 = wd_theme.added_thickness/2 + slide_width/2.0f;
            line_y1 = wd_theme.added_thickness/2 + wd_height/2.0f - line_thickness/2.0f;
            line_x2 = wd_theme.added_thickness/2 + wd_width - slide_width/2.0f;
            line_y2 = wd_theme.added_thickness/2 + wd_height/2.0f + line_thickness/2.0f;
        }
        else
        {
            line_thickness = wd_width/3.0f;
            slide_height = wd_width/9.0f *5.0f;
            slide_width = wd_width;
            line_x1 = wd_theme.added_thickness/2 + wd_width/2.0f - line_thickness/2.0f;
            line_y1 = wd_theme.added_thickness/2 + slide_height/2.0f;
            line_x2 = wd_theme.added_thickness/2 + wd_width/2.0f + line_thickness/2.0f;
            line_y2 = wd_theme.added_thickness/2 + wd_height - slide_height/2.0f;
        }
        line_width = abs(line_x1 - line_x2);
        line_height = abs(line_y1 - line_y2);
    }

    int SlideBar::Input()
    {
        return Specific_Input(event);
    }

    int SlideBar::Specific_Input(ALLEGRO_EVENT &ev)
    {
        wd_md->Specific_Input(ev);

        if(wd_md->md_clicking == true || (wd_md->md_clicked == true && (rGUI::mouse_state->buttons & wd_md->md_mouse_button)))
        {
            if(wd_md->md_clicked == false)
            {
                /*if(trans_mouse_x <= wd_md->md_x1 + slide_x2 - wd_x1
                   && trans_mouse_x >= wd_md->md_x1 + slide_x1 - wd_x1  &&
                   trans_mouse_y <= wd_md->md_y2 + slide_y2 - wd_y2
                   && trans_mouse_y >= wd_md->md_y1 + slide_y1 - wd_y1)*/
                if(trans_mouse_x - wd_md->md_x1 <= slide_x2
                   && trans_mouse_x - wd_md->md_x1 >= slide_x1  &&
                   trans_mouse_y - wd_md->md_y1 <= slide_y2
                   && trans_mouse_y - wd_md->md_y1 >= slide_y1)
                {
                    cx = trans_mouse_x - ((value* wd_md->md_width)/(float)values + wd_md->md_x1);
                    cy = trans_mouse_y - ((value* wd_md->md_height)/(float)values + wd_md->md_y1);
                }
                else
                {
                    cx = 0;
                    cy = 0;
                }
            }
            wd_md->md_clicked = true;

            if((wd_bf & bf_HORIZONTAL))
            {
                value = (trans_mouse_x - cx - wd_md->md_x1)/(wd_md->md_width/(float)values);
            }
            else
            {
                value = (trans_mouse_y - cy - wd_md->md_y1)/(wd_md->md_height/(float)values);
            }

            if(value < val_min)
            {
                value = val_min;
            }
            if(value > val_max)
            {
                value = val_max;
            }
            sb_recalculate_slide_poz = true;
        }
        else if(wd_md->md_mouse_on_it == true)
        {
            wd_md->md_clicked = false;

            if(sb_mouse_z - mouse_state->z > 0)
            {
                value++;
            }
            else if(sb_mouse_z - mouse_state->z < 0)
            {
                value--;
            }

            if(value < val_min)
            {
                value = val_min;
            }
            else if(value > val_max)
            {
                value = val_max;
            }
            sb_recalculate_slide_poz = true;
        }
        else
        {
            wd_md->md_clicked = false;
        }
        sb_mouse_z = mouse_state->z;
    }

    void SlideBar::Print()
    {
        wd_PrintBegin();
        if(sb_recalculate_slide_poz == true)
        {
            sb_calculate_slide_poz();
            sb_recalculate_slide_poz = false;
        }

        if(wd_bf & bf_HAS_FRAME)
        {
            al_draw_rounded_rectangle(wd_theme.added_thickness/2, wd_theme.added_thickness/2,
                                      wd_width+wd_theme.added_thickness/2, wd_height+wd_theme.added_thickness/2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_outline, wd_theme.thickness);
        }

        //Line draw
        al_draw_filled_rounded_rectangle(line_x1, line_y1, line_x2, line_y2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_background);
        al_draw_rounded_rectangle(line_x1, line_y1, line_x2, line_y2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_outline, wd_theme.thickness);
        //slide box draw
        al_draw_filled_rounded_rectangle(slide_x1, slide_y1, slide_x2, slide_y2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_background);
        al_draw_rounded_rectangle(slide_x1, slide_y1, slide_x2, slide_y2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_outline, wd_theme.thickness);

        if(wd_md->md_mouse_on_it == true)
        {
            al_draw_rounded_rectangle(slide_x1, slide_y1, slide_x2, slide_y2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_outline, wd_theme.thickness + wd_theme.added_thickness);
        }

        if(wd_md->md_clicking == true || wd_md->md_clicked == true)
        {
            al_draw_filled_rounded_rectangle(slide_x1, slide_y1, slide_x2, slide_y2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_clicking);
        }

        wd_PrintEnd();
    }

    void SlideBar::Change_coords(float x1, float y1, float width, float height)
    {
        wd_Change_coords(x1, y1, width, height);
        sb_calculate_line_poz();
        sb_calculate_slide_poz();

        if((wd_bf & bf_HORIZONTAL))
        {
            wd_md->Change_coords(wd_x1 /*- slide_width/2.0f */, wd_y1, wd_width /*+ slide_width*/, wd_height);
        }
        else
        {
            wd_md->Change_coords(wd_x1, wd_y1 /*- slide_height/2.0f*/, wd_width, wd_height/* + slide_height*/);
        }
    }

    void SlideBar::Change_coords_r(float &x1, float &y1, float &width, float &height)
    {
        wd_Change_coords(x1, y1, width, height);
        sb_calculate_line_poz();
        sb_calculate_slide_poz();

        if((wd_bf & bf_HORIZONTAL))
        {
            wd_md->Change_coords(wd_x1 /*- slide_width/2.0f*/ , wd_y1, wd_width /*+ slide_width*/, wd_height);
        }
        else
        {
            wd_md->Change_coords(wd_x1, wd_y1 /*- slide_height/2.0f*/, wd_width, wd_height/* + slide_height*/);
        }
    }

    void SlideBar::Change_print_coords(float x1, float y1, float width, float height)
    {
        wd_Change_coords(x1, y1, width, height);
        sb_calculate_line_poz();
        sb_calculate_slide_poz();
    }

    void SlideBar::Change_print_coords_r(float &x1, float &y1, float &width, float &height)
    {
        wd_Change_coords(x1, y1, width, height);
        sb_calculate_line_poz();
        sb_calculate_slide_poz();
    }

    void SlideBar::Set_value(int val)
    {
        value = val;
        if(value < val_min)
        {
            value = val_min;
        }
        else if(value > val_max)
        {
            value = val_max;
        }
        sb_calculate_slide_poz();
    }

    void SlideBar::Set_flags(int flags)
    {
        wd_bf = flags;
        sb_calculate_line_poz();
        sb_calculate_slide_poz();
    }

    void SlideBar::Update_theme(Theme *thm)
    {
        wd_Update_theme(thm);
        sb_calculate_line_poz();
        sb_calculate_slide_poz();
    }

}
