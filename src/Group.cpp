#include "rGUI.h"

namespace rGUI //Group
{
    Theme ___thm__;

    Group::Group() : Widget(0,0,0,0, &___thm__)
    {

    }

    Group::Group(float x, float y, float width, float height) : Widget(x,y,width,height, &___thm__)
    {

    }

    Group::~Group()
    {
        g_widgets.clear();
    }

    void Group::Print()
    {
        return;
    }

    int Group::Specific_Input(ALLEGRO_EVENT& ev)
    {
        int ret = wd_md->Specific_Input(ev);

        if(g_AIoimiiGa == true)
        {
            if(wd_md->md_mouse_on_it == true)
            {
                Set_all_Inputing(true);
            }
            else
            {
                Set_all_Inputing(false);
            }
        }

        return ret;
    }

    int Group::Input()
    {
        return Specific_Input(event);
    }

    void Group::Set_all_Printing(bool TorF)
    {
        g_print = TorF;
        std::list<Widget*>::iterator wd_it;
        for(wd_it = g_widgets.begin(); wd_it != g_widgets.end(); ++wd_it)
        {
            (*wd_it)->wd_print_active = TorF;
        }
    }

    void Group::Set_all_Inputing(bool TorF)
    {
        g_input = TorF;
        std::list<Widget*>::iterator wd_it;
        for(wd_it = g_widgets.begin(); wd_it != g_widgets.end(); ++wd_it)
        {
            if((*wd_it)->wd_md->group_ovveridable == true)
            {
                (*wd_it)->wd_md->md_active = TorF;
            }
        }
    }

    void Group::Set_all_Clicked(bool TorF)
    {
        std::list<Widget*>::iterator wd_it;
        for(wd_it = g_widgets.begin(); wd_it != g_widgets.end(); ++wd_it)
        {
            if((*wd_it)->wd_md->group_ovveridable == true)
            {
                (*wd_it)->wd_md->md_clicked = TorF;
            }
        }
    }

    void Group::Set_all_Clicking(bool TorF)
    {
        std::list<Widget*>::iterator wd_it;
        for(wd_it = g_widgets.begin(); wd_it != g_widgets.end(); ++wd_it)
        {
            if((*wd_it)->wd_md->group_ovveridable == true)
            {
                (*wd_it)->wd_md->md_clicking = TorF;
            }
        }
    }

    void Group::Accept_Input_only_if_mouse_is_in_Group_area(bool TorF)
    {
        g_AIoimiiGa = TorF;
    }

    void Group::Add_Widget(Widget *wd)
    {
        g_widgets.push_back(wd);
    }

    void Group::Add_Widgets(std::vector<Widget*> &vec, int spoz, int epoz)
    {
        for(int a = spoz; a <= epoz;a++)
        {
            g_widgets.push_back(vec[a]);
        }
    }

    void Group::Add_Widgets(std::list<Widget*> &vec, std::list<Widget*>::iterator spoz, std::list<Widget*>::iterator epoz)
    {
        std::list<Widget*>::iterator it;
        for(it = spoz; it != epoz; ++it)
        {
            g_widgets.push_back( (*it));
        }
        g_widgets.push_back( (*epoz));
    }

    bool Group::Remove_Widget(Widget *wd)
    {
        std::list<Widget*>::iterator wd_it;
        for(wd_it = g_widgets.begin(); wd_it != g_widgets.end(); ++wd_it)
        {
            if( (*wd_it) == wd)
            {
                g_widgets.erase(wd_it);
                return true;
            }
        }
        return false;
    }


}
