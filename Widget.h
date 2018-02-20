#ifndef WIDGET_H
#define WIDGET_H

#include <allegro5/allegro.h>
#include <list>
#include <string>
#include <functional>

namespace rGUI
{

struct Theme{
/*------Background bitmap------*/
    /**this bitmap is your responsibility*/
    ALLEGRO_BITMAP* background_image = nullptr;
    /**if background_image == nullptr, those have no effect*/
    double background_image_align_x = 0.5;
    double background_image_align_y = 0.5;
    /**parts that are sticking outside of border rectangle will not be drawn (cornes sticking our of round border)*/
    enum BCK_IMG_MODS{
        NORMAL,             //will draw background_image acoording to background_image_align
        REPEAT,             //will draw background_image from [0,0] to fill whole widget bitmap (minus outwards thickness)
        FIT_STRECH,         //will strech background_image's width and height to fill whole widget bitmap (minus outwards thickness)
        FIT_MAX_DIMESION    //will uniformly scale background_image , so (at leas) one dimention is as big as possible for widget bitmap
        };
    BCK_IMG_MODS background_image_midifiers = NORMAL;
    
/*------different colors------*/
    ALLEGRO_COLOR background_color = al_map_rgb(255,255,255);
    ALLEGRO_COLOR border_color = al_map_rgb(255,0,0);
    ALLEGRO_COLOR text_color = al_map_rgb(255,0,255);
    ALLEGRO_COLOR on_click_fade_color = al_map_rgba(255,255,255,100);
    ALLEGRO_COLOR element1_color = al_map_rgb(255,255,255);
    ALLEGRO_COLOR element2_color = al_map_rgb(255,255,255);
    ALLEGRO_COLOR element3_color = al_map_rgb(255,255,255);
    
/*------border thickness------*/
    double border_thickness = 1;
    double on_click_border_addthickness_outwards = 1;
    double on_click_border_addthickness_inwards = 1;
    
/*------border roundness------*/
    double corner_round_x = 0;
    double corner_round_y = 0;
    
/*------text align------*/
    /**where should be middle of the text for each axis, in % /100 */
    double text_align_x = 0.5;
    double text_align_y = 0.5;
    
/*------scrolling------*/
    /**pixels per scroll*/
    double scroll_speed = 10;
    
/*------cursor------*/
    /**cursor blinks per second*/
    double cursor_bps = 0.75;
    
/*------font and text size------*/
    /**if fontfile path is passed to widget and text_size_override == true, it wil create text of this 
     * size. "-20" ensures that font will have max height of 20pixels
    */
    signed int text_size = -20;
    bool text_size_override = false;
    int font_flags = 0;
    std::string font_file = "";
    /**if true, it will load and use this (font_file) font no matter what(text_size_override)*/
    bool font_file_override = false;
    
/*------widget bitmap flags------*/
    int bitmap_flags = ALLEGRO_VIDEO_BITMAP | ALLEGRO_CONVERT_BITMAP | ALLEGRO_MIN_LINEAR |
                       ALLEGRO_MAG_LINEAR;
                       
    Theme operator=(const Theme& t)
    {
        background_image = t.background_image;
        background_image_align_x = t.background_image_align_x;
        background_image_align_y = t.background_image_align_y;
        background_image_midifiers = t.background_image_midifiers;
        background_color = t.background_color;
        border_color = t.border_color;
        text_color = t.text_color;
        on_click_fade_color = t.on_click_fade_color;
        element1_color = t.element1_color;
        element2_color = t.element2_color;
        element3_color = t.element3_color;
        border_thickness = t.border_thickness;
        on_click_border_addthickness_outwards = t.on_click_border_addthickness_outwards;
        on_click_border_addthickness_inwards = t.on_click_border_addthickness_inwards;
        corner_round_x = t.corner_round_x;
        corner_round_y = t.corner_round_y;
        scroll_speed = t.scroll_speed;
        cursor_bps = t.cursor_bps;
        text_size = t.text_size;
        text_size_override = t.text_size_override;
        font_flags = t.font_flags;
        font_file = t.font_file;
        font_file_override = t.font_file_override;
        bitmap_flags = t.bitmap_flags;
        return *this;
    }
}DefaultTheme;

class WidgetVisitor;    
class Scene;    

/**
 * @class Widget
 * @author 
 * @date 02/20/18
 * @file Widget.h
 * @brief base abstract class for widgets
 */
class Widget
{
public:
    virtual ~Widget();
    
/*------rendering------*/
    
