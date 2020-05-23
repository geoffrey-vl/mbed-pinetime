#ifndef CONFIGURATIONWINDOW_H
#define CONFIGURATIONWINDOW_H

#include "basewindow.h"

class ConfigurationWindow : public BaseWindow
{
public:
    ConfigurationWindow();
    virtual ~ConfigurationWindow();

    void onItemSelected(int32_t selectedItem);

    uint16_t selectedId() { return mSelectedItem; }

private:
    lv_obj_t * lstSubmenu;
    lv_obj_t * lblTitle;
    int32_t mSelectedItem;
};

#endif // CONFIGURATIONWINDOW_H