#include "rGUI.h"

namespace rGUI //ClickableText
{
    Label::Label(float x, float y, float width, float height, std::string texts,
          std::string font_file, int allegro_text_flag,Theme *thm, bool bitmap_only)
    : ClickableText(x, y,texts, font_file, allegro_text_flag, height/5.0f * 3.8, thm, bitmap_only)
    {

    }

    Label::~Label()
    {

    }

    int Label::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)
    {

    }

    void Label::Print()
    {

    }
}
