#include "rGUI.h"

namespace rGUI //ScrollBar
{
    ScrollBar::ScrollBar(float x, float y, float width, float height, float real_size, Theme *thm, bool vertical, bool bitmap_only)
    :Widget(x,y, width, height,thm, bitmap_only), vertical(vertical), r_size(real_size)
    {
        wd_type = wt_SCROLLBAR;
        scb_recalculate_rb_ratio();
        scb_recalculate_scroller_poz();
    }

    ScrollBar::~ScrollBar()
    {

    }

    void ScrollBar::scb_recalculate_rb_ratio()
    {
        if(vertical == false)
        {
            rb_ratio = r_size/wd_width;
        }
        else
        {
            rb_ratio = r_size/wd_height;
        }
    }

    void ScrollBar::scb_recalculate_scroller_poz()
    {
        if(vertical == false)
        {
            sroller_x1 = change/rb_ratio + wd_x1+1 + ceil(wd_thickness/2.0f);
            sroller_y1 = wd_y1+1 + ceil(wd_thickness/2.0f);
            sroller_x2 = change/rb_ratio + wd_x1 + (wd_width/r_size)*wd_width -1 - ceil(wd_thickness/2.0f);
            sroller_y2 = wd_y2-1 - ceil(wd_thickness/2.0f);
        }
        else
        {
            sroller_x1 = wd_x1+1 + ceil(wd_thickness/2.0f);
            sroller_y1 = change/rb_ratio + wd_y1 +1 + ceil(wd_thickness/2.0f);
            sroller_x2 = wd_x2-1 - ceil(wd_thickness/2.0f);
            sroller_y2 = change/rb_ratio + wd_y1 + (wd_height/r_size)*wd_height -1 - ceil(wd_thickness/2.0f);
        }
        if(sroller_x1 < wd_x1)
        {
            sroller_x1 = +1 + ceil(wd_thickness/2.0f);
        }
        if(sroller_x2 > wd_x2)
        {
            sroller_x2 = wd_x2-1 - ceil(wd_thickness/2.0f);
        }
        if(sroller_y1 < wd_y1)
        {
            sroller_y1 = wd_y1+1 + ceil(wd_thickness/2.0f);
        }
        if(sroller_y2 > wd_y2)
        {
            sroller_y2 = wd_y2-1 - ceil(wd_thickness/2.0f);
        }

    }

    int ScrollBar::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
        if(wd_md->Input(ev, scalex, scaley) == 9)
            return 9;

