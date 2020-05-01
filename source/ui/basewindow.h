#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <LittlevGL.h>

class BaseWindow
{
public:
    BaseWindow();
    virtual ~BaseWindow();

    virtual void draw();

protected:
    LittlevGL& lvgl;
};

#endif // BASEWINDOW_H