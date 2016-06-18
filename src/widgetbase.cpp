#include "rGUI.h"

namespace rGUI //mousestate
{
    ALLEGRO_MOUSE_STATE *mouse_state = nullptr;
    ALLEGRO_KEYBOARD_STATE *keyboard_state = nullptr;

    void no_null(){};

    bool _multilinecb(int _line_num, const char *_line, int _sizes, void *_extra)
    {
        ((ml_data*)_extra)->lines = _line_num + 1;

        std::string currentlinestr = ((std::string)_line).substr(0,_sizes);
        int currnetlinesize = al_get_text_width(((ml_data*)_extra)->font, currentlinestr.c_str());

        if(((ml_data*)_extra)->maxlinesize < currnetlinesize)
        {
            ((ml_data*)_extra)->maxlinesize = currnetlinesize;
            ((ml_data*)_extra)->longesttext = currentlinestr;
        }
        return true;
    }

    void Init()
    {
        mouse_state = new ALLEGRO_MOUSE_STATE;
        keyboard_state = new ALLEGRO_KEYBOARD_STATE;
    }

    void GetStates()
    {
        al_get_mouse_state(mouse_state);
        al_get_keyboard_state(keyboard_state);
    }

    void End()
    {
        if(keyboard_state != nullptr)
            delete keyboard_state;
        if(mouse_state != nullptr)
            delete mouse_state;
    }
}

namespace rGUI //Mouse detector
{
    MouseDetector::MouseDetector(float x1, float y1, float x2, float y2, float sotherearenosafunctions)
                                : md_x1(x1), md_y1(y1), md_x2(x2), md_y2(y2),
                                md_width(abs(x1 - x2)), md_height(abs(y1 - y2))
    {}

    MouseDetector::MouseDetector(float x, float y, float width, float height)
                                : md_x1(x), md_y1(y), md_x2(x+width), md_y2(y+height),
                                md_width(width), md_height(height)
    {}

    MouseDetector::~MouseDetector()
    {}


    float MouseDetector::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
     /**return 0   if mouse is on button but mouse button isnt down
        return 1   if mouse is on button and mouse button is down
        return 2   if mouse clicked this button
        return 3   if mouse isnt on button and mouse isnt clicking
        return 9   if button is not active
        return 999 if something went wrong**/

        if(md_active == false)
        {
            return 9;
        }
        md_just_clicked = false;



        if(mouse_state->x >= md_x1*scalex && mouse_state->x <= md_x2*scalex &&
           mouse_state->y >= md_y1*scaley && mouse_state->y <= md_y2*scaley)
        {
            if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && ev.mouse.button == md_mouse_button && md_clicking == true)
            {
                if(md_clicked == true)
                {
                    md_clicked_again = true;
                    Clicked_again_callback();
                }
                md_clicking = false;
                md_clicked = true;
                md_just_clicked = true;
                Just_clicked_callback();
                return 2;
            }
            else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == md_mouse_button || md_clicking == true)
            {
                md_clicking = true;
                Clicking_callback();
                return 1;
            }
            else
            {
                md_mouse_on_it = true;
                Mouse_on_it_callback();
                md_clicking = false;
                Clicking_callback();
                return 0;
            }
        }
        else
        {
            md_mouse_on_it = false;
            md_clicking = false;
            return 3;
        }

        return 999;
    }

    void MouseDetector::Change_coords(float x1, float y1, float x2, float y2, float sotherearenosafunctions)
    {
        md_x1 = x1;
        md_x2 = x2;
        md_y1 = y1;
        md_y2 = y2;
        md_width  = abs(x1 - x2);
        md_height = abs(y1 - y2);
    }

    void MouseDetector::Change_coords_r(float &x1, float &y1, float &x2, float &y2, float sotherearenosafunctions)
    {
        md_x1 = x1;
        md_x2 = x2;
        md_y1 = y1;
        md_y2 = y2;
        md_width  = abs(x1 - x2);
        md_height = abs(y1 - y2);
    }

    void MouseDetector::Change_coords(float x, float y, float width, float height)
    {
        md_x1 = x;
        md_x2 = x + width;
        md_y1 = y;
        md_y2 = y + height;
        md_width  = width;
        md_height = height;
    }

    void MouseDetector::Change_coords_r(float &x, float &y, float &width, float &height)
    {
        md_x1 = x;
        md_x2 = x + width;
        md_y1 = y;
        md_y2 = y + height;
        md_width  = width;
        md_height = height;
    }

}

