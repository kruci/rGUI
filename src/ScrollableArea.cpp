#include "rGUI.h"

namespace rGUI //ScrollableArea
{
    ScrollableArea::ScrollableArea(float x, float y, float width, float height, float real_width, float real_height,
                   Theme *thm)
    : Widget(x,y, float(width+scb_thickness), float(height+scb_thickness), thm, false), r_size_w(real_width), r_size_h(real_height)
    {
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
                   Theme *thm, float)
    : Widget(x1,y1,x2+scb_thickness, y2+scb_thickness,0, thm, false), r_size_w(real_width), r_size_h(real_height)
    {
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
            scb_vertical->Scrolling_input(ev, scalex, scaley);
        }
        scb_vertical->Input(ev, scalex, scaley);
        scb_horizontal->Input(ev, scalex, scaley);

    }

    void ScrollableArea::Print()
    {
        al_set_target_bitmap(wd_bmp);
        al_clear_to_color(al_map_rgba(0,0,0,0));

        al_draw_filled_rounded_rectangle(0, 0, wd_width, wd_height,
                                wd_roundx, wd_roundy, wd_c_background);
        al_draw_rounded_rectangle(1, 0, wd_width, wd_height,
                                wd_roundx, wd_roundy, wd_c_outline, wd_thickness);

        scb_horizontal->Print();
        scb_vertical->Print();


        al_set_target_backbuffer(al_get_current_display());
        if(wd_bitmap_only == false)
        {
            al_draw_bitmap(wd_bmp, wd_x1, wd_y1, 0);
        }
    }

}
