#ifndef GLOBALFUNC_H
#define GLOBALFUNC_H

#include "qcolor.h"
#include <QObject>
#include <ElaMessageBar.h>
#include <ElaScrollPageArea.h>
#include <ElaText.h>
#include <QHBoxLayout>
#include "singleton.h"
#include "stdafx.h"

#define gFunc GlobalFunc::getInstance()

class GlobalFunc :public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(GlobalFunc);

public:
    explicit GlobalFunc(QObject *parent = nullptr);
    static QColor getDarkerColor(const QColor& color, int factor = 90) ;

    static QColor getLighterColor(const QColor& color, int factor = 110) ;
    static void showSuccess(QString title, QString content, int mSec=-1);
    static void showErr(QString title, QString content, int mSec=-1);
    static void showInfo(QString title, QString content, int mSec=-1);
    static void showWarn(QString title, QString content, int mSec=-1);
    static void updateThemeUI();
    static void updateThemeUI(QColor customColor);

    static double getReadTime(QString context);
    static double getReadWidth(ElaText *context);
    static bool askDialog(QWidget *parent,QString title,QString content);
    static bool askDialog(QString title, QString content);
    static void confirmDialog(QWidget *parent,QString title, QString content);
    static void confirmDialog(QString title, QString content);

    static ElaScrollPageArea * GenerateArea(QWidget *parent,ElaText * title, QWidget * widget,bool convert=true);
    static ElaScrollPageArea * GenerateArea(QWidget *parent,ElaText * title, QHBoxLayout * widget,bool convert=true);
    static ElaScrollPageArea * GenerateArea(QWidget *parent,QWidget * title, QHBoxLayout * widget,bool convert=true);
    static ElaScrollPageArea * GenerateArea(QWidget *parent,QHBoxLayout * title, QHBoxLayout * widget,bool convert=true);
    static ElaScrollPageArea * GenerateArea(QWidget *parent, ElaText *title, ElaText *subtitle, QWidget *widget, bool convert=true);
    static ElaScrollPageArea * GenerateArea(QWidget *parent, QString iconPath, ElaText *title, ElaText *subtitle, QWidget *widget, bool convert=true);
    static ElaScrollPageArea * GenerateArea(QWidget *parent, ElaIconType::IconName iconType, ElaText *title, ElaText *subtitle, QWidget *widget, bool convert=true);
    static ElaScrollPageArea * GenerateArea(QWidget *parent, QString iconPath, ElaText *title, ElaText *subtitle, QHBoxLayout *widget, bool convert=true);
    static ElaScrollPageArea * GenerateArea(QWidget *parent, ElaIconType::IconName iconType, ElaText *title, ElaText *subtitle, QHBoxLayout *widget, bool convert=true);


    static void saveResourceFile(QString resProfiex, QString resFileName, QString destFullPathFileName);
    static std::pair<bool, QString> createDirectoryRecursively(const QString &path);
    static void addThemeSyncList(QWidget *controler);
    static bool UnzipFile(const QString &archivePath, const QString &extractDir);

signals:
    void updateThemeUISignal();
    void updateThemeUISignalByCustom(QColor customColor);
    
};

#endif // GLOBALFUNC_H
