#include "rGUI.h"

namespace rGUI //DropBox
{
    DropBox::DropBox(float x, float y, float width, float height, Theme *thm, DropBoxManager *dpm, DropBox_Item *dpi)
            : Widget(x,y,width, height, thm)
    {
        Set_new_DrobBoxItem(dpi);
        //wd_extented_input = true;
        db_dbm = dpm;

        if(db_dbm != nullptr)
        {
            db_dbm->dbm_dropboxes.push_back(this);
        }
    }

    DropBox::~DropBox()
    {
        if(db_item != nullptr)
        {
            delete db_item;
        }
        if(db_dbm != nullptr)
        {
            std::list<DropBox*>::iterator dbm_it;
            for(dbm_it = db_dbm->dbm_dropboxes.begin(); dbm_it!= db_dbm->dbm_dropboxes.end(); ++dbm_it)
            {
                if((*dbm_it) == this)
                {
                    db_dbm->dbm_dropboxes.erase(dbm_it);
                }
            }
        }
    }

    void DropBox::Print()
    {
        wd_PrintBegin();

        wd_Print_Background();
        wd_Print_Frame();

        if(db_item != nullptr && db_item->bmp != nullptr && db_dragging == false)
        {
            al_draw_scaled_bitmap(db_item->bmp, 0, 0, al_get_bitmap_width(db_item->bmp),al_get_bitmap_height(db_item->bmp),
                                  db_item->print_x + wd_theme.added_thickness/2,db_item->print_y+ wd_theme.added_thickness/2,
                                  db_item->print_w,db_item->print_h, 0);
        }

        if(wd_md->md_mouse_on_it == true || db_dragging == true)
        {
            wd_Print_AddedThickness();
        }

        wd_PrintEnd();
    }

    int DropBox::Specific_Input(ALLEGRO_EVENT& ev)
    {
        if(wd_md->Specific_Input(ev) == 1)
        {
            db_dragging = true;

            if(db_dbm != nullptr)
            {
                if(db_dbm->dbm_block_dps == false)
                {
                    db_dbm->dbm_dragging_DB = this;
                    if(db_item == nullptr)
                    {
                        db_dbm->dbm_offset_x = trans_mouse_x -wd_md->md_x1;
                        db_dbm->dbm_offset_y = trans_mouse_y -wd_md->md_y1;
                    }
                    else
                    {
                        db_dbm->dbm_offset_x = trans_mouse_x -wd_md->md_x1 - db_item->print_x;
                        db_dbm->dbm_offset_y = trans_mouse_y -wd_md->md_y1 - db_item->print_y;
                    }
                    db_dbm->dbm_block_dps = true;
                }
            }
        }
        return 9;
    }

    int DropBox::Input()
    {
        return Specific_Input(event);
    }

    void DropBox::Set_new_DrobBoxItem(DropBox_Item *dpi)
    {
        //dpi->Copy_this_DropBox_item(db_item);
        if(dpi == nullptr)
        {
            if(db_item != nullptr)
            {
                delete db_item;
            }
            db_item = nullptr;
            return;
        }

        if(db_item != nullptr)
        {
            delete db_item;
        }
        db_item = new DropBox_Item;
        db_item->bmp_str = dpi->bmp_str;
        db_item->load_bmp_fom_file = dpi->load_bmp_fom_file;
        if(dpi->bmp != nullptr)
        {
            db_item->bmp = al_clone_bitmap(dpi->bmp);
        }
        db_item->print_x = dpi->print_x;
        db_item->print_y = dpi->print_y;
        db_item->print_w = dpi->print_w;
        db_item->print_h= dpi->print_h;
        if(dpi->data != nullptr)
        {
            db_item->data = dpi->data;
        }

        if(db_item->load_bmp_fom_file == true)
        {
            if(db_item->bmp != nullptr)
            {
                al_destroy_bitmap(db_item->bmp);
            }

            db_item->bmp = al_load_bitmap(db_item->bmp_str.c_str());
            if(db_item == nullptr)
            {
                error_message("Failed to laod Image " + db_item->bmp_str);
            }
        }
    }
}
