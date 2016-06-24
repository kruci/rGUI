# GUI library for Allegro 5

###### TO DO (v1.0):
  - ~~replace booleans in label(~~TextBox~~) with bitflags~~ *done*
  - ~~make Label more universal (fewer constructors, heavy bitflags use), maybe rename it to TextBox~~ *done*
  - ~~add option to make ScrollableArea(ScbA) zoomable~~ *done*
  - every widget should be drawn to its bitmap and then Widget::Print() will draw this btimap to display
  	(remove everyting related to bitmap_only)
  - add x_offset and y_offset as global variables
  - make scale_x nad scale_y global wariables as well (display scale related), remove them 
    from Widget::Input(ALLEGRO_EVENT &ev, float &scalex, float &scaley) 
  - normal text selection *right now only CTRA+A and CTRL+C, CTRL+V are supported, and only in InputField*
  - code cleanup, variables name correcting, variable and struct usage optimization 
  - (?) replace all booleans with bitflags
  - (?) adding new widgets to ScbA via template
  - (?) (examples) have you heard about static_cast ? :D 