#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basewindow.h"

class MainWindow : public BaseWindow
{
public:
    MainWindow(LVGLDisplayDriver& driver);
    virtual ~MainWindow();

    void draw() override;

private:
    lv_obj_t * icBatt;
    lv_obj_t * lblTime;
    lv_obj_t * lblDay;
    time_t currentTime;

    void setTxtTime();
    void setTxtDate();
};

#endif // MAINWINDOW_H