namespace rGUI //Widget
{
    Widget::Widget(float x1, float y1, float x2, float y2,float sotherearenosafunctions, Theme *thm, bool bitmap_only = false)
                                : wd_x1(x1), wd_y1(y1), wd_x2(x2), wd_y2(y2),
                                wd_width(abs(x1 - x2)), wd_height(abs(y1 - y2)), comentary_text_y(y1 + wd_height/2),
                                wd_bitmap_only(bitmap_only)
    {
        orig_x1 = x1;
        orig_x2 = x2;
        orig_y1 = y1;
        orig_y2 = y2;

        wd_roundx = thm->roundx;
        wd_roundy = thm->roundy;
        wd_thickness = thm->thickness;
        wd_c_outline = thm->c_outline;
        wd_c_background = thm->c_background;
        wd_c_text = thm->c_text;
        wd_c_clicking = thm->c_clicking;
        wd_added_thickness = thm->added_thickness;

        if(wd_bitmap_only == true)
        {
            wd_bmp = al_create_bitmap(wd_width, wd_height);
            wd_Change_coords(0, 0, wd_width, wd_height);
        }

        wd_md = new MouseDetector(x1,y1, wd_width, wd_height);

        comentary_text_y = wd_y1 + wd_height/2.0f;
    }

    Widget::Widget(float x, float y, float width, float height, Theme *thm, bool bitmap_only = false)
                                : wd_x1(x), wd_y1(y), wd_x2(x+width), wd_y2(y+height),
                                wd_width(width), wd_height(height), comentary_text_y(y + width/2),
                                wd_bitmap_only(bitmap_only)
    {
        orig_x1 = wd_x1;
        orig_x2 = wd_x2;
        orig_y1 = wd_y1;
        orig_y2 = wd_y2;

        wd_roundx = thm->roundx;
        wd_roundy = thm->roundy;
        wd_thickness = thm->thickness;
        wd_c_outline = thm->c_outline;
        wd_c_background = thm->c_background;
        wd_c_text = thm->c_text;
        wd_c_clicking = thm->c_clicking;
        wd_added_thickness = thm->added_thickness;

        if(wd_bitmap_only == true)
        {
            wd_bmp = al_create_bitmap(wd_width, wd_height);
            wd_Change_coords(0, 0, wd_width, wd_height);
        }

        wd_md = new MouseDetector(x,y, wd_width, wd_height);

        comentary_text_y = wd_y1 + wd_height/2.0f;
    }

    Widget::~Widget()
    {
        if(wd_bmp == nullptr)
            al_destroy_bitmap(wd_bmp);

        if(wd_child == nullptr)
            delete wd_child;

        delete wd_md;
    }

    void Widget::wd_Change_coords(float x1, float y1, float x2, float y2, float sotherearenosafunctions)
    {
        if((Is_changed(abs(x1-x2), wd_width) || Is_changed(abs(y1-y2), wd_height)) && wd_bitmap_only == true)
        {
            al_destroy_bitmap(wd_bmp);
            wd_bmp = al_create_bitmap(abs(x1-x2), abs(y1-y2));
        }

        wd_x1 = x1;
        wd_x2 = x2;
        wd_y1 = y1;
        wd_y2 = y2;
        wd_width  = abs(x1 - x2);
        wd_height = abs(y1 - y2);
    }

