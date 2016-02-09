#include "../rGUI.h"

const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
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

    ALLEGRO_MOUSE_STATE ms;
    rGUI::mouse_state = &ms;
    rGUI::Theme tmh;
    tmh.c_text = al_map_rgb(255,255,255);
    tmh.c_outline = al_map_rgb(255,0,0);
    tmh.c_background = al_map_rgb(0,0,150);
    tmh.c_clicking = al_map_rgba(0,0,0,125);
    tmh.added_thickness = 1;
    tmh.roundx = 0;
    tmh.roundy = 0;
    ALLEGRO_FONT *fnt = al_load_ttf_font("Calibri.ttf",30,0);

    rGUI::Button *bt = new rGUI::Button(10,10,100,45,"TText", "Calibri.ttf",&tmh);
    rGUI::CheckBox *chb = new rGUI::CheckBox(10,70, 25, 25, &tmh, true);
    tmh.roundx = 12;
    tmh.roundy = 12;
    chb->Update_theme(&tmh);
     tmh.roundx = 0;
    tmh.roundy = 0;
    rGUI::ClickableText *clckbt = new rGUI::ClickableText(120,100, "Clicke me Sempai!", fnt, 0, &tmh, false);
    clckbt->wd_c_text = al_map_rgb(88,88,88);
    rGUI::ClickableText *clcc = new rGUI::ClickableText(chb->wd_x2 + 5, chb->comentary_text_y - (chb->wd_height -5)/2, "Comentary text",
                                                        "Calibri.ttf", 0, chb->wd_height -5, &tmh, false);
    tmh.c_text = al_map_rgb(0,0,180);
    clcc->Update_theme(&tmh);

    rGUI::SlideBar *sba = new rGUI::SlideBar(10,100, 100, 30, 1, 50, &tmh, false, false);
    sba->Update_theme(&tmh);
    int aaaaaaa = 0;

    float scale = 1;
    while(1)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        al_get_mouse_state(&ms);

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
        bt->Input(ev, aaa, aaa);
        chb->Input(ev, aaa, aaa);
        clckbt->Input(ev, aaa, aaa);
        clcc->Input(ev, aaa, aaa);
        sba->Input(ev, aaa, aaa);

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(255,255,255));

            al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);
            bt->Print();
            chb->Print();
            clckbt->text = std::to_string(sba->value);
            clckbt->Print();
            clcc->Print();
            sba->Print();
            al_flip_display();
        }
    }

    delete bt;
    delete chb;
    delete clckbt;
    delete clcc;
    delete sba;

    al_destroy_bitmap(bouncer);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
