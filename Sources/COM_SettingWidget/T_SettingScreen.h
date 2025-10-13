#ifndef T_SETTINGSCREEN_H
#define T_SETTINGSCREEN_H

#include <QWidget>
#include "../SystemKit/BaseScrollPage.h"
#include "../CTL_AsulComboBox/AsulComboBox.h"
class T_SettingScreen : public BaseScrollPage
{
    Q_OBJECT
public:
    explicit T_SettingScreen(QWidget *parent = nullptr);

signals:
};

#endif // T_SETTINGSCREEN_H
