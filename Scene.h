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
     * @brief Call Detect() for all Widgets in widgets list, will transform stored mouse coords
     * @param _e
     */
    virtual void Detect(ALLEGRO_EVENT * _e);
    
    /**
     * @brief Call Render() for all Widgets in widgets list
     */
    virtual void Render();
    
private:
    double trans_mouse_x, trans_mouse_y;
};

}

#endif // SCENE_H
