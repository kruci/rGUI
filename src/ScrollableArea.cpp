#include "rGUI.h"

namespace rGUI //ScrollableArea
{
    ScrollableArea::ScrollableArea(float x, float y, float width, float height, float real_width, float real_height,
                   Theme *thm, float scrollbars_thickness, int bitflags)
    : Widget(x,y, float(width+scrollbars_thickness), float(height+scrollbars_thickness), thm),
    r_size_w(real_width), r_size_h(real_height), scb_thickness(scrollbars_thickness)
    {
        wd_type = wt_SCROLLABLEAREA;
        scb_vertical = new   ScrollBar(wd_theme.added_thickness/2 + wd_width - scb_thickness,
                                       wd_theme.added_thickness/2 -1,
                                       scb_thickness-1,
                                       wd_theme.added_thickness/2 + wd_height- scb_thickness +1, real_height, thm, bf_VERTICAL);

        scb_horizontal = new ScrollBar(wd_theme.added_thickness/2-1,
                                       wd_theme.added_thickness/2 + wd_height - scb_thickness,
                                       wd_theme.added_thickness/2 + wd_width - scb_thickness +1,
                                       scb_thickness-1,real_width, thm, bf_HORIZONTAL);

        scb_vertical->wd_md->Change_coords(wd_x2 - scb_thickness, wd_y1, scb_thickness,
                                       wd_height- scb_thickness);
        scb_horizontal->wd_md->Change_coords(wd_x1, wd_y2 - scb_thickness, wd_width - scb_thickness,
                                       scb_thickness);

        wd_md->Change_coords(wd_x1, wd_y1, wd_width - scb_thickness, wd_height - scb_thickness);

        Set_flags(bitflags);
    }

