#include "rGUI.h"

namespace rGUI //mousestate
{
    ALLEGRO_MOUSE_STATE *mouse_state = nullptr;
    ALLEGRO_KEYBOARD_STATE *keyboard_state = nullptr;
    ALLEGRO_EVENT event;
    float scale_x = 1, scale_y = 1, offset_x = 0, offset_y = 0, trans_mouse_x, trans_mouse_y;

    void no_null(){};

    int error_message(std::string error_string)
    {
        return al_show_native_message_box(al_get_current_display(), "ERROR", "", error_string.c_str(), nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    }

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

    void Init_rGUI(float _scale_x , float _scale_y, float _offset_x, float _offset_y)
    {
        if(mouse_state == nullptr)
            mouse_state = new ALLEGRO_MOUSE_STATE;
        if(keyboard_state == nullptr)
            keyboard_state = new ALLEGRO_KEYBOARD_STATE;

        scale_x = _scale_x;
        scale_y = _scale_y;
        offset_x = _offset_x;
        offset_y = _offset_y;
    }

    void GetStatesAdnEvents(ALLEGRO_EVENT &ev)
    {
        al_get_mouse_state(mouse_state);
        al_get_keyboard_state(keyboard_state);
        event = ev;
        trans_mouse_x = (mouse_state->x - offset_x) / scale_x;
        trans_mouse_y = (mouse_state->y - offset_y) / scale_y;
    }

    void End()
    {
        if(keyboard_state != nullptr)
            delete keyboard_state;
        if(mouse_state != nullptr)
            delete mouse_state;
    }

    bool Init_Allegro()
    {
        if(!al_init())
        {
            error_message("al_init()");
        }
        if(!al_init_native_dialog_addon())
        {
            error_message("al_init_native_dialog_addon()");
        }
        if(!al_init_primitives_addon())
        {
            error_message("al_init_primitives_addon()");
            return false;
        }
        if(!al_install_keyboard())
        {
            error_message("al_install_keyboard()");
            return false;
        }
        if(!al_install_mouse())
        {
            error_message("al_install_mouse()");
            return false;
        }
        if(!al_init_image_addon())
        {
            error_message("al_init_image_addon()");
            return false;
        }
        if(!al_install_audio())
        {
            error_message("al_install_audio()");
            return false;
        }
        if(!al_init_acodec_addon())
        {
            error_message("al_init_acodec_addon()");
            return false;
        }
        al_init_font_addon(); // returns void
        if(!al_init_ttf_addon())
        {
            error_message("al_init_ttf_addon()");
            return false;
        }
        if(!al_init_video_addon())
        {
            error_message("al_init_video_addon()");
            return false;
        }
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


    int MouseDetector::Specific_Input(ALLEGRO_EVENT &ev)
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



        if(trans_mouse_x >= md_x1 && trans_mouse_x <= md_x2 &&
           trans_mouse_y >= md_y1 && trans_mouse_y <= md_y2)
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

    int MouseDetector::Input()
    {
        return Specific_Input(event);
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
    Widget::Widget(float x1, float y1, float x2, float y2,float sotherearenosafunctions, Theme *thm)
                                : wd_x1(x1), wd_y1(y1), wd_x2(x2), wd_y2(y2),
                                wd_width(abs(x1 - x2)), wd_height(abs(y1 - y2)), comentary_text_y(y1 + wd_height/2)
    {
        orig_x1 = x1;
        orig_x2 = x2;
        orig_y1 = y1;
        orig_y2 = y2;

        /*wd_theme.roundx = thm->roundx;
        wd_theme.roundy = thm->roundy;
        wd_theme.thickness = thm->thickness;
        wd_theme.c_outline = thm->c_outline;
        wd_theme.c_background = thm->c_background;
        wd_theme.c_text = thm->c_text;
        wd_theme.c_clicking = thm->c_clicking;
        wd_theme.added_thickness = thm->added_thickness;

        wd_CreateBitmap(wd_width,wd_height);*/
        wd_Update_theme(thm);

        wd_md = new MouseDetector(x1,y1, wd_width, wd_height);

        comentary_text_y = wd_y1 + wd_height/2.0f;
    }

    Widget::Widget(float x, float y, float width, float height, Theme *thm)
                                : wd_x1(x), wd_y1(y), wd_x2(x+width), wd_y2(y+height),
                                wd_width(width), wd_height(height), comentary_text_y(y + width/2)
    {
        orig_x1 = wd_x1;
        orig_x2 = wd_x2;
        orig_y1 = wd_y1;
        orig_y2 = wd_y2;

        /*wd_theme.roundx = thm->roundx;
        wd_theme.roundy = thm->roundy;
        wd_theme.thickness = thm->thickness;
        wd_theme.c_outline = thm->c_outline;
        wd_theme.c_background = thm->c_background;
        wd_theme.c_text = thm->c_text;
        wd_theme.c_clicking = thm->c_clicking;
        wd_theme.added_thickness = thm->added_thickness;

        wd_CreateBitmap(wd_width,wd_height);*/
        wd_Update_theme(thm);

        wd_md = new MouseDetector(x,y, wd_width, wd_height);

        comentary_text_y = wd_y1 + wd_height/2.0f;
    }

    Widget::~Widget()
    {
        if(wd_bmp == nullptr)
            al_destroy_bitmap(wd_bmp);

        for(int a = 0;a < (int)widgets.size();a++)
        {
            delete widgets[a];
        }
        widgets.clear();

        delete wd_md;
    }

    void Widget::wd_CreateBitmap(float w,float h)
    {
        if(wd_bmp == nullptr)
            al_destroy_bitmap(wd_bmp);

        wd_bmp = al_create_bitmap(w + wd_theme.added_thickness, h + wd_theme.added_thickness);
    }

    void Widget::wd_Change_coords(float x1, float y1, float x2, float y2, float sotherearenosafunctions)
    {
        if((Is_changed(abs(x1-x2), wd_width) || Is_changed(abs(y1-y2), wd_height)))
        {
            wd_CreateBitmap(abs(x1-x2), abs(y1-y2));
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
        if((Is_changed(abs(x1-x2), wd_width) || Is_changed(abs(y1-y2), wd_height)))
        {
            wd_CreateBitmap(abs(x1-x2), abs(y1-y2));
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
        if((Is_changed(width, wd_width) || Is_changed(height, wd_height)))
        {
            wd_CreateBitmap(width, height);
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
        if((Is_changed(width, wd_width) || Is_changed(height, wd_height)))
        {
            wd_CreateBitmap(width, height);
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
        wd_ref_bmp = al_get_target_bitmap();
        al_set_target_bitmap(wd_bmp);
        al_clear_to_color(al_map_rgba(0,0,0,0));
    }

    void Widget::wd_PrintEnd()
    {
        al_set_target_bitmap(wd_ref_bmp);
        if(wd_print_active == true)
        {
            al_draw_bitmap(wd_bmp, wd_x1 - wd_theme.added_thickness/2, wd_y1 - wd_theme.added_thickness/2,0);
        }
    }

    void Widget::Update_theme(Theme *thm)
    {
        wd_Update_theme(thm);
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

    int Widget::Specific_Input(ALLEGRO_EVENT &ev)
    {
        return wd_md->Specific_Input(ev);
    }

    int Widget::Input()
    {
        return wd_md->Input();
    }

    void Widget::Print()
    {
        wd_PrintBegin();
        /*al_draw_filled_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_background);
        al_draw_rounded_rectangle(wd_x1, wd_y1, wd_x2, wd_y2,
                                wd_theme.roundx, wd_theme.roundy, wd_theme.c_outline, wd_theme.thickness);*/
        wd_PrintEnd();
    }

    int Widget::Get_flags()
    {
        return wd_bf;
    }

    void Widget::Set_flags(int flags)
    {
        wd_bf = flags;
    }

    void Widget::wd_Update_theme(Theme *thm)
    {
        wd_theme.roundx = thm->roundx;
        wd_theme.roundy = thm->roundy;
        wd_theme.c_outline = thm->c_outline;
        wd_theme.c_background = thm->c_background;
        wd_theme.c_text = thm->c_text;
        wd_theme.c_clicking = thm->c_clicking;
        //wd_theme.thickness = round(thm->thickness);
        wd_theme.thickness = thm->thickness;
        wd_theme.added_thickness = round(thm->added_thickness);

        if( (int)wd_theme.added_thickness % 2 != 0)
        {
            wd_theme.added_thickness++;
        }
        /*if( (int)wd_theme.thickness % 2 != 0)
        {
            wd_theme.thickness++;
        }*/

        wd_CreateBitmap(wd_width,wd_height);
    }
}
