#include "../rGUI.h"
#include <ctime>

#define TIME_B clock_t cccccc = clock();
#define TIME_E std::cout << (clock() - cccccc) << std::endl;

const float FPS = 60;
const int SCREEN_W = 900;
const int SCREEN_H = 700;
const int BOUNCER_SIZE = 32;

std::vector<rGUI::Widget*>widgets;
rGUI::Theme tmh;

void thmreset()
{
    tmh.c_text = al_map_rgba(255,255,255,0);
    tmh.c_outline = al_map_rgba(0,200,0,255);
    tmh.c_background = al_map_rgba(255,0,0,255);
    tmh.c_clicking = al_map_rgba(0,0,0,125);
    tmh.thickness = 2;
    tmh.added_thickness = 2;
    tmh.roundx = 2;
    tmh.roundy = 2;
}

int main(int argc, char **argv)
{
    int newsw = 771 , newsh = 600;
    float _sx = (float)newsw / (float)SCREEN_W, _sy = (float)newsh / (float)SCREEN_H;
    float _offx = (SCREEN_W - newsw)/2, _offy = (SCREEN_H - newsh)/2;

    rGUI::Init_Allegro();
    rGUI::Init_rGUI();
    //rGUI::Init_rGUI(_sx, _sy, _offx, _offy);

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
    //display = al_create_display(newsw, newsh);

    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
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

    /*tmh.c_outline = al_map_rgb(255,255,255),
    tmh.c_background = al_map_rgba(0,0,66,100),
    tmh.c_text = al_map_rgb(255,255,255),
    tmh.c_clicking = al_map_rgba(0,0,0,150);*/

    tmh.c_text = al_map_rgba(255,255,255,0);
    tmh.c_outline = al_map_rgba(0,200,0,255);
    tmh.c_background = al_map_rgba(255,0,0,255);
    tmh.c_clicking = al_map_rgba(0,0,0,125);
    tmh.thickness = 2;
    tmh.added_thickness = 2;
    tmh.roundx = 0;
    tmh.roundy = 0;
    ALLEGRO_FONT *fnt = al_load_ttf_font("Calibri.ttf",30,0);



    widgets.push_back(new rGUI::Button(10,40,100,50,"wwwLong text", "Calibri.ttf",&tmh));
    tmh.roundx = 3;
    tmh.roundy = 3;
    widgets.push_back(new rGUI::CheckBox(160, 40, 50,50,&tmh, false));
    widgets.push_back(new rGUI::SlideBar(10,100,100,30,0,300,&tmh, (rGUI::bf_HORIZONTAL | rGUI::bf_HAS_FRAME) ));
    widgets.push_back(new rGUI::SlideBar(115,40,30,50,0,300,&tmh, (rGUI::bf_VERTICAL | rGUI::bf_HAS_FRAME) ));
    widgets.push_back(new rGUI::BitmapButton(10,140,100,45, "button.png",&tmh, (rGUI::bf_HAS_FRAME)));
    widgets.push_back(new rGUI::ScrollBar(220, 5, 30, 185, 800, &tmh, (rGUI::bf_VERTICAL)));
    widgets.push_back(new rGUI::ScrollBar(10, 195, 240, 30, 800, &tmh, (rGUI::bf_HORIZONTAL)));
    widgets.push_back(new rGUI::InputField(115,100,95,40, "Calibri.ttf",&tmh, FPS));
    widgets.push_back(new rGUI::SingleKeyInputField(115,145,95,40, ALLEGRO_KEY_BACKQUOTE, "Calibri.ttf",&tmh));
    int progressbar = widgets.size();
    widgets.push_back(new rGUI::ProgressBar(10,5,200,30,0,&tmh));



    widgets.push_back(new rGUI::ScrollableArea(10,240,220,200,2000,2000,&tmh,20, (rGUI::bf_VERTICAL | rGUI::bf_HORIZONTAL | rGUI::bf_ZOOMABLE)));
    int scbapoz = widgets.size()-1;
    widgets[scbapoz]->widgets.push_back(new rGUI::Button(10,10,100,30, "Text", "Calibri.ttf", &tmh));
    widgets[scbapoz]->widgets.push_back(new rGUI::BitmapButton(10,50,100,45, "button.png",&tmh, (rGUI::bf_HAS_FRAME)));
    widgets[scbapoz]->widgets.push_back(new rGUI::CheckBox(120, 30, 50,50,&tmh, false));
    widgets[scbapoz]->widgets.push_back(new rGUI::SlideBar(10,100,120,30,0,300,&tmh, (rGUI::bf_HORIZONTAL | rGUI::bf_HAS_FRAME) ));
    widgets[scbapoz]->widgets.push_back(new rGUI::ScrollBar(10, 160, 120, 30, 800, &tmh, (rGUI::bf_HORIZONTAL)));
    widgets[scbapoz]->widgets.push_back(new rGUI::InputField(10,2000,100,40, "Calibri.ttf",&tmh, FPS));
    ((rGUI::ScrollableArea*)widgets[scbapoz])->I_added_new_widgets();

    //roundXY
    widgets.push_back(new rGUI::TextBox(680,5,300,40, "RoundXY",  "Calibri.ttf",15, &tmh, (rGUI::bf_HAS_FRAME)));
    int poz_roundXY = widgets.size();
    widgets.push_back(new rGUI::SlideBar(800,10,100,30,0,10,&tmh, (rGUI::bf_HORIZONTAL)));
    //Thickness
    widgets.push_back(new rGUI::TextBox(680,50,300,40, "Thickness",  "Calibri.ttf",15, &tmh, (rGUI::bf_HAS_FRAME)));
    int poz_thickness = widgets.size();
    widgets.push_back(new rGUI::SlideBar(800,55,100,30,0,10,&tmh, (rGUI::bf_HORIZONTAL)));

    //Added thickness
    widgets.push_back(new rGUI::TextBox(680,95,300,40, "Added Thickness",  "Calibri.ttf",10, &tmh, (rGUI::bf_HAS_FRAME)));
    int poz_addedthickness = widgets.size();
    widgets.push_back(new rGUI::SlideBar(800,100,100,30,0,10,&tmh, (rGUI::bf_HORIZONTAL)));

    //Update Button
    int poz_updatebutton = widgets.size();
    widgets.push_back(new rGUI::Button(680,140,180,40,"Update Theme","Calibri.ttf", &tmh));

    //Reset Button
    int poz_rb = widgets.size();
    widgets.push_back(new rGUI::TextBox(862,140,36,40,"Reset theme","Calibri.ttf", 12, &tmh,
                                        (rGUI::bf_AS_BUTTON | rGUI::bf_HAS_FRAME | rGUI::bf_MULTILINE)));
    //widgets.push_back(new rGUI::Button(865,140,30,40,"Reset colors","Calibri.ttf", &tmh));

    //Color selector
    int poz_colscba = widgets.size();
    widgets.push_back(new rGUI::ScrollableArea(400,5,275,180,2000,2000,&tmh,20,0));
        //red
        int poz_colsba_R =  widgets[poz_colscba]->widgets.size();
        widgets[poz_colscba]->widgets.push_back(new rGUI::SlideBar(5,5,30,140,0,255,&tmh, rGUI::bf_VERTICAL));
        //Green
        int poz_colsba_G =  widgets[poz_colscba]->widgets.size();
        widgets[poz_colscba]->widgets.push_back(new rGUI::SlideBar(40,5,30,140,0,255,&tmh, rGUI::bf_VERTICAL));
        //Blue
        int poz_colsba_B =  widgets[poz_colscba]->widgets.size();
        widgets[poz_colscba]->widgets.push_back(new rGUI::SlideBar(75,5,30,140,0,255,&tmh, rGUI::bf_VERTICAL));
        //Alpha
        int poz_colsba_A =  widgets[poz_colscba]->widgets.size();
        widgets[poz_colscba]->widgets.push_back(new rGUI::SlideBar(110,5,30,140,0,255,&tmh, rGUI::bf_VERTICAL));
        //cols
        int poz_colsba_cols =  widgets[poz_colscba]->widgets.size();
        widgets[poz_colscba]->widgets.push_back(new rGUI::TextBox(15,150,120,20, "R    G     B     A", "Calibri.ttf", 20, &tmh, rGUI::bf_HAS_FRAME));

        //TextC
        int poz_colsba_c_T =  widgets[poz_colscba]->widgets.size();
        widgets[poz_colscba]->widgets.push_back(new rGUI::Button(150,5,120,30, "Text color", "Calibri.ttf", &tmh));
        //BackgroundC
        int poz_colsba_c_B =  widgets[poz_colscba]->widgets.size();
        widgets[poz_colscba]->widgets.push_back(new rGUI::Button(150,40,120,30, "Background color", "Calibri.ttf", &tmh));
        //OutlineC
        int poz_colsba_c_O =  widgets[poz_colscba]->widgets.size();
        widgets[poz_colscba]->widgets.push_back(new rGUI::Button(150,75,120,30, "Outline color", "Calibri.ttf", &tmh));
        //ClickingC
        int poz_colsba_c_C =  widgets[poz_colscba]->widgets.size();
        widgets[poz_colscba]->widgets.push_back(new rGUI::Button(150,110,120,30, "Clicking color", "Calibri.ttf", &tmh));

    ((rGUI::ScrollableArea*)widgets[poz_colscba])->I_added_new_widgets();


    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_scale_transform(&trans, _sx, _sy);
    al_translate_transform(&trans, _offx, _offy);
    //al_use_transform(&trans);
    float progresval = 0, progressadd = 0.1;

    float scale = 1.0f;
    while(1)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        rGUI::GetStatesAdnEvents(ev);

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
            progresval += progressadd;
            ((rGUI::ProgressBar*)widgets[progressbar])->Set_value(progresval);
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


        for(int a = 0;a < (int)widgets.size();a++)
        {
            widgets[a]->Input();
        }

        if(widgets[poz_updatebutton]->wd_md->md_clicked == true || widgets[poz_rb]->wd_md->md_clicked == true)
        {
            widgets[poz_updatebutton]->wd_md->md_clicked = false;
            tmh.thickness = ((rGUI::SlideBar*)widgets[poz_thickness])->value;
            tmh.added_thickness = ((rGUI::SlideBar*)widgets[poz_addedthickness])->value;
            tmh.roundx = tmh.roundy = ((rGUI::SlideBar*)widgets[poz_roundXY])->value;

            if(widgets[poz_rb]->wd_md->md_clicked == true)
            {
                widgets[poz_rb]->wd_md->md_clicked = false;
                thmreset();
            }

            for(int a = 0;a < (int)widgets.size();a++)
            {
                widgets[a]->Update_theme(&tmh);
            }
            widgets[poz_colscba]->widgets[poz_colsba_cols]->wd_theme.c_background =
                al_premul_rgba(((rGUI::SlideBar*)widgets[poz_colscba]->widgets[poz_colsba_R])->Get_value(),
                            ((rGUI::SlideBar*)widgets[poz_colscba]->widgets[poz_colsba_G])->Get_value(),
                            ((rGUI::SlideBar*)widgets[poz_colscba]->widgets[poz_colsba_B])->Get_value(),
                            ((rGUI::SlideBar*)widgets[poz_colscba]->widgets[poz_colsba_A])->Get_value());
        }
        else if(widgets[poz_colscba]->widgets[poz_colsba_R]->wd_md->md_mouse_on_it == true ||
                widgets[poz_colscba]->widgets[poz_colsba_G]->wd_md->md_mouse_on_it == true ||
                widgets[poz_colscba]->widgets[poz_colsba_B]->wd_md->md_mouse_on_it == true ||
                widgets[poz_colscba]->widgets[poz_colsba_A]->wd_md->md_mouse_on_it == true)
        {
            widgets[poz_colscba]->widgets[poz_colsba_cols]->wd_theme.c_background =
                al_premul_rgba(((rGUI::SlideBar*)widgets[poz_colscba]->widgets[poz_colsba_R])->Get_value(),
                            ((rGUI::SlideBar*)widgets[poz_colscba]->widgets[poz_colsba_G])->Get_value(),
                            ((rGUI::SlideBar*)widgets[poz_colscba]->widgets[poz_colsba_B])->Get_value(),
                            ((rGUI::SlideBar*)widgets[poz_colscba]->widgets[poz_colsba_A])->Get_value());
        }
        else if(widgets[poz_colscba]->widgets[poz_colsba_c_T]->wd_md->md_clicked == true)
        {
            tmh.c_text = widgets[poz_colscba]->widgets[poz_colsba_cols]->wd_theme.c_background;
            widgets[poz_colscba]->widgets[poz_colsba_c_T]->wd_md->md_clicked = false;
        }
        else if(widgets[poz_colscba]->widgets[poz_colsba_c_O]->wd_md->md_clicked == true)
        {
            tmh.c_outline = widgets[poz_colscba]->widgets[poz_colsba_cols]->wd_theme.c_background;
            widgets[poz_colscba]->widgets[poz_colsba_c_O]->wd_md->md_clicked = false;
        }
        else if(widgets[poz_colscba]->widgets[poz_colsba_c_B]->wd_md->md_clicked == true)
        {
            tmh.c_background = widgets[poz_colscba]->widgets[poz_colsba_cols]->wd_theme.c_background;
            widgets[poz_colscba]->widgets[poz_colsba_c_B]->wd_md->md_clicked = false;
        }
        else if(widgets[poz_colscba]->widgets[poz_colsba_c_C]->wd_md->md_clicked == true)
        {
            tmh.c_clicking = widgets[poz_colscba]->widgets[poz_colsba_cols]->wd_theme.c_background;
            widgets[poz_colscba]->widgets[poz_colsba_c_C]->wd_md->md_clicked = false;
        }

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(255,255,255));

            al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);

            for(int a = 0;a < (int)widgets.size();a++)
            {
                //TIME_B
                widgets[a]->Print();
                /*if(widgets[a]->wd_type == rGUI::wt_SCROLLABLEAREA)
                    TIME_E*/
            }
            //al_draw_rectangle(160,40,210,90, al_map_rgb(0,0,255),0);
            /*al_draw_rectangle(200,200,220,220, al_map_rgb(0,0,0),10);
            al_draw_rectangle(200,200,220,220, al_map_rgb(0,255,0),0);
            al_draw_rectangle(205,205,215,215, al_map_rgb(0,255,0),0);*/
            al_flip_display();
        }
    }

    for(int a = 0;a < (int)widgets.size();a++)
    {
        delete widgets[a];
    }
    widgets.clear();
    rGUI::End();

    al_destroy_bitmap(bouncer);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