    ScrollableArea::ScrollableArea(float x1, float y1, float x2, float y2, float real_width, float real_height,
                   Theme *thm, float scrollbars_thickness, int bitflags, float whatever)
    : Widget(x1,y1,x2+scrollbars_thickness, y2+scrollbars_thickness,0, thm),
    r_size_w(real_width), r_size_h(real_height), scb_thickness(scrollbars_thickness)
    {
        wd_type = wt_SCROLLABLEAREA;
        scb_vertical = new   ScrollBar(wd_theme.added_thickness/2 + wd_width - scb_thickness,
                                       wd_theme.added_thickness/2 -1,
                                       scb_thickness-1,
                                       wd_theme.added_thickness/2 + wd_height- scb_thickness +1, real_height, thm, bf_VERTICAL);

        scb_horizontal = new ScrollBar(wd_theme.added_thickness/2-1,
                                       wd_theme.added_thickness/2 + wd_height - scb_thickness,
                                       wd_theme.added_thickness/2 + wd_width - scb_thickness +1,
                                       scb_thickness-1,real_width, thm, bf_HORIZONTAL);

        scb_vertical->wd_md->Change_coords(wd_x2 - scb_thickness, wd_y1, scb_thickness,
                                       wd_height- scb_thickness);
        scb_horizontal->wd_md->Change_coords(wd_x1, wd_y2 - scb_thickness, wd_width - scb_thickness,
                                       scb_thickness);

        wd_md->Change_coords(wd_x1, wd_y1, wd_width - scb_thickness, wd_height - scb_thickness);
        Set_flags(bitflags);
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

    int ScrollableArea::Input()
    {
        return Specific_Input(event);
    }

    int ScrollableArea::Specific_Input(ALLEGRO_EVENT &ev)
    {
        if(wd_md->md_active == false)
            return 9;
        wd_md->Specific_Input(ev);

        if(wd_md->md_mouse_on_it == true)
        {

            if(ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == zoomkey)
            {
                sca_mouse_z = mouse_state->z;
            }

            if(al_key_down(keyboard_state, horizontalscrollkey))
            {
                scb_horizontal->Scrolling_input();
            }
            else if(al_key_down(keyboard_state, zoomkey) && (wd_bf & bf_ZOOMABLE))
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
                scb_vertical->Scrolling_input();
            }
        }
        scb_vertical->Specific_Input(ev);
        scb_horizontal->Specific_Input(ev);

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

                widgets[a]->Specific_Input(ev);
            }
        }
        changec_i = false;
    }

    void ScrollableArea::Print()
    {
        wd_PrintBegin();

        wd_Print_Background();

        al_identity_transform(&rest);
        if((wd_bf & bf_ZOOMABLE))
        {
            al_scale_transform(&rest, zoom, zoom);
        }
        al_translate_transform(&rest, wd_theme.added_thickness/2, wd_theme.added_thickness/2);
        al_use_transform(&rest);
        al_set_clipping_rectangle(wd_theme.added_thickness/2 + wd_theme.thickness,
                                  wd_theme.added_thickness/2 + wd_theme.thickness,
                                  wd_theme.added_thickness/2 + wd_width - (scb_horizontal_active == true ? scb_thickness:0),
                                  wd_theme.added_thickness/2 + wd_height - (scb_vertical_active == true ? scb_thickness:0) );

        for(int a = 0; a < (int)widgets.size();a++)
        {

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



                    widgets[a]->wd_md->Change_coords(wd_x1 + widgets[a]->orig_x1*zoom - scb_horizontal->change,
                                                     wd_y1 + widgets[a]->orig_y1*zoom - scb_vertical->change,
                                                     (widgets[a]->orig_x2 - widgets[a]->orig_x1)*zoom,
                                                     (widgets[a]->orig_y2 - widgets[a]->orig_y1)*zoom);
                }
                widgets[a]->Print();
            }
        }
        changec_p = false;

        al_identity_transform(&rest);
        al_translate_transform(&rest, wd_theme.added_thickness/2, wd_theme.added_thickness/2);
        al_use_transform(&rest);

        al_set_clipping_rectangle(0,0,
                                  wd_width + wd_theme.added_thickness, wd_height + wd_theme.added_thickness);
        scb_horizontal->Print();
        scb_vertical->Print();


        al_identity_transform(&rest);
        al_use_transform(&rest);
        wd_Print_Frame();

        wd_PrintEnd();
    }

    void ScrollableArea::I_added_new_widgets()
    {
        float wid = 0, hei = 0;

        for(int a = 0; a < (int)widgets.size();a++)
        {


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
            scb_vertical->wd_print_active = true;
            scb_vertical->wd_md->md_active = true;
            wd_height = wd_height + scb_thickness;

            wd_CreateBitmap(wd_width, wd_height);
        }
        else if(enabled == false && scb_vertical_active == true)
        {
            scb_vertical_active = false;
            scb_vertical->change = 0;
            scb_vertical->wd_print_active = false;
            scb_vertical->wd_md->md_active = false;
            wd_height = wd_height - scb_thickness;

            wd_CreateBitmap(wd_width, wd_height);
        }
    }

    void ScrollableArea::Set_horizontal_sba_status(bool enabled)
    {
        if(enabled == true && scb_horizontal_active == false)
        {
            scb_horizontal_active = true;
            scb_horizontal->change = 0;
            scb_horizontal->wd_print_active = true;
            scb_horizontal->wd_md->md_active = true;
            wd_width = wd_width + scb_thickness;

            wd_CreateBitmap(wd_width, wd_height);
        }
        else if(enabled == false && scb_horizontal_active == true)
        {
            scb_horizontal_active = false;
            scb_horizontal->change = 0;
            scb_horizontal->wd_print_active = false;
            scb_horizontal->wd_md->md_active = false;
            wd_width = wd_width - scb_thickness;

            wd_CreateBitmap(wd_width, wd_height);
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
    void ScrollableArea::Set_flags(int flags)
    {
        wd_bf = flags;
        if((wd_bf & bf_HORIZONTAL_SCROLL) || (wd_bf & bf_HORIZONTAL))
        {
            Set_horizontal_sba_status(true);
        }
        else
        {
            Set_horizontal_sba_status(false);
        }

        if((wd_bf & bf_VERTICAL_SCROLL) || (wd_bf & bf_VERTICAL))
        {
            Set_vertical_sba_status(true);
        }
        else
        {
            Set_vertical_sba_status(false);
        }
    }

    void ScrollableArea::Update_theme(Theme *thm)
    {
        wd_Update_theme(thm);

        for(int a = 0;a < (int)widgets.size();a++)
        {
            widgets[a]->Update_theme(thm);
        }
        scb_horizontal->Update_theme(thm);
        scb_vertical->Update_theme(thm);
    }

}
