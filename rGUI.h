#ifndef _RGUI_H__
#define _RGUI_H__

//Other
#include <iostream>
#include <vector>
#include <list>
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
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_video.h>

namespace rGUI
{
    extern ALLEGRO_MOUSE_STATE *mouse_state;
    extern ALLEGRO_KEYBOARD_STATE *keyboard_state;
    extern ALLEGRO_EVENT event;
    extern float scale_x , scale_y , offset_x , offset_y,//scale is Screen size divided by display size
                 trans_mouse_x, trans_mouse_y;
    extern bool Init_Allegro();
    extern void Init_rGUI(float _scale_x = 1, float _scale_y = 1, float _offset_x = 0, float _offset_y = 0);
    extern void GetStatesAdnEvents(ALLEGRO_EVENT &ev);
    extern void End();


    extern int error_message(std::string error_string);
    extern void no_null(void* _dat_); //blank function for function pointer
    extern bool _multilinecb(int _line_num, const char *_line, int _sizes, void *_extra); //calculate TextBox multiline dimensions
    struct ml_data{ //used by TextBox for multiline dimensions and custom text draw
        ALLEGRO_FONT *font = nullptr;
        int maxlinesize = 0, lines = 0;
        std::string longesttext = "";
        void *dat = nullptr;
    };

    enum WidgetsTypes//wt
    {
        wt_BUTTON, wt_CHECKBOX, wt_BITMAPBUTTON, wt_CLICKABLETEXT, wt_INPUTFIELD, wt_SCROLLABLEAREA,
        wt_SCROLLBAR, wt_SLIDEBAR, wt_WIDGET, wt_SINGLEKEYINPUTFIELD, wt_LABEL, wt_TEXTBOX
    };

    enum BitFlags_TextBox{//bf
        bf_TOP = 0x002, bf_BOTOM = 0x004, bf_VERTICAL_CENTER = 0x008,
        bf_LEFT = 0x010, bf_RIGHT = 0x020, bf_HORIZONTAL_CENTER = 0x040,
        bf_RESIZE_WIDGET_H = 0x080, bf_RESIZE_WIDGET_W = 0x100, bf_RESIZE_WIDGET = 0x200,
        bf_RESIZE_CONTENT = 0x400,bf_MULTILINE = 0x800, bf_CUSTOM_TEXT_DRAW = 0x1000,
        bf_AS_BUTTON = 0x2000, bf_HAS_FRAME = 0x4000
    };
    enum BitFlags_ScrollableArea{
        bf_VERTICAL_SCROLL = 0x001, bf_HORIZONTAL_SCROLL = 0x002, bf_ZOOMABLE = 0x004
    };
    enum BitFlags_SlideBar_ScrollBar{
        bf_VERTICAL = 0x001, bf_HORIZONTAL = 0x002
    };

    struct Theme;
    class MouseDetector;
    class Widget;
    class BigBitmap;

    class Button;
    class CheckBox;
    class SlideBar;
    class ScrollBar;
    class ScrollableArea;
    class InputField;
    class SingleKeyInputField;
    class ProgressBar;
    class TextBox;
    class DropBoxManager;
    class DropBox;

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

struct DropBox_Item{
    std::string bmp_str = "";
    bool load_bmp_fom_file = false; //set this to false so *bmp would be used
    ALLEGRO_BITMAP *bmp = nullptr;
    float print_x = 0, print_y = 0, print_w = 0, print_h = 0;
    void *data = nullptr;

    void Copy_this_DropBox_item(DropBox_Item *destination)
    {
        if(destination != nullptr)
        {
            delete destination;
        }
        destination = new DropBox_Item;
        destination->bmp_str = bmp_str;
        destination->load_bmp_fom_file = load_bmp_fom_file;
        if(bmp != nullptr)
        {
            destination->bmp = al_clone_bitmap(bmp);
        }
        destination->print_x = print_x;
        destination->print_y = print_y;
        destination->print_w = print_w;
        destination->print_h= print_h;
        if(data != nullptr)
        {
            destination->data = data;
        }
    }

    ~DropBox_Item()
    {
        if(bmp != nullptr)
            al_destroy_bitmap(bmp);
    }
};
extern void Copy_DropBox_item(DropBox_Item *source, DropBox_Item *destination);


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

    int Specific_Input(ALLEGRO_EVENT &ev);
    int Input();

    void Change_coords(float x1, float y1, float x2, float y2, float);
    void Change_coords(float x1, float y1, float width, float height);
    void Change_coords_r(float &x1, float &y1, float &x2, float &y2, float);
    void Change_coords_r(float &x1, float &y1, float &width, float &height);

    void (*Mouse_on_it_callback)(void*) = &no_null;
    void (*Clicking_callback)(void*) = &no_null;
    void (*Just_clicked_callback)(void*) = &no_null;
    void (*Clicked_again_callback)(void*) = &no_null;
    void *md_callback_data = nullptr;
};


class Widget
{
protected:
    ALLEGRO_BITMAP *wd_ref_bmp;
    bool Is_changed(float a, float b);
    bool Is_changed_r(float &a, float &b);

    void wd_CreateBitmap(float w, float h);

