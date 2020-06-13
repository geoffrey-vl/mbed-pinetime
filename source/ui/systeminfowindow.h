#ifndef SYSTEMINFOWINDOW
#define SYSTEMINFOWINDOW

#include "basewindow.h"
#include <string>

class SystemInfoWindow : public BaseWindow
{
public:
    SystemInfoWindow();
    virtual ~SystemInfoWindow();

private:
    lv_obj_t * pgInfo;
    lv_obj_t * lblTitle;
    std::string txt;
};


#endif //SYSTEMINFOWINDOW