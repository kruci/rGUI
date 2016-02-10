#include "rGUI.h"

namespace rGUI //Button
{
    Button::Button(float x, float y, float width, float height, std::string texts, std::string fontfile, Theme *thm)
    : Widget( x, y, width, height, thm, false), text(texts), bt_font_file(fontfile)
    {

        recalculate_text(fontfile);

    }

    Button::Button(float width, float height, std::string texts, std::string fontfile, Theme *thm)
    : Widget( 0, 0, width, height, thm, true), text(texts), bt_font_file(fontfile)
    {
        recalculate_text(fontfile);

    }

    Button::~Button()
    {
        if(wd_bmp != nullptr)
            al_destroy_bitmap(wd_bmp);

    }

    int Button::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {
        return wd_md->Input(ev, scalex, scaley);
    }

    void Button::Print()
    {
        wd_PrintBegin();
        al_draw_text(font,wd_c_text, text_x, text_y,0, text.c_str());
        wd_PrintEnd();
    }

    void Button::recalculate_text(std::string fontt)
    {
        font_height =(wd_height / 5.0f) * 3.8f;

        font = al_load_ttf_font(fontt.c_str(), font_height, 0);

        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", fontt.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }
        float fontwidth = al_get_text_width(font, text.c_str());

        if(fontwidth >= wd_width)
        {
            font_height = font_height * (wd_width/fontwidth);
            al_destroy_font(font);
            font = al_load_ttf_font(fontt.c_str(),font_height,0);
        }
        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", fontt.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }

        text_x = wd_x1 + (wd_width - fontwidth) / 2.0f;
        text_y = wd_y1 + (wd_height -al_get_font_ascent(font))   / 2.0f;

    }

    void Button::Change_coords(float x1, float y1, float width, float height)
    {
        wd_Change_coords(x1, y1, width, height);
        wd_md->Change_coords(x1, y1, width, height);
        recalculate_text(bt_font_file);
    }

    void Button::Change_coords_r(float &x1, float &y1, float &width, float &height)
    {
        wd_Change_coords(x1, y1, width, height);
        wd_md->Change_coords(x1, y1, width, height);
        recalculate_text(bt_font_file);
    }
}