        if(wd_md->md_clicking == true || (wd_md->md_clicked == true && (rGUI::mouse_state->buttons & wd_mouse_button)))
        {
            if(wd_md->md_clicked == false)
            {
                if(scalex*rGUI::mouse_state->x <= wd_md->md_x1 + sroller_x2 - wd_x1
                   && scalex*rGUI::mouse_state->x >= wd_md->md_x1 + sroller_x1 - wd_x1  &&
                   scaley*rGUI::mouse_state->y <= wd_md->md_y2 + sroller_y2 - wd_y2
                   && scaley*rGUI::mouse_state->y >= wd_md->md_y1 + sroller_y1 - wd_y1)
                {
                    cx = (wd_md->md_x1 + sroller_x2  - wd_x1 - scalex*(float)rGUI::mouse_state->x)- 1 - ceil(wd_thickness/2.0f);// - rb_ratio;
                    cy = (wd_md->md_y1 + sroller_y2  - wd_y1 - scaley*(float)rGUI::mouse_state->y)- 1 - ceil(wd_thickness/2.0f);// - rb_ratio;
                }
                else
                {
                    cx = 0;
                    cy = 0;
                }
            }

            wd_md->md_clicked = true;

            if(vertical == false)
            {
                //change = (scalex*rGUI::mouse_state->x - abs(wd_md->md_x1 + sroller_x2 - sroller_x1))*rb_ratio;
                //change = (scalex*(float)rGUI::mouse_state->x - abs(wd_md->md_x1 + sroller_x2 - sroller_x1) + cx)*rb_ratio;// + cx*rb_ratio;
                change = (scalex*(float)rGUI::mouse_state->x - abs(wd_md->md_x1 + sroller_x2 - sroller_x1 - cx))*rb_ratio;
            }
            else
            {
                //change = (scaley*rGUI::mouse_state->y - abs(wd_md->md_y1 + sroller_y2 - sroller_y1))*rb_ratio;
                //change = (scaley*(float)rGUI::mouse_state->y - abs(wd_md->md_y1 + sroller_y2 - sroller_y1) +cy)*rb_ratio;// + cy*rb_ratio;
                change = (scaley*(float)rGUI::mouse_state->y - abs(wd_md->md_y1 + sroller_y2 - sroller_y1 - cy))*rb_ratio;
            }
            std::cout << cx << " " << cy << std::endl;
            std::cout << cx << " " << cy << std::endl;
            std::cout << change << " " << std::endl;

            if(change > r_size-(vertical == false ? wd_width : wd_height))
            {
                change = r_size-(vertical == false ? wd_width : wd_height);
            }
            if(change < 0)
            {
                change = 0;
            }
            changed = true;
            scb_dorecalculate_scroller_poz = true;
        }
        else if(wd_md->md_mouse_on_it == true)
        {
            wd_md->md_clicked = false;

            if(scb_mouse_z - mouse_state->z > 0)
            {
                change += scroll_step;
                changed = true;
            }
            else if(scb_mouse_z - mouse_state->z < 0)
            {
                change -= scroll_step;
                changed = true;
            }

            if(change > r_size-(vertical == false ? wd_width : wd_height))
            {
                change = r_size-(vertical == false ? wd_width : wd_height);
            }
            if(change < 0)
            {
                change = 0;
            }
            scb_dorecalculate_scroller_poz = true;
        }
        else
        {
            wd_md->md_clicked = false;
        }
        scb_mouse_z = mouse_state->z;
    }

    void ScrollBar::Scrolling_input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
        if(wd_md->md_active == false)
            return;

        if(scb_mouse_z - mouse_state->z > 0)
        {
            change += scroll_step;
            changed = true;
            scb_dorecalculate_scroller_poz = true;
        }
        else if(scb_mouse_z - mouse_state->z < 0)
        {
            change -= scroll_step;
            changed = true;
            scb_dorecalculate_scroller_poz = true;
        }

        if(change > r_size-(vertical == false ? wd_width : wd_height))
        {
            change = r_size-(vertical == false ? wd_width : wd_height);
        }
        if(change < 0)
        {
            change = 0;
        }

    }

    void ScrollBar::Print()
    {
        wd_PrintBegin();
        if(scb_dorecalculate_scroller_poz == true)
        {
            scb_recalculate_scroller_poz();
            scb_dorecalculate_scroller_poz = false;
        }

        /*al_draw_filled_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_roundx, wd_roundy, wd_c_background);*/

        al_draw_filled_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_roundx, wd_roundy, al_map_rgb_f(wd_c_background.r*c_background_mult,
                                                                   wd_c_background.g*c_background_mult,
                                                                   wd_c_background.b*c_background_mult ));
        al_draw_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_roundx, wd_roundy, wd_c_outline, wd_thickness);

        al_draw_filled_rounded_rectangle(sroller_x1, sroller_y1, sroller_x2, sroller_y2,
                                wd_roundx, wd_roundy, wd_c_background);
        al_draw_rounded_rectangle(sroller_x1, sroller_y1, sroller_x2, sroller_y2,
                                wd_roundx, wd_roundy, wd_c_outline, 1);

        if(wd_md->md_clicking == true || wd_md->md_clicked == true)
        {
            al_draw_filled_rounded_rectangle(sroller_x1, sroller_y1, sroller_x2, sroller_y2,
                                wd_roundx, wd_roundy, wd_c_clicking);
        }
        else if(wd_md->md_mouse_on_it == true)
        {
            al_draw_rounded_rectangle(sroller_x1+wd_added_thickness/2.0, sroller_y1+wd_added_thickness/2.0,
                                      sroller_x2-wd_added_thickness/2.0, sroller_y2-wd_added_thickness/2.0,
                                wd_roundx, wd_roundy, wd_c_outline, wd_thickness + wd_added_thickness);
        }

        wd_PrintEnd();
    }

    void ScrollBar::Change_coords(float x1, float y1, float width, float height)
    {
        wd_Change_coords_r(x1,y1,width, height);
        wd_md->Change_coords_r(x1,y1,width,height);
        scb_recalculate_rb_ratio();
        scb_recalculate_scroller_poz();
    }

    void ScrollBar::Change_coords_r(float &x1, float &y1, float &width, float &height)
    {
        wd_Change_coords_r(x1,y1,width, height);
        wd_md->Change_coords_r(x1,y1,width,height);
        scb_recalculate_rb_ratio();
        scb_recalculate_scroller_poz();
    }

    void ScrollBar::Change_print_coords(float x1, float y1, float width, float height)
    {
        wd_Change_coords_r(x1,y1,width, height);
        scb_recalculate_rb_ratio();
        scb_recalculate_scroller_poz();
    }

    void ScrollBar::Change_print_coords_r(float &x1, float &y1, float &width, float &height)
    {
        wd_Change_coords_r(x1,y1,width, height);
        scb_recalculate_rb_ratio();
        scb_recalculate_scroller_poz();
    }

    void ScrollBar::Change_real_size(float s)
    {
        r_size = s;
        if(change > r_size-(vertical == false ? wd_width : wd_height))
        {
            change = r_size-(vertical == false ? wd_width : wd_height);
        }
        if(change < 0)
        {
            change = 0;
        }
        scb_recalculate_rb_ratio();
        scb_recalculate_scroller_poz();
    }

    void ScrollBar::Change_real_size_r(float &s)
    {
        r_size = s;
        if(change > r_size-(vertical == false ? wd_width : wd_height))
        {
            change = r_size-(vertical == false ? wd_width : wd_height);
        }
        if(change < 0)
        {
            change = 0;
        }
        scb_recalculate_rb_ratio();
        scb_recalculate_scroller_poz();
    }

    void ScrollBar::Set_change(float chan)
    {
        change = chan;
        if(change < 0)
        {
            change = 0;
        }
        else if(change > r_size-(vertical == false ? wd_width : wd_height))
        {
            change = r_size-(vertical == false ? wd_width : wd_height);
        }
        scb_recalculate_scroller_poz();
    }
}
