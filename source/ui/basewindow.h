#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <LittlevGL.h>

class BaseWindow
{
public:
    BaseWindow();
    virtual ~BaseWindow();

    virtual void draw();

    lv_coord_t width() { return 240; }
    lv_coord_t height() { return 240; }

    bool isFinished() { return mIsFinished; }
    void signalFinished() { mIsFinished = true; }

protected:
    LittlevGL& mLvgl;

private:
    bool mIsFinished;
};

#endif // BASEWINDOW_H