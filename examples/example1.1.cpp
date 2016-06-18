#include "../rGUI.h"
#include <ctime>

#define TIME_B clock_t cccccc = clock();
#define TIME_E std::cout << (clock() - cccccc) << std::endl;

const float FPS = 60;
const int SCREEN_W = 900;
const int SCREEN_H = 700;
const int BOUNCER_SIZE = 32;

inline int error_message(std::string error_string)
{
    return al_show_native_message_box(al_get_current_display(), "ERROR", "", error_string.c_str(), nullptr, ALLEGRO_MESSAGEBOX_ERROR);
}

int main(int argc, char **argv)
{

    if(!al_init())
    {
        error_message("al_init()");
    }
    if(!al_init_primitives_addon())
    {
        error_message("al_init_primitives_addon()");
        return false;
    }
    if(!al_install_keyboard())
    {
        error_message("al_install_keyboard()");
        return false;
    }
    if(!al_install_mouse())
    {
        error_message("al_install_mouse()");
        return false;
    }
    if(!al_init_image_addon())
    {
        error_message("al_init_image_addon()");
        return false;
    }
    if(!al_install_audio())
    {
        error_message("al_install_audio()");
        return false;
    }
    if(!al_init_acodec_addon())
    {
        error_message("al_init_acodec_addon()");
        return false;
    }
    al_init_font_addon(); // returns void
    if(!al_init_ttf_addon())
    {
        error_message("al_init_ttf_addon()");
        return false;
    }

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *bouncer = NULL;
    float bouncer_x = SCREEN_W / 2.0 - BOUNCER_SIZE / 2.0;
    float bouncer_y = SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0;
    bool redraw = true;
    ALLEGRO_FONT *font = al_load_font("Calibri.ttf", 35, 0);

    timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        return -1;
    }

    bouncer = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);
    if(!bouncer)
    {
        fprintf(stderr, "failed to create bouncer bitmap!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    al_set_target_bitmap(bouncer);

    al_clear_to_color(al_map_rgb(255, 0, 255));

    al_set_target_bitmap(al_get_backbuffer(display));

    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_bitmap(bouncer);
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());


    al_clear_to_color(al_map_rgb(0,0,0));

    al_flip_display();

    al_start_timer(timer);

    rGUI::mouse_state = new ALLEGRO_MOUSE_STATE;
    rGUI::keyboard_state = new ALLEGRO_KEYBOARD_STATE;

    rGUI::Theme tmh;
    tmh.c_text = al_map_rgb(255,255,255);
    tmh.c_outline = al_map_rgb(255,0,0);
    tmh.c_background = al_map_rgb(0,0,150);
    tmh.c_clicking = al_map_rgba(0,0,0,125);
    tmh.added_thickness = 1;
    tmh.roundx = 0;
    tmh.roundy = 0;
    ALLEGRO_FONT *fnt = al_load_ttf_font("Calibri.ttf",30,0);

    std::vector<rGUI::Widget*>widgets;

    widgets.push_back(new rGUI::Button(50,10,100,45,"Standart long text", "Calibri.ttf",&tmh));
    widgets.push_back(new rGUI::CheckBox(50,70, 25, 25, &tmh, true));
    tmh.roundx = 12;
    tmh.roundy = 12;
    widgets[1]->Update_theme(&tmh);
    tmh.roundx = 0;
    tmh.roundy = 0;
    tmh.c_text = al_map_rgb(255,0,255);
    widgets.push_back(new rGUI::ClickableText(160,100, "Clicke me Sempai!", fnt, 0, &tmh, false));
    widgets.push_back(new rGUI::ClickableText(widgets[1]->wd_x2 + 5, widgets[1]->comentary_text_y - (widgets[1]->wd_height -5)/2,
                                              "Comentary text", "Calibri.ttf", 0, widgets[1]->wd_height -5, &tmh, false));
    int slidebar1poz = widgets.size();
    widgets.push_back(new rGUI::SlideBar(50,100, 100, 30, 0, 255, &tmh, false, false));
    ((rGUI::SlideBar*)widgets[widgets.size()-1])->wd_c_background = al_map_rgb(255,0,0);
    widgets.push_back(new rGUI::SlideBar(10,10, 30, 120, 0, 255, &tmh, true, false));
    ((rGUI::SlideBar*)widgets[widgets.size()-1])->wd_c_background = al_map_rgb(0,255,0);
    widgets.push_back(new rGUI::SlideBar(210,10, 30, 120, 0, 255, &tmh, true, false));
    ((rGUI::SlideBar*)widgets[widgets.size()-1])->wd_c_background = al_map_rgb(0,0,255);
    ((rGUI::SlideBar*)widgets[widgets.size()-1])->Set_value(125);
    widgets.push_back(new rGUI::BitmapButton(10, 140, "button.png", &tmh, false));
    widgets.push_back(new rGUI::BitmapButton(10, 190,100, 45, "button.png", &tmh, false));
    widgets.push_back(new rGUI::ScrollBar(10, 250, 300, 20, 1000,&tmh, false, false));
    ((rGUI::ScrollBar*)widgets[widgets.size()-1])->Set_change(7000);
    widgets.push_back(new rGUI::ScrollBar(290, 10, 20, 230, 1000,&tmh, true, false));
    ((rGUI::ScrollBar*)widgets[widgets.size()-1])->c_background_mult = 1.4f;
    tmh.c_background = al_map_rgba(40,40,40,150);
    tmh.roundx = 2;
    tmh.roundy = 2;
    widgets.push_back(new rGUI::InputField(10, 400, 100, 45, "GFSArtemisia.ttf", &tmh, FPS, false));
    widgets.push_back(new rGUI::InputField(10, 450, 100, 25, "GFSArtemisia.ttf", &tmh, FPS, false));
    widgets.push_back(new rGUI::SingleKeyInputField(130,400,200,45, 55, "Calibri.ttf", &tmh, false));

    widgets.push_back(new rGUI::ScrollableArea(170, 170, 50,50,300,300,&tmh, 10));

    int scbnapoz = widgets.size();
    widgets.push_back(new rGUI::ScrollableArea(320, 10,300,200,1000,1000,&tmh, 14));

    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->widgets.push_back(new rGUI::ClickableText(10,100,
                                                                "Utf-8 text test: Iñtërnâtiônàlizætiøn", fnt, 0, &tmh, false));
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->widgets.push_back(new rGUI::Button(50,
                                                                10,100,45,"TText", "Calibri.ttf",&tmh));
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->widgets.push_back(new rGUI::ClickableText(10,500,
                                                                "Big Test Text", "Calibri.ttf", 0, 100, &tmh, false));
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->widgets.push_back(new rGUI::CheckBox(10,
                                                                        150, 25, 25, &tmh, true));
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->widgets.push_back(new rGUI::BitmapButton(10,1500, "image.jpg", &tmh, false));
    //this disables Bitmapbutton clicking shadow -> it can be use as causual image
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->
        widgets[((rGUI::ScrollableArea*)widgets[widgets.size()-1])->
        widgets.size() - 1]->wd_md->md_active = false;

    for(int a = 0; a < 500;a++)
    {
        //((rGUI::ScrollableArea*)widgets[widgets.size()-1])->widgets.push_back(new rGUI::Button(10 + a*150, 2000 + a * 80, 120, 60, "Test #" + std::to_string(a), "Calibri.ttf", &tmh));
        ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->widgets.push_back(new rGUI::Button(10 , 2000, 120, 60, "Test #" + std::to_string(a), "Calibri.ttf", &tmh));
    }


    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->widgets.push_back(new rGUI::InputField(10,
                                                                        400, 100, 45, "Calibri.ttf", &tmh, FPS, false));
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->widgets.push_back(new rGUI::InputField(10,
                                                                        1930, 300, 60, "Calibri.ttf", &tmh, FPS, false));
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->widgets.push_back(new rGUI::SingleKeyInputField(130,
                                                                                    400,200,45, 55, "Calibri.ttf", &tmh, false));
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->widgets.push_back(new rGUI::SlideBar(600,
                                                                                             100, 100, 30, 1, 50, &tmh, false, false));
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->I_added_new_widgets();

    tmh.c_outline = al_map_rgba(0,0,0,0);
    tmh.c_background = al_map_rgba(0,0,0,0);
    tmh.c_text = al_map_rgb(255,215,0);
    widgets.push_back(new rGUI::Label(30, 280, 300, 40, "Test Label with no outline, nor background", "Calibri.ttf",
                                      1, &tmh, false));
    tmh.c_outline = al_map_rgb(0,0,0);
    widgets.push_back(new rGUI::Label(30, 330, 300, "Font label ypq", font, 0, &tmh, false));
    widgets.push_back(new rGUI::Label(340, 260, 300, 200, "Some long test text, propably something copyed form THE internet :D "
                                      , "Calibri.ttf", 35, ALLEGRO_ALIGN_CENTER, &tmh, false, true));
    widgets.push_back(new rGUI::Label(340, 260, 300, 200, "Some long test text, propably something copyed form THE internet :D ",
                                      font, ALLEGRO_ALIGN_CENTER, &tmh, false, true));



    tmh.c_text = al_map_rgb(0,0,255);
    tmh.thickness = 10;
    tmh.c_background = al_map_rgba(100,100,100,100);
    widgets.push_back(new rGUI::TextBox
            (10,500,500,150,
             //"0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40",
            "loooooooooooooooooong text small ttext lol ol pol klol loooooooooooooooooooooooooonger text",
            "Calibri.ttf", 30, &tmh, ( rGUI::rg_RESIZE_FRAME | rGUI::rg_MULTILINE)));




    tmh.c_background = al_map_rgb(150,22,86);
    widgets.push_back(new rGUI::Button(400,400,150,68,"Close", "Calibri.ttf",&tmh));
    int dialpoz = widgets.size();
    tmh.c_background = al_map_rgb(88,88,88);
    widgets.push_back(new rGUI::ScrollableArea(100, 100, SCREEN_W - 200,SCREEN_H - 200 ,1000,1000,&tmh, 14));
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->Set_vertical_sba_status(false);
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->Set_horizontal_sba_status(false);
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->widgets.push_back(new rGUI::Button(20, SCREEN_H - 200 - 60,
                                                            80, 32, "End", "Calibri.ttf", &tmh));
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->widgets.push_back(new rGUI::Button(SCREEN_W - 200 - 100,
                                            SCREEN_H - 200 - 60, 80, 32, "Cancel", "Calibri.ttf", &tmh));
     ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->widgets.push_back(new rGUI::ClickableText(15, 100,
                                                            "Do you rly want end the fun?", font, 0,&tmh, false));
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->print_active = false;
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->wd_md->md_active = false;
    ((rGUI::ScrollableArea*)widgets[widgets.size()-1])->I_added_new_widgets();

    /*widgets.push_back(new rGUI::Label(0,0,SCREEN_W, SCREEN_H, "Asdfghjklqwertzuiopmnbvbcxy", font, ALLEGRO_ALIGN_CENTER, &tmh, false));
    ((rGUI::Label*)widgets[widgets.size()-1])->wd_c_background = al_map_rgba(0,0,0,0);
    widgets.push_back(new rGUI::Label(0,0,SCREEN_W, SCREEN_H, "Asd fghjkl qwe rtz ui opm n bvbc xy", "Calibri.ttf", 50,ALLEGRO_ALIGN_CENTRE, &tmh, false, true));
    ((rGUI::Label*)widgets[widgets.size()-1])->wd_c_background = al_map_rgba(0,0,0,0);
    */


    float scale = 1.0f;
    while(1)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        al_get_mouse_state(rGUI::mouse_state);
        al_get_keyboard_state(rGUI::keyboard_state);

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
        else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES ||
                ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
        {

            bouncer_x = ev.mouse.x;
            bouncer_y = ev.mouse.y;
        }

        float aaa = 1;

        for(int a = 0;a < (int)widgets.size();a++)
        {
            //TIME_B
            widgets[a]->Input(ev, aaa, aaa);
            /*if(widgets[a]->wd_type == rGUI::wt_SCROLLABLEAREA)
                TIME_E*/
        }

        if(widgets[dialpoz-1]->wd_md->md_clicked == true)
        {
            widgets[dialpoz-1]->wd_md->md_clicked = false;
            for(int a = 0;a < (int)widgets.size();a++)
            {
                widgets[a]->wd_md->md_active = false;
            }
            widgets[dialpoz]->wd_md->md_active = true;
            widgets[dialpoz]->print_active = true;
        }
        else if(((rGUI::ScrollableArea*)widgets[dialpoz])->widgets[0]->wd_md->md_clicked == true)
        {
            break;
        }
        else if(((rGUI::ScrollableArea*)widgets[dialpoz])->widgets[1]->wd_md->md_clicked == true)
        {
            ((rGUI::ScrollableArea*)widgets[dialpoz])->widgets[1]->wd_md->md_clicked = false;
            for(int a = 0;a < (int)widgets.size();a++)
            {
                widgets[a]->wd_md->md_active = true;
            }
            widgets[dialpoz]->wd_md->md_active = false;
            widgets[dialpoz]->print_active = false;
        }
        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(255,255,255));

            al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);

            ((rGUI::ClickableText*)widgets[2])->text = std::to_string(((rGUI::SlideBar*)widgets[4])->value);
            widgets[scbnapoz]->wd_c_background = al_map_rgb( ((rGUI::SlideBar*)widgets[slidebar1poz])->value ,
                                                             ((rGUI::SlideBar*)widgets[slidebar1poz+1])-> value,
                                                             ((rGUI::SlideBar*)widgets[slidebar1poz+2])-> value);
            for(int a = 0;a < (int)widgets.size();a++)
            {
                //TIME_B
                widgets[a]->Print();
                /*if(widgets[a]->wd_type == rGUI::wt_SCROLLABLEAREA)
                    TIME_E*/
            }

            al_flip_display();
        }
    }

    for(int a = 0;a < (int)widgets.size();a++)
    {
        delete widgets[a];
    }
    widgets.clear();

    delete rGUI::keyboard_state;
    delete rGUI::mouse_state;

    al_destroy_bitmap(bouncer);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
