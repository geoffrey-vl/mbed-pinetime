#ifndef CONFIGURATIONWINDOW_H
#define CONFIGURATIONWINDOW_H

#include "basewindow.h"

class ConfigurationWindow : public BaseWindow
{
public:
    ConfigurationWindow();
    virtual ~ConfigurationWindow();

    void onItemSelected();

    uint16_t selectedId() { return mSelectedItem; }

private:
    lv_obj_t * lstClocks;
    lv_obj_t * lblTitle;
    uint16_t mSelectedItem;
};

#endif // CONFIGURATIONWINDOW_H