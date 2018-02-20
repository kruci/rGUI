#ifndef SCENE_H
#define SCENE_H

#include <allegro5/allegro.h>
#include <list>

namespace rGUI
{

class Widget;
class WidgetVisitor;    
    
/**
 * @class Scene
 * @author 
 * @date 02/20/18
 * @file Scene.h
 * @brief Holds and manages Widgets
 */
class Scene
{
friend class Widget;
public:
    std::list<Widget*> widgets;
    
    Scene();
    /**
     * @brief Will destroy widgets from widgets list
     * @param _w
     */
    virtual ~Scene();
    
    /**
     * @brief add Widget to this scene
     * @param _w
     */
    virtual void addWidget(Widget* _w);
    
    /**
     * @brief Remove Widget from scene
     * @param _w
     */
    virtual void removeWidget(Widget* _w);
    
    /**
     * @brief Call Detect() for all Widgets in widgets list, will transform stored mouse coords, should be called
     * when on backbuffer(if tihs scene is first in chain), so al_transform_coordinates(...) works correctly
     * @param _e
     */
    virtual void Detect(ALLEGRO_EVENT * _e);
    
    /**
     * @brief Call Render() for all Widgets in widgets list (draws to current bitmap)
     */
    virtual void Render();
    
    
    double getTransMouseX() const;
    double getTransMouseY() const;
    const ALLEGRO_EVENT* getCurrentEvent() const;
    
    /**
     * @brief If events have display source, scene will run Detect() only for events with source display _d
     * @param _d
     */
    virtual void hookSceneToDispaly(ALLEGRO_DISPLAY *_d);
    
    /**
     * @brief 
     */
    virtual void hookSceneToDispaly();
    
    /**
     * @brief Will call accept(_v) for all childrens
     * @param _v
     */
    virtual void accept(WidgetVisitor* _v);
    
protected:
    
    /**
     * @brief so only one widget per scene is focused
     */
    virtual void focusRecalc();

    double trans_mouse_x, trans_mouse_y;
    ALLEGRO_EVENT* current_event();
    bool recal_focus = false;
    ALLEGRO_DISPLAY *display = nullptr;
};

}

#endif // SCENE_H
