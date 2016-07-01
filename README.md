# GUI library for Allegro 5

###### TO DO (for v1.0):
  - [ ] Test stability
  - [ ] Add "changed" and "dragging" callbacks to DropBox
  - [x] ~~Add drag and drop widget~~ *done*	
  - [x] ~~Fix ScrollBar and SlideBar input when in zoomed ScrollableArea~~ *done, maybe some corrections for SlideBar*
  - [x] ~~Fix that too small thickness cause frames not to be print correctly~~ *done (for scale >= 1)*
  - [x] ~~replace booleans in label(~~TextBox~~) with bitflags~~ *done*
  - [x] ~~make Label more universal (fewer constructors, heavy bitflags use), maybe rename it to TextBox~~ *done*
  - [x] ~~add option to make ScrollableArea(ScbA) zoomable~~ *done*
  - [x] ~~every widget should be drawn to its bitmap and then Widget::Print() will draw this btimap to display~~
        ~~(remove everyting related to bitmap_only)~~ *done*
  - [x] ~~add x_offset and y_offset as global variables~~ *done*
  - [x] ~~make scale_x nad scale_y global wariables as well (display scale related), remove them~~
        ~~from Widget::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley)~~ *done* 

###### Maybe later:
  - [ ] add SCBA alternative which will create bigger/smaller widgets when zoomed
  - [ ] normal text selection *right now only CTRA+A and CTRL+C, CTRL+V are supported, and only in InputField*
  - [ ] code cleanup, variables name correcting, variable and struct usage optimization 
  - [x] replace all Constructors booleans with bitflags
  - [ ] adding new widgets to ScbA via template
