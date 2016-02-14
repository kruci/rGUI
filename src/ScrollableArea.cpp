#include "rGUI.h"

namespace rGUI //ScrollableArea
{
    ScrollableArea::ScrollableArea(float x, float y, float width, float height, float real_width, float real_height,
                   Theme *thm, float scrollbars_thickness)
    : Widget(x,y, float(width+scrollbars_thickness), float(height+scrollbars_thickness), thm, false),
    r_size_w(real_width), r_size_h(real_height), scb_thickness(scrollbars_thickness)
    {
        wd_type = wt_SCROLLABLEAREA;
        scb_vertical = new   ScrollBar(wd_width - scb_thickness,0,scb_thickness,
                                       wd_height- scb_thickness, real_height, thm, true,false);

        scb_horizontal = new ScrollBar(0,wd_height - scb_thickness, wd_width - scb_thickness,
                                       scb_thickness,real_width, thm, false, false);

        scb_vertical->wd_md->Change_coords(wd_x2 - scb_thickness, wd_y1, scb_thickness,
                                       wd_height- scb_thickness);
        scb_horizontal->wd_md->Change_coords(wd_x1, wd_y2 - scb_thickness, wd_width - scb_thickness,
                                       scb_thickness);

        wd_bmp = al_create_bitmap(wd_width, wd_height);
        wd_md->Change_coords(wd_x1, wd_y1, wd_width - scb_thickness, wd_height - scb_thickness);
    }

    ScrollableArea::ScrollableArea(float x1, float y1, float x2, float y2, float real_width, float real_height,
                   Theme *thm, float scrollbars_thickness, float whatever)
    : Widget(x1,y1,x2+scrollbars_thickness, y2+scrollbars_thickness,0, thm, false),
    r_size_w(real_width), r_size_h(real_height), scb_thickness(scrollbars_thickness)
    {
        wd_type = wt_SCROLLABLEAREA;
        scb_vertical = new   ScrollBar(wd_width - scb_thickness,0,scb_thickness,
                                       wd_height- scb_thickness, real_height, thm, true,false);

        scb_horizontal = new ScrollBar(0,wd_height - scb_thickness, wd_width - scb_thickness,
                                       scb_thickness,real_width, thm, false, false);

        scb_vertical->wd_md->Change_coords(wd_x2 - scb_thickness, wd_y1, scb_thickness,
                                       wd_height- scb_thickness);
        scb_horizontal->wd_md->Change_coords(wd_x1, wd_y2 - scb_thickness, wd_width - scb_thickness,
                                       scb_thickness);

        wd_bmp = al_create_bitmap(wd_width, wd_height);
        wd_md->Change_coords(wd_x1, wd_y1, wd_width - scb_thickness, wd_height - scb_thickness);
    }

    ScrollableArea::~ScrollableArea()
    {
        if(scb_horizontal != nullptr)
            delete scb_horizontal;
        if(scb_vertical != nullptr)
            delete scb_vertical;
    }

    void ScrollableArea::sca_recalculate_sc_bars()
    {
        scb_vertical->Change_real_size_r(r_size_h);
        scb_horizontal->Change_real_size_r(r_size_w);
    }

    int ScrollableArea::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
        wd_md->Input(ev, scalex, scaley);

