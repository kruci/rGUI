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
        if(wd_md->md_active == false)
            return 9;
        wd_md->Input(ev, scalex, scaley);

        if(wd_md->md_mouse_on_it == true)
        {
            /*for(int a = 0; a < (int)widgets.size();a++)
            {
                widgets[a]->wd_md->Change_coords(wd_x1 + widgets[a]->orig_x1 - scb_horizontal->change,
                       wd_y1 + widgets[a]->orig_y1 - scb_vertical->change, widgets[a]->wd_width, widgets[a]->wd_height);
                widgets[a]->Input(ev, scalex, scaley);
            }*/
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == zoomkey)
            {
                sca_mouse_z = mouse_state->z;
            }

            if(al_key_down(keyboard_state, horizontalscrollkey))
            {
                scb_horizontal->Scrolling_input(ev, scalex, scaley);
            }
            else if(al_key_down(keyboard_state, zoomkey) && (zoomable == true))
            {
                prevzoom = zoom;
                if(sca_mouse_z - mouse_state->z > 0)
                {
                    zoom -= zoomstep;
                }
                else if(sca_mouse_z - mouse_state->z < 0)
                {
                    zoom += zoomstep;
                }

                if(zoom <= 0)
                {
                    zoom = zoomstep;
                }
                sca_mouse_z = mouse_state->z;

                scb_vertical->Change_real_size(r_size_h * zoom);
                scb_vertical->Set_change( (scb_vertical->change/prevzoom) * zoom);
                scb_horizontal->Change_real_size(r_size_w * zoom);
                scb_horizontal->Set_change((scb_horizontal->change/prevzoom) * zoom);
                scb_vertical->changed = true;
                scb_horizontal->changed = true;
            }
            else
            {
                scb_vertical->Scrolling_input(ev, scalex, scaley);
            }
        }
        scb_vertical->Input(ev, scalex, scaley);
        scb_horizontal->Input(ev, scalex, scaley);

        if(scb_vertical->changed == true || scb_horizontal->changed == true)
        {
            changec_i = true;
            changec_p = true;
            scb_vertical->changed = false;
            scb_horizontal->changed = false;
        }


        for(int a = 0; a < (int)widgets.size();a++)
        {
            if( (wd_md->md_mouse_on_it == false && widgets[a]->wd_md->md_clicked == true && widgets[a]->wd_extented_input == true &&
               (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN ||
                ev.type == ALLEGRO_EVENT_KEY_CHAR || ev.type == ALLEGRO_EVENT_KEY_DOWN))
               || wd_md->md_mouse_on_it == true) // SLOW ?
            {
                if(changec_i == true)
                {
                    widgets[a]->wd_md->Change_coords(wd_x1 + widgets[a]->orig_x1*zoom - scb_horizontal->change,
                                                     wd_y1 + widgets[a]->orig_y1*zoom - scb_vertical->change,
                                                     (widgets[a]->orig_x2 - widgets[a]->orig_x1)*zoom,
                                                     (widgets[a]->orig_y2 - widgets[a]->orig_y1)*zoom);
                }
                /*float sxz = scalex*zoom, sxy = scaley*zoom;
                widgets[a]->Input(ev, sxz, sxy);*/
                widgets[a]->Input(ev, scalex, scaley);
            }
        }
        changec_i = false;
    }

    void ScrollableArea::Print()
    {
        if(print_active == false)
            return ;

        wd_ref_bmp = al_get_target_bitmap();
        al_set_target_bitmap(wd_bmp);
        al_clear_to_color(al_map_rgba(0,0,0,0));
        al_draw_filled_rounded_rectangle(0, 0, wd_width, wd_height,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_background);

        if(zoomable == true)
        {
            //al_copy_transform(&ct, al_get_current_transform());
            al_scale_transform(&rest, zoom, zoom);
            al_use_transform(&rest);
        }

        //al_hold_bitmap_drawing(true);
        for(int a = 0; a < (int)widgets.size();a++)
        {
            /*if(( (widgets[a]->orig_x1 >= scb_horizontal->change && widgets[a]->orig_x1 <= scb_horizontal->change + wd_width)
                 || (widgets[a]->orig_x2 >= scb_horizontal->change && widgets[a]->orig_x2 <= scb_horizontal->change + wd_width)
                 || (widgets[a]->orig_x1 <= scb_horizontal->change && widgets[a]->orig_x2 >= scb_horizontal->change + wd_width)
                ) && (
                (widgets[a]->orig_y1 >= scb_vertical->change && widgets[a]->orig_y1 <= scb_vertical->change + wd_height)
                 || (widgets[a]->orig_y2 >= scb_vertical->change && widgets[a]->orig_y2 <= scb_vertical->change + wd_height)
                 || (widgets[a]->orig_y1 <= scb_vertical->change && widgets[a]->orig_y2 >= scb_vertical->change + wd_height)))
            {*/
            if(( (widgets[a]->orig_x1 *zoom >= scb_horizontal->change && widgets[a]->orig_x1 *zoom<= scb_horizontal->change + wd_width)
                 || (widgets[a]->orig_x2 *zoom>= scb_horizontal->change && widgets[a]->orig_x2 *zoom<= scb_horizontal->change + wd_width)
                 || (widgets[a]->orig_x1 *zoom<= scb_horizontal->change && widgets[a]->orig_x2 *zoom>= scb_horizontal->change + wd_width)
                ) && (
                (widgets[a]->orig_y1 *zoom>= scb_vertical->change && widgets[a]->orig_y1 *zoom<= scb_vertical->change + wd_height)
                 || (widgets[a]->orig_y2 *zoom>= scb_vertical->change && widgets[a]->orig_y2 *zoom<= scb_vertical->change + wd_height)
                 || (widgets[a]->orig_y1 *zoom<= scb_vertical->change && widgets[a]->orig_y2 *zoom>= scb_vertical->change + wd_height)))
            {
                if(changec_p == true)
                {
                    widgets[a]->Change_print_coords(widgets[a]->orig_x1 - scb_horizontal->change/zoom,
                    widgets[a]->orig_y1 - scb_vertical->change/zoom, widgets[a]->wd_width, widgets[a]->wd_height);

                    /*widgets[a]->wd_md->Change_coords(wd_x1 + widgets[a]->orig_x1 - scb_horizontal->change,
                    wd_y1 + widgets[a]->orig_y1 - scb_vertical->change, widgets[a]->wd_width, widgets[a]->wd_height);*/

                    widgets[a]->wd_md->Change_coords(wd_x1 + widgets[a]->orig_x1*zoom - scb_horizontal->change,
                                                     wd_y1 + widgets[a]->orig_y1*zoom - scb_vertical->change,
                                                     (widgets[a]->orig_x2 - widgets[a]->orig_x1)*zoom,
                                                     (widgets[a]->orig_y2 - widgets[a]->orig_y1)*zoom);
                }
                widgets[a]->Print();
            }
        }
        changec_p = false;
        //al_hold_bitmap_drawing(false);

        if(zoomable == true)
        {
            al_identity_transform(&rest);
            al_use_transform(&rest);
        }

        scb_horizontal->Print();
        scb_vertical->Print();

        al_draw_rounded_rectangle(wd_theme.thickness/2.0f , wd_theme.thickness/2.0f,
                                  wd_width - wd_theme.thickness/2.0f, wd_height - wd_theme.thickness/2.0f,
                                  wd_theme.roundx, wd_theme.roundy, wd_theme.c_outline, wd_theme.thickness);


        al_set_target_bitmap(wd_ref_bmp);
        if(wd_bitmap_only == false)
        {
            al_draw_bitmap(wd_bmp, wd_x1, wd_y1, 0);
        }
    }

    void ScrollableArea::I_added_new_widgets()
    {
        //float wid = scb_horizontal->r_size, hei = scb_vertical->r_size;
        float wid = 0, hei = 0;

        for(int a = 0; a < (int)widgets.size();a++)
        {
            /*widgets[a]->Change_coords(widgets[a]->orig_x1 - scb_horizontal->change,
                    widgets[a]->orig_y1 - scb_vertical->change, widgets[a]->wd_width, widgets[a]->wd_height);

            widgets[a]->wd_md->Change_coords(wd_x1 + widgets[a]->orig_x1 - scb_horizontal->change,
                    wd_y1 + widgets[a]->orig_y1 - scb_vertical->change, widgets[a]->wd_width, widgets[a]->wd_height);*/

            if(widgets[a]->orig_x2 + 1 > wid - orig_x1)
            {
                wid = widgets[a]->orig_x2 + 1;
            }
            if(widgets[a]->orig_y2 + 1 > hei - orig_y1)
            {
                hei = widgets[a]->orig_y2 + 1;
            }
        }

        if(wid < wd_width - scb_thickness)
        {
            wid = wd_width - scb_thickness;
        }
        if(hei < wd_height - scb_thickness)
        {
            hei = wd_height - scb_thickness;
        }

        scb_horizontal->Change_real_size_r(wid);
        scb_vertical->Change_real_size_r(hei);
        r_size_w = wid;
        r_size_h = hei;

        for(int a = 0; a < (int)widgets.size();a++)
        {
            /*widgets[a]->Change_coords(widgets[a]->orig_x1 - scb_horizontal->change,
                    widgets[a]->orig_y1 - scb_vertical->change, widgets[a]->wd_width, widgets[a]->wd_height);

            widgets[a]->wd_md->Change_coords(wd_x1 + widgets[a]->orig_x1 - scb_horizontal->change,
                    wd_y1 + widgets[a]->orig_y1 - scb_vertical->change, widgets[a]->wd_width, widgets[a]->wd_height);*/
            widgets[a]->Change_coords(widgets[a]->orig_x1 - scb_horizontal->change/zoom,
                    widgets[a]->orig_y1 - scb_vertical->change/zoom, widgets[a]->wd_width, widgets[a]->wd_height);

            widgets[a]->wd_md->Change_coords(wd_x1 + widgets[a]->orig_x1*zoom - scb_horizontal->change,
                                             wd_y1 + widgets[a]->orig_y1*zoom - scb_vertical->change,
                                             (widgets[a]->orig_x2 - widgets[a]->orig_x1)*zoom,
                                             (widgets[a]->orig_y2 - widgets[a]->orig_y1)*zoom);
        }
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

    void ScrollableArea::Change_coords(float x1, float y1, float width, float height)
    {
        wd_Change_coords(x1,y1,width, height);
        if(wd_bmp != nullptr)
        {
            al_destroy_bitmap(wd_bmp);
        }
        wd_bmp = al_create_bitmap(wd_width, wd_height);

        scb_vertical->Change_coords(wd_width - scb_thickness,0,scb_thickness,
                                       wd_height- scb_thickness);

        scb_horizontal->Change_coords(0,wd_height - scb_thickness, wd_width - scb_thickness,
                                       scb_thickness);

        scb_vertical->wd_md->Change_coords(wd_x2 - scb_thickness, wd_y1, scb_thickness,
                                       wd_height- scb_thickness);
        scb_horizontal->wd_md->Change_coords(wd_x1, wd_y2 - scb_thickness, wd_width - scb_thickness,
                                       scb_thickness);
        wd_md->Change_coords(wd_x1, wd_y1, wd_width - scb_thickness, wd_height - scb_thickness);

        I_added_new_widgets();
    }

}
