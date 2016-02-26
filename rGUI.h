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
    extern ALLEGRO_KEYBOARD_STATE *keyboard_state;
    extern void no_null();

    enum WidgetsTypes
    {
        wt_BUTTON, wt_CHECKBOX, wt_BITMAPBUTTON, wt_CLICKABLETEXT, wt_INPUTFIELD, wt_SCROLLABLEAREA,
        wt_SCROLLBAR, wt_SLIDEBAR, wt_WIDGET, wt_SINGLEKEYINPUTFIELD, wt_LABEL
    };

    struct Theme;
    class MouseDetector;
    class Widget;
    class BigBitmap;

    class Button;
    class CheckBox;
    class ClickableText;
    class SlideBar;
    class ScrollBar;
    class ScrollableArea;
    class InputField;
    class SingleKeyInputField;

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

    void (*Mouse_on_it_callback)() = &no_null;
    void (*Clicking_callback)() = &no_null;
    void (*Just_clicked_callback)() = &no_null;
    void (*Clicked_again_callback)() = &no_null;
};


class Widget
{
protected:
    ALLEGRO_BITMAP *wd_ref_bmp;
    bool Is_changed(float a, float b);
    bool Is_changed_r(float &a, float &b);
    void wd_PrintBegin();
    void wd_PrintEnd();

    void wd_Change_coords(float x1, float y1, float x2, float y2, float);
    void wd_Change_coords(float x1, float y1, float width, float height);
    void wd_Change_coords_r(float &x1, float &y1, float &x2, float &y2, float);
    void wd_Change_coords_r(float &x1, float &y1, float &width, float &height);
public:
    int wd_type;
    int wd_mouse_button = 1;
    Widget *wd_child = nullptr;

    float comentary_text_y;
    float wd_x1, wd_y1, wd_x2, wd_y2, wd_width, wd_height;
    float orig_x1, orig_x2, orig_y1, orig_y2;
    bool wd_bitmap_only = false;
    bool wd_extented_input = false;
    bool print_active = true;
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

    void Update_theme(Theme *thm);

    virtual int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    virtual void Print();
    virtual void Change_coords(float x1, float y1, float width, float height);
    virtual void Change_coords_r(float &x1, float &y1, float &width, float &height);
    virtual void Change_print_coords(float x1, float y1, float width, float height);
    virtual void Change_print_coords_r(float &x1, float &y1, float &width, float &height);
};

class Button : public Widget
{
protected:
    float text_x, text_y, font_height;
    float fontwidth;
    ALLEGRO_FONT *font = nullptr;

    bool delete_font = true;

    void constructors_recalculate_text();
    void recalculate_text();
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
    void Change_print_coords(float x1, float y1, float width, float height);
    void Change_print_coords_r(float &x1, float &y1, float &width, float &height);
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
    int print_flag;
public:
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

    virtual void Set_Print_flag(int flag);
    int Get_Print_flag();
};

class SlideBar : public Widget{
    float line_y1, line_y2, line_x1, line_x2, line_width, line_height;
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
    void Change_print_coords(float x1, float y1, float width, float height);
    void Change_print_coords_r(float &x1, float &y1, float &width, float &height);
    void Set_value(int val);
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
    float c_background_mult = 1.2f;
    bool vertical = false;
    bool disable = false;
    bool changed = false;
    float r_size;
    float change = 0;
    float scroll_step = 30;

    ScrollBar(float x, float y, float width, float height, float real_size, Theme *thm, bool vertical, bool bitmap_only);
    ~ScrollBar();

    int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    void Print();

    void Change_coords(float x1, float y1, float width, float height);
    void Change_coords_r(float &x1, float &y1, float &width, float &height);
    void Change_print_coords(float x1, float y1, float width, float height);
    void Change_print_coords_r(float &x1, float &y1, float &width, float &height);
    void Change_real_size(float s);
    void Change_real_size_r(float &s);
    void Scrolling_input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    void Set_change(float chan);
};