        if(wd_md->md_mouse_on_it == true)
        {
            /*for(int a = 0; a < (int)widgets.size();a++)
            {
                widgets[a]->wd_md->Change_coords(wd_x1 + widgets[a]->orig_x1 - scb_horizontal->change,
                       wd_y1 + widgets[a]->orig_y1 - scb_vertical->change, widgets[a]->wd_width, widgets[a]->wd_height);
                widgets[a]->Input(ev, scalex, scaley);
            }*/
            if(al_key_down(keyboard_state, ALLEGRO_KEY_LSHIFT))
            {
                scb_horizontal->Scrolling_input(ev, scalex, scaley);
            }
            else
            {
                scb_vertical->Scrolling_input(ev, scalex, scaley);
            }
        }
        for(int a = 0; a < (int)widgets.size();a++)
        {
            if( (wd_md->md_mouse_on_it == false && widgets[a]->wd_md->md_clicked == true && widgets[a]->wd_extented_input == true &&
               (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN ||
                ev.type == ALLEGRO_EVENT_KEY_CHAR || ev.type == ALLEGRO_EVENT_KEY_DOWN))
               || wd_md->md_mouse_on_it == true) // SLOW ?
            {
                widgets[a]->wd_md->Change_coords(wd_x1 + widgets[a]->orig_x1 - scb_horizontal->change,
                    wd_y1 + widgets[a]->orig_y1 - scb_vertical->change, widgets[a]->wd_width, widgets[a]->wd_height);
                widgets[a]->Input(ev, scalex, scaley);
            }
        }
        scb_vertical->Input(ev, scalex, scaley);
        scb_horizontal->Input(ev, scalex, scaley);
    }

    void ScrollableArea::Print()
    {
        wd_ref_bmp = al_get_target_bitmap();
        al_set_target_bitmap(wd_bmp);
        al_clear_to_color(al_map_rgba(0,0,0,0));

        al_draw_filled_rounded_rectangle(0, 0, wd_width, wd_height,
                                wd_roundx, wd_roundy, wd_c_background);
        al_draw_rounded_rectangle(1, 0, wd_width, wd_height-1,
                                wd_roundx, wd_roundy, wd_c_outline, wd_thickness);

        for(int a = 0; a < (int)widgets.size();a++)
        {
            if(( (widgets[a]->orig_x1 >= scb_horizontal->change && widgets[a]->orig_x1 <= scb_horizontal->change + wd_width)
                 || (widgets[a]->orig_x2 >= scb_horizontal->change && widgets[a]->orig_x2 <= scb_horizontal->change + wd_width)
                 || (widgets[a]->orig_x1 <= scb_horizontal->change && widgets[a]->orig_x2 >= scb_horizontal->change + wd_width)
                ) && (
                (widgets[a]->orig_y1 >= scb_vertical->change && widgets[a]->orig_y1 <= scb_vertical->change + wd_height)
                 || (widgets[a]->orig_y2 >= scb_vertical->change && widgets[a]->orig_y2 <= scb_vertical->change + wd_height)
                 || (widgets[a]->orig_y1 <= scb_vertical->change && widgets[a]->orig_y2 >= scb_vertical->change + wd_height)))
            {
                widgets[a]->Change_coords(widgets[a]->orig_x1 - scb_horizontal->change,
                    widgets[a]->orig_y1 - scb_vertical->change, widgets[a]->wd_width, widgets[a]->wd_height);
                widgets[a]->Print();
            }
        }

        scb_horizontal->Print();
        scb_vertical->Print();

        al_set_target_bitmap(wd_ref_bmp);
        if(wd_bitmap_only == false)
        {
            al_draw_bitmap(wd_bmp, wd_x1, wd_y1, 0);
        }
    }

    void ScrollableArea::I_added_new_widgets()
    {
        float wid = scb_horizontal->r_size, hei = scb_vertical->r_size;

        for(int a = 0; a < (int)widgets.size();a++)
        {
            if(widgets[a]->wd_x2 + 1 > wid)
            {
                wid = widgets[a]->wd_x2 + 1;
            }
            if(widgets[a]->wd_y2 + 1 > hei)
            {
                hei = widgets[a]->wd_y2 + 1;
            }
        }

        scb_horizontal->Change_real_size_r(wid);
        scb_vertical->Change_real_size_r(hei);
    }

    void ScrollableArea::Set_vertical_sba_status(bool enabled)
    {
        if(enabled == true && scb_vertical_active == false)
        {
            scb_vertical_active = true;
            scb_vertical->change = 0;
            scb_vertical->print_active = true;
            scb_vertical->wd_md->md_active = true;
            wd_height = wd_height + scb_thickness;
            if(wd_bmp != nullptr)
            {
                al_destroy_bitmap(wd_bmp);
            }
            wd_bmp = al_create_bitmap(wd_width, wd_height);
        }
        else if(enabled == false && scb_vertical_active == true)
        {
            scb_vertical_active = false;
            scb_vertical->change = 0;
            scb_vertical->print_active = false;
            scb_vertical->wd_md->md_active = false;
            wd_height = wd_height - scb_thickness;
            if(wd_bmp != nullptr)
            {
                al_destroy_bitmap(wd_bmp);
            }
            wd_bmp = al_create_bitmap(wd_width, wd_height);
        }
    }

    void ScrollableArea::Set_horizontal_sba_status(bool enabled)
    {
        if(enabled == true && scb_horizontal_active == false)
        {
            scb_horizontal_active = true;
            scb_horizontal->change = 0;
            scb_horizontal->print_active = true;
            scb_horizontal->wd_md->md_active = true;
            wd_width = wd_width + scb_thickness;
            if(wd_bmp != nullptr)
            {
                al_destroy_bitmap(wd_bmp);
            }
            wd_bmp = al_create_bitmap(wd_width, wd_height);
        }
        else if(enabled == false && scb_horizontal_active == true)
        {
            scb_horizontal_active = false;
            scb_horizontal->change = 0;
            scb_horizontal->print_active = false;
            scb_horizontal->wd_md->md_active = false;
            wd_width = wd_width - scb_thickness;
            if(wd_bmp != nullptr)
            {
                al_destroy_bitmap(wd_bmp);
            }
            wd_bmp = al_create_bitmap(wd_width, wd_height);
        }
    }

}