    /**
     * @brief true - will draw, false -will not draw
     * @param _tof
     */
    virtual void setRenderingStatus(bool _tof);
    /**
     * @brief 
     * @return true - will draw when Render() is called, false - will not draw 
     */
    virtual bool getRenderingStatus() const;
    
    /**
     * @brief draws this widget bitmap to location if getRenderingStatus() == true
     */
    virtual void Render();
    
    /**
     * @brief returns bitmap with widget renderet on it
     * @return 
     */
    virtual const ALLEGRO_BITMAP* getWidgetBitmap() const;
    
/*------event detection------*/
    
    /**
     * @brief true - all detectors will detect, false - all detectors will not detect
     * @param _tof
     */
    virtual void setDetectionStatus(bool _tof);
    
    /**
     * @brief true - all detectors will detect when Detect() is called, false - all detectors will not detect
     * @return 
     */
    virtual bool getDetectionStatus() const;
    
    /**
     * @brief run event detection
     */
    virtual void Detect();
    
    /**
     * @brief run event detection with specific event
     * @param _e
     */
    virtual void Detect(ALLEGRO_EVENT * _e);
    
/*------rendering and event detection combined------*/
    
    /**
     * @brief 
     * @return false - rendering and event detection is disabled, true - both are enabled 
     */
    virtual bool getWidgetStatus();
    
    /**
     * @brief calls setDetectionStatus(bool) and setRenderingStatus(bool) with _tof as parameter
     * @param _tof
     */
    virtual void setWidgetStatus(bool _tof);
    
    /**
     * @brief will change position for Render() and Detect() for this widget and all his childrens,
     *MouseDetector's coord are changed according to current transform
     * @param _x
     * @param _y
     */
    virtual void setPosition(double _x, double _y);
    
    /**
     * @brief will change position and size(scale for childrens) for this widget and all his childrens,
     *MouseDetector's coord are changed according to current transform
     * @param _x left top x
     * @param _y left top y
     * @param _w width
     * @param _h height
     */
    virtual void setPosition(double _x, double _y, double _w, double _h);
    
    const double getX() const;
    const double getY() const;
    const double getX2() const;
    const double getY2() const;
    const double getWidth() const;
    const double getHeight() const;
    
/*------Theme------*/
    
    /**
     * @brief Will set theme and, if needed resize bitmap (outward border...)
     * @param _t
     */
    virtual void setTheme(Theme* _t);
    
    /**
     * @brief You can edit current theme by edditing return Theme of this function
     * @return 
     */
    virtual Theme* getTheme();
    
/*------Widget------*/
    virtual void addWidget(Widget *_w);
    virtual void removeWidget(Widget *_w);
    virtual std::list<Widget *> getChildrens();
    
    /**
     * @brief Will mowe this Widget to last position in Scene->childrens list
     */
    virtual void setLast();
    
    /**
     * @brief Was clicked
     * @return 
     */
    virtual bool isFocused() const;
    /**
     * @brief when was lastly clicked
     * @return 
     */
    virtual const double getFocusTimeStamp() const;
    
/*------Visitor------*/
    virtual void accept(WidgetVisitor *_v);

/*------Clicking------*/
    /**reference to function that will be called when md->just_clicked == true*/
    std::function<void(Widget*)> onClick;
    /**reference to function that will be called when md->just_released == true*/
    std::function<void(Widget*)> onRelease;
    /**reference to function that will be called when md->clicking == true*/
    std::function<void(Widget*)> onClicking;
    /**reference to function that will be called when md->mouse_on_it == true*/
    std::function<void(Widget*)> onMouseOnIt;
protected:
    class MouseDetector{
    private:
        /**becouse 1 Widget can have more MouseDetectors*/
        double md_x1, md_y1, md_x2, md_y2;
    public:
        MouseDetector(double _x1, double _y1, double _x2, double _y2);
        void Detect(double mouse_x, double mouse_y, ALLEGRO_EVENT *e);
        void Detect();
        void changePosition(double _x1, double _y1, double _x2, double _y2);
        bool mouse_on_it = false;
        bool just_clicked = false;
        bool just_released = false;
        bool clicking = false;
    };
    
    virtual void createBitmap();
    virtual void DrawBackGround();
    virtual void DrawBorder();
    virtual void DrawClickingBorder();
    virtual void DrawClickingFading();
    
    double x, y, w, h, x2, y2;
    ALLEGRO_BITMAP* bmp;
    Scene* scene;
    std::list<Widget *> childrens;
    MouseDetector *md;
    Theme *theme;
    
    bool recreateBMP = false;
    bool redrawBMP = true;
    
    bool focus = false;
    double focus_time_stamp = -1; // = clock() onClick;
};

}

#endif // WIDGET_H
