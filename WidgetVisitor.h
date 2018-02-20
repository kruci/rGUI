#ifndef WIDGETVISITOR_H
#define WIDGETVISITOR_H

namespace rGUI
{

class Widget;    
    
class WidgetVisitor
{
public:
    virtual ~WidgetVisitor()=0;
    
    virtual void visit(Widget *w);
};

}

#endif // WIDGETVISITOR_H
