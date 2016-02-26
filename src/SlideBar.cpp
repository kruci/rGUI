#include "rGUI.h"

namespace rGUI //SlideBars
{
    SlideBar::SlideBar(float x, float y, float width, float height, int minval, int maxval,
                       Theme *thm, bool vertical, bool bitmaponly)
    : Widget(x,y, width, height, thm, bitmaponly), vertical(vertical), val_min(minval),
    val_max(maxval), value(minval), values(maxval - minval + 1)
    {
        wd_extented_input = true;
        wd_type = wt_SLIDEBAR;
        sb_mouse_z = mouse_state->z;
        if(vertical == false)
        {
            line_thickness = height/3.0f;
            slide_height = height;
            slide_width = slide_height/9.0f *5.0f;
            line_x1 = x + slide_width/2.0f;
            line_y1 = y + height/2.0f - line_thickness/2.0f;
            line_x2 = x + width - slide_width/2.0f;
            line_y2 = y + height/2.0f + line_thickness/2.0f;
        }
        else
        {
            line_thickness = width/3.0f;
            slide_height = width/9.0f *5.0f;
            slide_width = width;
            line_x1 = x + width/2.0f - line_thickness/2.0f;
            line_y1 = y + slide_height/2.0f;
            line_x2 = x + width/2.0f + line_thickness/2.0f;
            line_y2 = y + height - slide_height/2.0f;
        }
        line_width = abs(line_x1 - line_x2);
        line_height = abs(line_y1 - line_y2);

        sb_calculate_slide_poz();

        if(vertical == false)
        {
            wd_md->Change_coords(wd_x1 /*- slide_width/2.0f */, wd_y1, wd_width /*+ slide_width*/, wd_height);
        }
        else
        {
            wd_md->Change_coords(wd_x1, wd_y1 /*- slide_height/2.0f*/, wd_width, wd_height /*+ slide_height*/);
        }
    }

    SlideBar::~SlideBar()
    {

    }

    void SlideBar::sb_calculate_slide_poz()
    {
        if(vertical == false)
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
            slide_x1 = wd_x1;
            slide_x2 = wd_x2;
            slide_y1 = poz - slide_height/2.0f;
            slide_y2 = poz + slide_height/2.0f;
        }
        sb_recalculate_slide_poz = false;
    }

    void SlideBar::sb_calculate_line_poz()
    {
        if(vertical == false)
        {
            line_thickness = wd_height/3.0f;
            slide_height = wd_height;
            slide_width = slide_height/9.0f *5.0f;
            line_x1 = wd_x1 + slide_width/2.0f;
            line_y1 = wd_y1 + wd_height/2.0f - line_thickness/2.0f;
            line_x2 = wd_x1 + wd_width - slide_width/2.0f;
            line_y2 = wd_y1 + wd_height/2.0f + line_thickness/2.0f;
        }
        else
        {
            line_thickness = wd_width/3.0f;
            slide_height = wd_width/9.0f *5.0f;
            slide_width = wd_width;
            line_x1 = wd_x1 + wd_width/2.0f - line_thickness/2.0f;
            line_y1 = wd_y1 + slide_height/2.0f;
            line_x2 = wd_x1 + wd_width/2.0f + line_thickness/2.0f;
            line_y2 = wd_y1 + wd_height - slide_height/2.0f;
        }
        line_width = abs(line_x1 - line_x2);
        line_height = abs(line_y1 - line_y2);
    }

    int SlideBar::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
        wd_md->Input(ev, scalex, scaley);

        if(wd_md->md_clicking == true || (wd_md->md_clicked == true && (rGUI::mouse_state->buttons & wd_mouse_button)))
        {
            wd_md->md_clicked = true;
            if(vertical == false)
            {
                value = (rGUI::mouse_state->x - wd_md->md_x1)/(wd_width/values);
            }
            else
            {
                value = (rGUI::mouse_state->y - wd_md->md_y1)/(wd_height/values);
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
        //Line draw
        al_draw_filled_rounded_rectangle(line_x1, line_y1, line_x2, line_y2,
                                wd_roundx, wd_roundy, wd_c_background);
        al_draw_rounded_rectangle(line_x1, line_y1, line_x2, line_y2,
                                wd_roundx, wd_roundy, wd_c_outline, wd_thickness);
        //slide box draw
        al_draw_filled_rounded_rectangle(slide_x1, slide_y1, slide_x2, slide_y2,
                                wd_roundx, wd_roundy, wd_c_background);
        al_draw_rounded_rectangle(slide_x1, slide_y1, slide_x2, slide_y2,
                                wd_roundx, wd_roundy, wd_c_outline, wd_thickness);

        if(wd_md->md_mouse_on_it == true)
        {
            al_draw_rounded_rectangle(slide_x1, slide_y1, slide_x2, slide_y2,
                                wd_roundx, wd_roundy, wd_c_outline, wd_thickness + wd_added_thickness);
        }

        if(wd_md->md_clicking == true || wd_md->md_clicked == true)
        {
            al_draw_filled_rounded_rectangle(slide_x1, slide_y1, slide_x2, slide_y2,
                                wd_roundx, wd_roundy, wd_c_clicking);
        }

        wd_PrintEnd();
    }

    void SlideBar::Change_coords(float x1, float y1, float width, float height)
    {
        wd_Change_coords(x1, y1, width, height);
        sb_calculate_line_poz();
        sb_calculate_slide_poz();

        if(vertical == false)
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

        if(vertical == false)
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
}
