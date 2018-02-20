#ifndef WIDGET_H
#define WIDGET_H

#include <allegro5/allegro.h>
#include <list>

namespace rGUI
{

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
     * @param _x
     * @param _y
     * @param _w
     * @param _h
     */
    virtual void setPosition(double _x, double _y, double _w, double _h);
    
protected:
    class MouseDetector{
    private:
        double md_x1, md_y1, md_x2, md_y2;
    public:
        MouseDetector(double _x1, double _y1, double _x2, double _y2);
        void Detect(double mouse_x, double mouse_y, ALLEGRO_EVENT *e);
        void Detect();
        bool mouse_on_it;
        bool just_clicked;
        bool just_uncliked;
        bool clicking;
    };
    
    double x, y, w, h, x2, y2;
    ALLEGRO_BITMAP* bmp;
    Scene* scene;
    std::list<Widget *> childrens;
    MouseDetector *md;
};

}

#endif // WIDGET_H
