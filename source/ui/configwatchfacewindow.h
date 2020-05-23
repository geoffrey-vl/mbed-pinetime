#ifndef CONFIGWATCHFACEWINDOW_H
#define CONFIGWATCHFACEWINDOW_H

#include "basewindow.h"

class ConfigWatchFaceWindow : public BaseWindow
{
public:
    ConfigWatchFaceWindow();
    virtual ~ConfigWatchFaceWindow();

    void onItemSelected();

    uint16_t selectedId() { return mSelectedItem; }

private:
    lv_obj_t * lstClocks;
    lv_obj_t * lblTitle;
    uint16_t mSelectedItem;
};

#endif // CONFIGWATCHFACEWINDOW_H