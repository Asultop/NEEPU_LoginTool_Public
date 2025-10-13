#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QObject>
#include "stdafx.h"
#include "singleton.h"
#include <QTemporaryDir>
#include <QTranslator>
#include <QSettings>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

#define gSets GlobalSettings::getInstance()
class GlobalSettings : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(GlobalSettings);
    Q_PROPERTY_CREATE(qint64,QINT64_Example);
    Q_PROPERTY_CREATE(int,CharactersPerMinute);
    Q_PROPERTY_CREATE(QTemporaryDir*,GLoc);

    Q_PROPERTY_CREATE(bool,EnableThemeColorSyncWithSystem);
    Q_PROPERTY_CREATE(bool,EnableThemeModeSyncWithSystem);
    Q_PROPERTY_CREATE(bool,EnableLogPreview);
    Q_PROPERTY_CREATE(QString,EnableDisplayMode);
    Q_PROPERTY_CREATE(QStringList,SupportedLang);
    Q_PROPERTY_CREATE(qint64,ReconnectedInterval);

    Q_PROPERTY_CREATE(QSettings*,RegisterSettings);
public:
    QMap<QString,QTranslator*> translators;
public:
    explicit GlobalSettings(QObject *parent = nullptr);
    void init();
    void destroy();
    QString getProgramName();
    QString getProgramVersion();
    QString getProgramDescription();
    QString getProgramAuthor();
    QString getProgramLicense();
    QString getProgramRepository();
    QString getProgramOrganization();
    QString getTargetName();
private:
    QJsonObject jsonObj;
signals:
};

#endif // GLOBALSETTINGS_H