    void Widget::wd_Change_coords_r(float &x1, float &y1, float &x2, float &y2, float sotherearenosafunctions)
    {
        if((Is_changed(abs(x1-x2), wd_width) || Is_changed(abs(y1-y2), wd_height)) && wd_bitmap_only == true)
        {
            al_destroy_bitmap(wd_bmp);
            wd_bmp = al_create_bitmap(abs(x1-x2), abs(y1-y2));
        }

        wd_x1 = x1;
        wd_x2 = x2;
        wd_y1 = y1;
        wd_y2 = y2;
        wd_width  = abs(x1 - x2);
        wd_height = abs(y1 - y2);
    }

    void Widget::wd_Change_coords(float x, float y, float width, float height)
    {
        if((Is_changed_r(width, wd_width) || Is_changed_r(width, wd_height)) && wd_bitmap_only == true)
        {
            al_destroy_bitmap(wd_bmp);
            wd_bmp = al_create_bitmap(width, height);
        }

        wd_x1 = x;
        wd_x2 = x + width;
        wd_y1 = y;
        wd_y2 = y + height;
        wd_width  = width;
        wd_height = height;
    }

    void Widget::wd_Change_coords_r(float &x, float &y, float &width, float &height)
    {
        if((Is_changed_r(width, wd_width) || Is_changed_r(width, wd_height)) && wd_bitmap_only == true)
        {
            al_destroy_bitmap(wd_bmp);
            wd_bmp = al_create_bitmap(width, height);
        }

        wd_x1 = x;
        wd_x2 = x + width;
        wd_y1 = y;
        wd_y2 = y + height;
        wd_width  = width;
        wd_height = height;

    }

    bool Widget::Is_changed(float a, float b)
    {
        if(a != b)
        {
            return true;
        }
        return false;
    }

    bool Widget::Is_changed_r(float &a, float &b)
    {
        if(a != b)
        {
            return true;
        }
        return false;
    }


    void Widget::wd_PrintBegin()
    {
        if(wd_bitmap_only == true)
        {
            wd_ref_bmp = al_get_target_bitmap();
            al_set_target_bitmap(wd_bmp);
            al_clear_to_color(al_map_rgba(0,0,0,0));
        }
        else if(print_active == false)
        {
            return;
        }
    }

    void Widget::wd_PrintEnd()
    {
        if(wd_bitmap_only == true)
        {
            al_set_target_bitmap(wd_ref_bmp);
        }
    }

    void Widget::Update_theme(Theme *thm)
    {
        wd_roundx = thm->roundx;
        wd_roundy = thm->roundy;
        wd_thickness = thm->thickness;
        wd_c_outline = thm->c_outline;
        wd_c_background = thm->c_background;
        wd_c_text = thm->c_text;
        wd_c_clicking = thm->c_clicking;
        wd_added_thickness = thm->added_thickness;
    }

    void Widget::Change_coords(float x1, float y1, float width, float height)
    {
        wd_Change_coords_r(x1, y1, width, height);
        wd_md->Change_coords_r(x1, y1, width, height);
    }

    void Widget::Change_coords_r(float &x1, float &y1, float &width, float &height)
    {
        wd_Change_coords_r(x1, y1, width, height);
        wd_md->Change_coords_r(x1, y1, width, height);
    }

    void Widget::Change_print_coords(float x1, float y1, float width, float height)
    {
        wd_Change_coords_r(x1, y1, width, height);
    }

    void Widget::Change_print_coords_r(float &x1, float &y1, float &width, float &height)
    {
        wd_Change_coords_r(x1, y1, width, height);
    }

    int Widget::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley){}
    void Widget::Print()
    {
        wd_PrintBegin();
        al_draw_filled_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_roundx, wd_roundy, wd_c_background);
        al_draw_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_roundx, wd_roundy, wd_c_outline, wd_thickness);
        wd_PrintEnd();
    }
}
