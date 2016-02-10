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
    class BigBitmap;

    class Button;
    class CheckBox;
    class ClickableText;
    class SlideBar;
    class ScrollBar;


struct Theme{
    float roundx = 0;
    float roundy = 0;
    float thickness = 1;
    float added_thickness = 1;

    ALLEGRO_COLOR
        c_outline = al_map_rgb(255,255,255),
        c_background = al_map_rgb(0,0,66),
        c_text = al_map_rgb(255,255,255),
        c_clicking = al_map_rgba(0,0,0,150);
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

    int md_mouse_button = 1;

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
    int wd_mouse_button = 1;
    Widget *wd_child = nullptr;

    float comentary_text_y;
    float wd_x1, wd_y1, wd_x2, wd_y2, wd_width, wd_height;
    bool wd_bitmap_only = false;
    float wd_roundx = 0, wd_roundy = 0, wd_thickness = 1, wd_added_thickness = 1;
    ALLEGRO_COLOR wd_c_outline = al_map_rgb(255,255,255),
                  wd_c_background = al_map_rgb(0,0,66),
                  wd_c_text = al_map_rgb(255,255,255),
                  wd_c_clicking = al_map_rgba(0,0,0,150);
    ALLEGRO_BITMAP *wd_bmp = nullptr;
    MouseDetector *wd_md = nullptr;

    Widget(float x1, float y1, float x2, float y2, float, Theme *thm, bool bitmap_only);
    Widget(float x, float y, float width, float height, Theme *thm, bool bitmap_only);
    ~Widget();

    void wd_Change_coords(float x1, float y1, float x2, float y2, float);
    void wd_Change_coords(float x1, float y1, float width, float height);
    void wd_Change_coords_r(float &x1, float &y1, float &x2, float &y2, float);
    void wd_Change_coords_r(float &x1, float &y1, float &width, float &height);

    void Update_theme(Theme *thm);

    virtual int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    virtual void Print();
    virtual void Change_coords(float x1, float y1, float width, float height);
    virtual void Change_coords_r(float &x1, float &y1, float &width, float &height);
};

class Button : public Widget
{
protected:
    float text_x, text_y, font_height;
    ALLEGRO_FONT *font = nullptr;

    bool delete_font = true;

    void recalculate_text(std::string fontt);
    std::string bt_font_file;
public:
    std::string text;

    Button(float x, float y, float width, float height, std::string texts, std::string fontfile, Theme *thm);
    Button(float width, float height, std::string texts, std::string fontfile, Theme *thm);
    ~Button();

    int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    void Print();
    void Change_coords(float x1, float y1, float width, float height);
    void Change_coords_r(float &x1, float &y1, float &width, float &height);
};

class CheckBox : public Widget{
public:
    bool selected = false;

    CheckBox(float x, float y, float width, float height, Theme *thm, bool is_selected);
    CheckBox(float width, float height, Theme *thm, bool is_selected);
    ~CheckBox();

    int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    void Print();
};

class ClickableText : public Widget{
public:
    int print_flag;
    float fsize;
    std::string text;
    ALLEGRO_FONT *font = nullptr;
    bool delete_font = true;
    bool disable_clicking_shadow = false;

    ClickableText(float x, float y, std::string texts, std::string fontfile,int allegro_text_flag,
                  float font_height, Theme *thm, bool bitmap_only);
    ClickableText(float x, float y, std::string texts, ALLEGRO_FONT *fnt,int allegro_text_flag,
                  Theme *thm, bool bitmap_only);
    ~ClickableText();

    int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    void Print();
};

class SlideBar : public Widget{
    float line_y1, line_y2, line_x1, line_x2;
    float slide_x1, slide_y1, slide_x2, slide_y2;
    float line_thickness = 11, slide_height = line_thickness*3, slide_width = slide_height/3.0f;
    float poz;
    int sb_mouse_z;

    void sb_calculate_slide_poz();
    void sb_calculate_line_poz();

    int values;
    bool sb_recalculate_slide_poz = false;
public:
    bool vertical = false;
    int value, val_max, val_min;

    SlideBar(float x, float y, float width, float height, int minval, int maxval,
             Theme *thm, bool vertical = false, bool bitmaponly = false);
    ~SlideBar();

    int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    void Print();

    void Change_coords(float x1, float y1, float width, float height);
    void Change_coords_r(float &x1, float &y1, float &width, float &height);
};

class BitmapButton : public Widget{
public:
    float bb_orig_width, bb_orig_height;

    ALLEGRO_BITMAP *bmp = nullptr;
    bool delete_bitmap = false;

    BitmapButton(float x, float y, float width, float height, std::string image, Theme *thm, bool bitmaponly);
    BitmapButton(float x, float y, float width, float height, ALLEGRO_BITMAP *image, Theme *thm, bool bitmaponly);
    BitmapButton(float x, float y, std::string image, Theme *thm, bool bitmaponly);
    BitmapButton(float x, float y, ALLEGRO_BITMAP *image, Theme *thm, bool bitmaponly);
    ~BitmapButton();

    int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    void Print();
};

class BigBitmap
{
private:
    std::string bitmap_name;
    ALLEGRO_BITMAP *big_bitmap = nullptr;
    ALLEGRO_BITMAP *tmpbmp = nullptr;
    float region_size_w = 512;
    float region_size_h = 512;
    float orig_width, orig_height;
    int w_regions, h_regions;
    float print_width = region_size_w, print_height = region_size_h;
    int orig_flags;

    std::vector<ALLEGRO_BITMAP*> bitmaps;
public:
    ALLEGRO_COLOR c_clear = al_map_rgb(0,0,0);
    float width, height;
    bool resizable = true;

    BigBitmap(std::string bitmap_to_load, float sector_width, float sector_height);
    BigBitmap(std::string bitmap_to_load, float sector_width, float sector_height, float print_width, float print_height);
    ~BigBitmap();

    bool Draw_bitmap_region(float sx, float sy, float sw, float sh, float dx, float dy, int flags);//slower
    bool Draw_bitmap_region(float sx, float sy, float dx, float dy, int flags);
};

class ScrollBar : public Widget
{
private:
    float sroller_x1, sroller_y1, sroller_x2, sroller_y2, sroller_width, sroller_height;
    int scb_mouse_z;
    bool scb_dorecalculate_scroller_poz = false;
    float rb_ratio;
    float click_x, click_y;

    void scb_recalculate_scroller_poz();
    void scb_recalculate_rb_ratio();
public:
    float c_background_mult = 0.9f;
    bool vertical = false;
    float r_size;
    float change = 0;
    float scroll_step = 30;

    ScrollBar(float x, float y, float width, float height, float real_size, Theme *thm, bool vertical, bool bitmap_only);
    ~ScrollBar();

    int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    void Print();

    void Change_coords(float x1, float y1, float width, float height);
    void Change_coords_r(float &x1, float &y1, float &width, float &height);
    void Change_real_size(float s);
    void Change_real_size_r(float &s);
};

}
#endif // _RGUI_H__