    void wd_Change_coords(float x1, float y1, float x2, float y2, float);
    void wd_Change_coords(float x1, float y1, float width, float height);
    void wd_Change_coords_r(float &x1, float &y1, float &x2, float &y2, float);
    void wd_Change_coords_r(float &x1, float &y1, float &width, float &height);

    void wd_Update_theme(Theme *thm);

    void wd_PrintBegin();
    void wd_PrintEnd();
    void wd_Print_Frame();
    void wd_Print_Background();
    void wd_Print_ClickingShadow();
    void wd_Print_AddedThickness();
public:
    int wd_type;
    int wd_bf = 0;

    float comentary_text_y;
    float wd_x1, wd_y1, wd_x2, wd_y2, wd_width, wd_height;
    float orig_x1, orig_x2, orig_y1, orig_y2;
    bool wd_extented_input = false;
    bool wd_print_active = true;
    Theme wd_theme;

    ALLEGRO_BITMAP *wd_bmp = nullptr;
    MouseDetector *wd_md = nullptr;

    std::vector<Widget*> widgets;

    Widget(float x1, float y1, float x2, float y2, float, Theme *thm);
    Widget(float x, float y, float width, float height, Theme *thm);
    ~Widget();

    virtual void Update_theme(Theme *thm);

    virtual int Specific_Input(ALLEGRO_EVENT &ev);
    virtual int Input();
    virtual void Print();
    virtual void Change_coords(float x1, float y1, float width, float height);
    virtual void Change_coords_r(float &x1, float &y1, float &width, float &height);
    virtual void Change_print_coords(float x1, float y1, float width, float height);
    virtual void Change_print_coords_r(float &x1, float &y1, float &width, float &height);

    virtual int Get_flags();
    virtual void Set_flags(int flags);
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
    ~Button();

    void Print();
    void Change_coords(float x1, float y1, float width, float height);
    void Change_coords_r(float &x1, float &y1, float &width, float &height);
    void Change_print_coords(float x1, float y1, float width, float height);
    void Change_print_coords_r(float &x1, float &y1, float &width, float &height);
    void Update_theme(Theme *thm);
};

class CheckBox : public Widget{
public:
    bool selected = false;

    CheckBox(float x, float y, float width, float height, Theme *thm, bool is_selected);
    ~CheckBox();

    int Specific_Input(ALLEGRO_EVENT &ev);
    int Input();
    void Print();
};

class SlideBar : public Widget{
    float line_y1, line_y2, line_x1, line_x2, line_width, line_height;
    float slide_x1, slide_y1, slide_x2, slide_y2;
    float line_thickness = 11, slide_height = line_thickness*3, slide_width = slide_height/3.0f;
    float poz;
    float cx = 0, cy = 0;
    int sb_mouse_z;

    void sb_calculate_slide_poz();
    void sb_calculate_line_poz();

    int values;
    bool sb_recalculate_slide_poz = false;
public:
    bool vertical = false;
    int value, val_max, val_min;

    SlideBar(float x, float y, float width, float height, int minval, int maxval,
             Theme *thm, int bitflags);
    ~SlideBar();

    int Specific_Input(ALLEGRO_EVENT &ev);
    int Input();
    void Print();

    void Change_coords(float x1, float y1, float width, float height);
    void Change_coords_r(float &x1, float &y1, float &width, float &height);
    void Change_print_coords(float x1, float y1, float width, float height);
    void Change_print_coords_r(float &x1, float &y1, float &width, float &height);
    void Set_value(int val);
    int  Get_value();
    void Set_flags(int flags);
    void Update_theme(Theme *thm);
};

class BitmapButton : public Widget{
private:
    float pw, ph;
public:
    float bb_orig_width, bb_orig_height;

    ALLEGRO_BITMAP *bmp = nullptr;
    bool delete_bitmap = false;

    BitmapButton(float x, float y, float width, float height, std::string image, Theme *thm, int bitflags);
    BitmapButton(float x, float y, float width, float height, ALLEGRO_BITMAP *image, Theme *thm, int bitflags);
    ~BitmapButton();

    void Print();
    void Set_flags(int flags);
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
    float cx = 0, cy = 0;
    float md_D_wd_x = 1, md_D_wd_y = 1;

    void scb_recalculate_scroller_poz();
    void scb_recalculate_rb_ratio();
public:
    float c_background_mult = 1.2f;
    bool disable = false;
    bool changed = false;
    float r_size;
    float change = 0;
    float scroll_step = 30;

    ScrollBar(float x, float y, float width, float height, float real_size, Theme *thm, int bitflags);
    ~ScrollBar();

    int Specific_Input(ALLEGRO_EVENT& ev);
    int Input();
    void Print();

    void Change_coords(float x1, float y1, float width, float height);
    void Change_coords_r(float &x1, float &y1, float &width, float &height);
    void Change_print_coords(float x1, float y1, float width, float height);
    void Change_print_coords_r(float &x1, float &y1, float &width, float &height);
    void Change_real_size(float s);
    void Change_real_size_r(float &s);
    void Scrolling_input();
    void Set_change(float chan);
    float Get_change();
    void Set_flags(int flags);
    void Update_theme(Theme *thm);
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

