#ifndef T_HOME_H
#define T_HOME_H

#include <QWidget>
#include "ElaWidget.h"
#include "../SystemKit/BaseScrollPage.h"
class T_Home : public BaseScrollPage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Home(QWidget *parent = nullptr);
signals:
    void showSettingPage();
    void showLoginPage();
};



#endif // T_HOME_H
