#ifndef SCENE_H
#define SCENE_H

#include <allegro5/allegro.h>
#include <list>

namespace rGUI
{

class Widget;    
    
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
     * @brief Call Render() for all Widgets in widgets list
     */
    virtual void Render();
    
    
    const double getTransMouseX();
    const double getTransMouseY();
    const ALLEGRO_EVENT* getCurrentEvent();
    
protected:
    double trans_mouse_x, trans_mouse_y;
    ALLEGRO_EVENT* current_event();
    
    /**
     * @brief so only one widget per scene is focused
     */
    virtual void focusRecalc();
    bool recal_focus = false;
};

}

#endif // SCENE_H