    bool zoomable = false;
    float prevzoom = 1,zoom = 1, zoomstep = 0.05;
    float sca_mouse_z;
    ALLEGRO_TRANSFORM ct, rest;
    int zoomkey = ALLEGRO_KEY_LCTRL, horizontalscrollkey = ALLEGRO_KEY_LSHIFT;


    ScrollBar *scb_vertical = nullptr, *scb_horizontal = nullptr;

    ScrollableArea(float x, float y, float width, float height, float real_width, float real_height,
                   Theme *thm, float scrollbars_thickness, int bitflags);
    ScrollableArea(float x1, float y1, float x2, float y2, float real_width, float real_height,
                   Theme *thm, float scrollbars_thickness, int bitflags, float whatever);
    ~ScrollableArea();

    int Specific_Input(ALLEGRO_EVENT& ev);
    int Input();
    void Print();
    void Set_vertical_sba_status(bool enabled);
    void Set_horizontal_sba_status(bool enabled);
    void Change_coords(float x1, float y1, float width, float height);
    void Set_flags(int flags);

    void I_added_new_widgets();
    void Update_theme(Theme *thm);
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
    ALLEGRO_USTR *al_selected = nullptr;
    bool CTRL_A = false;
    int lenght_limit = 500;

    InputField(float x, float y, float width, float height, std::string font_file, Theme *thm, float FPS);
    InputField(float x, float y, float width, float height, std::string init_text,
               std::string font_file, Theme *thm, float FPS);
    ~InputField();

    virtual int Input();
    virtual int Specific_Input(ALLEGRO_EVENT& ev);
    virtual void Print();
    void Set_text(std::string t);
    std::string Get_text();
    void Update_theme(Theme *thm);

    /*void Change_coords(float x1, float y1, float width, float height);
    void Change_coords_r(float &x1, float &y1, float &width, float &height);*/
};

class SingleKeyInputField : public InputField
{
public:
    int al_key;

    SingleKeyInputField(float x, float y, float width, float height, std::string font_file, Theme *thm);
    SingleKeyInputField(float x, float y, float width, float height, int init_key, std::string font_file,
                        Theme *thm);
    ~SingleKeyInputField();

    int Specific_Input(ALLEGRO_EVENT& ev);
    int Input();
    void Print();
};

class ProgressBar : public Widget
{
private:
public:
    int value = 0;

    ProgressBar(float x, float y, float width, float height, int initial_val_from_0_to_100, Theme *thm);
    ~ProgressBar();

    void Print();

    void Set_value(int val);
    int Get_value();
};

class TextBox : public Widget
{
private:
    ALLEGRO_FONT *font = nullptr;
    bool delete_font = true;
    std::string font_file;
    int print_flag;
    float multiline_height = 0, multiline_longest_text = 0;
public:
    float text_x, text_y, text_height, text_width;
    std::string text; //if you want to set text use Set_text(std::string t)

    //add extra text draw data to mld->data
    ml_data *mld = nullptr;
    //pointer to custom text draw
    bool (*textdrawcallback)(int, const char*, int, void*) = nullptr;
    //pointer to text dimension calculating function
    bool (*textcalccallback)(int, const char*, int, void*) = &_multilinecb;

    TextBox(float x, float y, float width, float height, std::string texts,
          std::string font_file, float font_height, Theme *thm, int bitflags);
    TextBox(float x, float y, float width, float height, std::string texts,
          ALLEGRO_FONT *font, Theme *thm, int bitflags);

    ~TextBox();

    void Print();

    void Change_coords(float x1, float y1, float width, float height);
    void Change_coords_r(float &x1, float &y1, float &width, float &height);
    void Change_print_coords(float x1, float y1, float width, float height);
    void Change_print_coords_r(float &x1, float &y1, float &width, float &height);
    void Set_flags(int flags);
    int  Get_flags();
    void Set_text(std::string t);
    std::string Get_text();
    void Update_theme(Theme *thm);
};

class DropBoxManager : public Widget
{
private:
    DropBox_Item *temp_dpi = nullptr;
    void Set_new_DrobBoxItem(DropBox_Item *dpi);
public:
    bool dbm_w8 = false;
    bool dbm_block_dps = false;
    DropBox *dbm_dragging_DB = nullptr;
    std::list<DropBox*> dbm_dropboxes;
    float dbm_offset_x = 0, dbm_offset_y = 0;


    DropBoxManager();
    ~DropBoxManager();

    void Print();
    int Specific_Input(ALLEGRO_EVENT& ev);
    int Input();

};

class DropBox : public Widget
{
public:
    bool db_dragging = false;
    int id = 0;

    DropBox_Item *db_item = nullptr;
    DropBoxManager *db_dbm = nullptr;

    DropBox(float x, float y, float width, float height, Theme *thm, DropBoxManager *dpm, DropBox_Item *dpi);
    ~DropBox();

    void Print();
    int Specific_Input(ALLEGRO_EVENT& ev);
    int Input();

    void Set_new_DrobBoxItem(DropBox_Item *dpi);
};

}
#endif // _RGUI_H__
