#include "rGUI.h"

namespace rGUI //Button
{
    Button::Button(float x, float y, float width, float height, std::string texts, std::string fontfile, Theme *thm)
    : Widget( x, y, width, height, thm), text(texts), bt_font_file(fontfile)
    {
        wd_type = wt_BUTTON;
        constructors_recalculate_text();
    }


    Button::~Button()
    {
        #ifdef DEST_PRINT
        std::cout << "Button dtor"<<std::endl;
        #endif // DEST_PRINT

        if(wd_bmp != nullptr)
            al_destroy_bitmap(wd_bmp);

    }

    void Button::Print()
    {
        wd_PrintBegin();
        wd_Print_Background();
        wd_Print_Frame();

        al_set_clipping_rectangle(wd_theme.added_thickness/2,wd_theme.added_thickness/2,
                                  wd_theme.added_thickness/2 + wd_width -wd_theme.thickness, wd_height + wd_theme.added_thickness/2 -wd_theme.thickness);
        al_draw_text(font,wd_theme.c_text, text_x, text_y,ALLEGRO_ALIGN_CENTRE, text.c_str());
        al_set_clipping_rectangle(0,0,
                                  wd_width + wd_theme.added_thickness, wd_height + wd_theme.added_thickness);

        if(wd_md->md_mouse_on_it == true)
        {
            wd_Print_AddedThickness();
            if(wd_md->md_clicking == true && !(wd_bf & bf_DISABLE_CLICKING_SHADOW))
            {
                wd_Print_ClickingShadow();
            }
        }
        wd_PrintEnd();
    }

    void Button::constructors_recalculate_text()
    {
        font_height =(wd_height / 5.0f) * 3.8f;

        font = al_load_ttf_font(bt_font_file.c_str(), font_height, 0);

        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", bt_font_file.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }
        fontwidth = al_get_text_width(font, text.c_str());

        if(fontwidth >= (wd_width - 2*wd_theme.thickness) )
        {

            font_height = font_height * ((wd_width-2.0f*wd_theme.thickness)/fontwidth);
            al_destroy_font(font);
            font = al_load_ttf_font(bt_font_file.c_str(),font_height,0);
            fontwidth = al_get_text_width(font, text.c_str());
        }
        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", bt_font_file.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }

        text_x = (wd_theme.added_thickness + wd_width)/2;
        text_y = (wd_theme.added_thickness + wd_height)/2 - font_height/2;
    }

    void Button::recalculate_text()
    {
        fontwidth = al_get_text_width(font, text.c_str());

        if(fontwidth >= (wd_width - 2*wd_theme.thickness))
        {
            font_height = font_height * ((wd_width-2.0f*wd_theme.thickness)/fontwidth);
            al_destroy_font(font);
            font = al_load_ttf_font(bt_font_file.c_str(),font_height,0);
            fontwidth = al_get_text_width(font, text.c_str());
        }
        if(font == nullptr)
        {
            al_show_native_message_box(NULL, "Error", "Failed to load font!", bt_font_file.c_str(),
                                       NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }

        text_x = (wd_theme.added_thickness + wd_width)/2;
        text_y = (wd_theme.added_thickness + wd_height)/2 - font_height/2;
    }

    void Button::Change_coords(float x1, float y1, float width, float height)
    {
        wd_Change_coords(x1, y1, width, height);
        wd_md->Change_coords(x1, y1, width, height);
        recalculate_text();
    }

    void Button::Change_coords_r(float &x1, float &y1, float &width, float &height)
    {
        wd_Change_coords(x1, y1, width, height);
        wd_md->Change_coords(x1, y1, width, height);
        recalculate_text();
    }

    void Button::Change_print_coords(float x1, float y1, float width, float height)
    {
        wd_Change_coords(x1, y1, width, height);
        recalculate_text();
    }

    void Button::Change_print_coords_r(float &x1, float &y1, float &width, float &height)
    {
        wd_Change_coords(x1, y1, width, height);
        recalculate_text();
    }
    void Button::Update_theme(Theme *thm)
    {
        wd_Update_theme(thm);
        recalculate_text();
    }
}
