#include "rGUI.h"

namespace rGUI //DropBoxManager
{
    Theme __thm__;
    DropBoxManager::DropBoxManager() : Widget(0,0,0,0, &__thm__)
    {

    }

    DropBoxManager::~DropBoxManager()
    {
        dbm_dropboxes.clear();

        if(temp_dpi != nullptr)
            delete temp_dpi;
    }

    void DropBoxManager::Print()
    {
        if(dbm_block_dps == true && dbm_dragging_DB->db_item != nullptr && dbm_dragging_DB->db_item->bmp != nullptr)
        {
            al_draw_scaled_bitmap(dbm_dragging_DB->db_item->bmp, 0, 0, al_get_bitmap_width(dbm_dragging_DB->db_item->bmp),
                                  al_get_bitmap_height(dbm_dragging_DB->db_item->bmp),
                                  trans_mouse_x - dbm_offset_x,trans_mouse_y - dbm_offset_y,
                                  dbm_dragging_DB->db_item->print_w, dbm_dragging_DB->db_item->print_h, 0);
        }
    }

    int DropBoxManager::Specific_Input(ALLEGRO_EVENT& ev)
    {
        if(dbm_block_dps == true && ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && ev.mouse.button == dbm_dragging_DB->wd_md->md_mouse_button)
        {
            dbm_w8 = true;
        }
        else if(dbm_w8 == true)
        {
            dbm_block_dps = false;
            std::list<DropBox*>::iterator dbm_it;
            for(dbm_it = dbm_dropboxes.begin(); dbm_it!=dbm_dropboxes.end(); ++dbm_it)
            {
                if((*dbm_it)->wd_md->md_mouse_on_it == true && (*dbm_it) != dbm_dragging_DB)
                {
                    Set_new_DrobBoxItem((*dbm_it)->db_item);

                    (*dbm_it)->Set_new_DrobBoxItem(dbm_dragging_DB->db_item);
                    dbm_dragging_DB->Set_new_DrobBoxItem(temp_dpi);
                    (*dbm_it)->changed = true;
                    dbm_dragging_DB->changed = true;
                }
            }
            dbm_dragging_DB->db_dragging = false;
            dbm_w8 = false;
        }
    }

    int DropBoxManager::Input()
    {
        Specific_Input(event);
    }

    void DropBoxManager::Set_new_DrobBoxItem(DropBox_Item *dpi)
    {
        if(dpi == nullptr)
        {
            if(temp_dpi != nullptr)
            {
                delete temp_dpi;
            }
            temp_dpi = nullptr;
            return;
        }

        if(temp_dpi != nullptr)
        {
            delete temp_dpi;
        }
        temp_dpi = new DropBox_Item;
        temp_dpi->bmp_str = dpi->bmp_str;
        temp_dpi->load_bmp_fom_file = dpi->load_bmp_fom_file;
        if(dpi->bmp != nullptr)
        {
            temp_dpi->bmp = al_clone_bitmap(dpi->bmp);
        }
        temp_dpi->print_x = dpi->print_x;
        temp_dpi->print_y = dpi->print_y;
        temp_dpi->print_w = dpi->print_w;
        temp_dpi->print_h= dpi->print_h;
        if(dpi->data != nullptr)
        {
            temp_dpi->data = dpi->data;
        }

        if(temp_dpi->load_bmp_fom_file == true)
        {
            if(temp_dpi->bmp != nullptr)
            {
                al_destroy_bitmap(temp_dpi->bmp);
            }

            temp_dpi->bmp = al_load_bitmap(temp_dpi->bmp_str.c_str());
            if(temp_dpi == nullptr)
            {
                error_message("Failed to laod Image " + temp_dpi->bmp_str);
            }
        }
    }

}

