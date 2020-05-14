#include "basewindow.h"

BaseWindow::BaseWindow() :
    mLvgl(LittlevGL::get_instance()),
    mIsFinished(false)
{
}

BaseWindow::~BaseWindow()
{
}

void BaseWindow::draw()
{
    mLvgl.update();
}