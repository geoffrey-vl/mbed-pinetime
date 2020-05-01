#include "basewindow.h"

BaseWindow::BaseWindow() :
    lvgl(LittlevGL::get_instance())
{
}

BaseWindow::~BaseWindow()
{
}

void BaseWindow::draw()
{
    lvgl.update();
}