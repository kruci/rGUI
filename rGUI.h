#ifndef _RGUI_H__
#define _RGUI_H__

//Other
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

//ALLEGRO 5
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_memfile.h>

namespace rGUI
{
    extern ALLEGRO_MOUSE_STATE *mouse_state;


    struct Theme;

    class MouseDetector;
    class Widget;
    class Button;
    class CheckBox;


struct Theme{
    float roundx = 0;
    float roundy = 0;
    float thickness = 1;
    float added_thickness = 1;

    ALLEGRO_COLOR
        c_outline = al_map_rgb(255,255,255),
        c_background = al_map_rgb(0,0,66),
        c_text = al_map_rgb(255,255,255);
};

class MouseDetector
{
public:
    float md_x1 = 0, md_y1 = 0, md_x2 = 0, md_y2 = 0, md_width = 0, md_height = 0;

    bool md_clicking = false;
    bool md_clicked = false;
    bool md_clicked_again = false;
    bool md_just_clicked = false;
    bool md_mouse_on_it = false;
    bool md_active = true;

    MouseDetector(float x1, float y1, float x2, float y2, float);
    MouseDetector(float x, float y, float width, float height);
    ~MouseDetector();

    float Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);

    void Change_coords(float x1, float y1, float x2, float y2, float);
    void Change_coords(float x1, float y1, float width, float height);
    void Change_coords_r(float &x1, float &y1, float &x2, float &y2, float);
    void Change_coords_r(float &x1, float &y1, float &width, float &height);
};


class Widget
{
protected:
    bool Is_changed(float a, float b);
    bool Is_changed_r(float &a, float &b);
    void wd_PrintBegin();
    void wd_PrintEnd();

public:
    float comentary_text_y;
    float wd_x1, wd_y1, wd_x2, wd_y2, wd_width, wd_height;
    bool wd_bitmap_only = false;
    float wd_roundx = 0, wd_roundy = 0, wd_thickness = 1, wd_added_thickness = 1;
    ALLEGRO_COLOR wd_c_outline = al_map_rgb(255,255,255),
                  wd_c_background = al_map_rgb(0,0,66),
                  wd_c_text = al_map_rgb(255,255,255);
    ALLEGRO_BITMAP *wd_bmp = nullptr;
    MouseDetector *wd_md = nullptr;

    Widget(float x1, float y1, float x2, float y2, float, Theme *thm, bool bitmap_only);
    Widget(float x, float y, float width, float height, Theme *thm, bool bitmap_only);
    ~Widget();

    void wd_Change_coords(float x1, float y1, float x2, float y2, float);
    void wd_Change_coords(float x1, float y1, float width, float height);
    void wd_Change_coords_r(float &x1, float &y1, float &x2, float &y2, float);
    void wd_Change_coords_r(float &x1, float &y1, float &width, float &height);

    virtual int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    virtual void Print();
};

class Button : protected Widget
{
protected:
    float text_x, text_y, font_height;
    ALLEGRO_FONT *font = nullptr;

    bool delete_font = true;

    void recalculate_text(std::string fontt);
    //void recalculate_text(ALLEGRO_FONT *fontt);
public:
    std::string text;

    Button(float x, float y, float width, float height, std::string texts, std::string fontfile, Theme *thm);
    Button(float width, float height, std::string texts, std::string fontfile, Theme *thm);
    ~Button();

    int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    void Print();
};

class CheckBox : protected Widget{
public:
    bool selected = false;

    CheckBox(float x, float y, float width, float height, Theme *thm, bool is_selected);
    CheckBox(float width, float height, Theme *thm, bool is_selected);
    ~CheckBox();

    int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    void Print();
};

}
#endif // _RGUI_H__
