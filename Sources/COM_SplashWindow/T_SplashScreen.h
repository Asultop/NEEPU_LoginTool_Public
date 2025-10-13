#ifndef T_SplashScreen_H
#define T_SplashScreen_H

#include <QObject>
#include <QWidget>
#include "../SystemKit/BaseInclude.h"
#include "../SystemKit/BaseScrollpage.h"
#include <QTextBrowser>
#include <ElaWindow.h>

class T_SplashScreen : public ElaWindow
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_SplashScreen(QWidget *parent = nullptr,const QString productName="%Name%",const QString productImg=QString());
    ~T_SplashScreen();

signals:
};

#endif // T_SplashScreen_H