class ScrollableArea : public Widget
{
private:
    void sca_recalculate_sc_bars();
    bool changec_i = false, changec_p = false;
public:
    bool scb_vertical_active = true, scb_horizontal_active = true;
    float r_size_w, r_size_h;
    float scb_thickness = 14;
    std::vector<Widget*> widgets;
    ScrollBar *scb_vertical = nullptr, *scb_horizontal = nullptr;

    ScrollableArea(float x, float y, float width, float height, float real_width, float real_height,
                   Theme *thm, float scrollbars_thickness);
    ScrollableArea(float x1, float y1, float x2, float y2, float real_width, float real_height,
                   Theme *thm, float scrollbars_thickness, float whatever);
    ~ScrollableArea();

    int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    void Print();
    void Set_vertical_sba_status(bool enabled);
    void Set_horizontal_sba_status(bool enabled);

    void I_added_new_widgets();
};

class InputField : public Widget
{
private:
    std::string if_help_string;
    ALLEGRO_USTR *if_help_ustr = nullptr;
    ALLEGRO_USTR *if_help_ustr2 = nullptr;
    int if_help_var = 0;
public:
    std::string font_file;
    float texty_shift, text_x, text_y, text_width, font_height;
    float bar_x = 4;
    ALLEGRO_FONT *font = nullptr;
    int if_a = 0;
    bool bmp_only = false;

    void recalculate_text();
//public:
    float if_FPS;
    float bar_width = 3;
    int bar_char_poz = 0;
    std::string text;
    ALLEGRO_USTR *al_text = nullptr;
    int lenght_limit = 500;

    InputField(float x, float y, float width, float height, std::string font_file, Theme *thm, float FPS, bool bitmap_only);
    InputField(float x, float y, float width, float height, std::string init_text,
               std::string font_file, Theme *thm, float FPS, bool bitmap_only);
    ~InputField();

    virtual int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    virtual void Print();
    void Set_text(std::string t);
    std::string Get_text();

    /*void Change_coords(float x1, float y1, float width, float height);
    void Change_coords_r(float &x1, float &y1, float &width, float &height);*/
};

class SingleKeyInputField : public InputField
{
public:
    int al_key;

    SingleKeyInputField(float x, float y, float width, float height, std::string font_file, Theme *thm, bool bitmap_only);
    SingleKeyInputField(float x, float y, float width, float height, int init_key, std::string font_file,
                        Theme *thm, bool bitmap_only);
    ~SingleKeyInputField();

    int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    void Print();
};

class Label : public Widget
{
private:
    ALLEGRO_FONT *font = nullptr;
    bool delete_font = true, recal_f_w = false;
    void recalsulate_text_flag_poz();
    void recalculate_text();
    std::string font_file;
    int print_flag;
public:
    float text_x, text_y, text_height, text_width;
    bool multiline = false;
    std::string text;

    //Single line
    Label(float x1, float y1, float width, float height, std::string texts,
          std::string font_file, int allegro_text_flag,Theme *thm, bool bitmap_only);
    Label(float x1, float y1, float width, std::string texts,
          ALLEGRO_FONT *font, int allegro_text_flag,Theme *thm, bool bitmap_only);
    Label(float x1, float y1, float width, float height, std::string texts,
          ALLEGRO_FONT *font, int allegro_text_flag,Theme *thm, bool bitmap_only);
    //Multiline
    Label(float x1, float y1, float width, float height, std::string texts,
          std::string font_file, float font_height,int allegro_text_flag,Theme *thm, bool bitmap_only, bool multiline);
    Label(float x1, float y1, float width, float height, std::string texts,
          ALLEGRO_FONT *font, int allegro_text_flag,Theme *thm, bool bitmap_only, bool multiline);

    ~Label();

    int Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley);
    void Print();

    void Change_coords(float x1, float y1, float width, float height);
    void Change_coords_r(float &x1, float &y1, float &width, float &height);
    void Change_print_coords(float x1, float y1, float width, float height);
    void Change_print_coords_r(float &x1, float &y1, float &width, float &height);
    virtual void Set_Print_flag(int flag);
    int Get_Print_flag();
};

}
#endif // _RGUI_H__
