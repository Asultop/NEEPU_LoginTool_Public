#include "GlobalSettings.h"
#include "../SystemKit/AsulApplication.h"
#include <QStyleHints>
#include <ElaTheme.h>
#include "GlobalFunc.h"
#include <ElaPushButton.h>
#include <ElaLineEdit.h>
#include <ElaText.h>



Q_SINGLETON_CREATE_CPP(GlobalSettings);
GlobalSettings::GlobalSettings(QObject *parent)
    : QObject{parent}{}

void GlobalSettings::init()
{
    QFile programJson(":/PropertySetting/Program_Property.json");
    if(programJson.open(QIODevice::ReadOnly)){
        QByteArray data = programJson.readAll();
        programJson.close();
        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);
        jsonObj = jsonDoc.object();
    }


    this->setCharactersPerMinute(120);                  // 每分钟字符数 （提升阅读体验）
    this->setGLoc(new QTemporaryDir());                 // 临时目录
    this->setEnableThemeColorSyncWithSystem(true);      // 启用主题颜色同步
    this->setEnableThemeModeSyncWithSystem(true);       // 启用主题模式同步
    this->setEnableLogPreview(false);                   // 禁用日志预览
    this->setReconnectedInterval(50000);                // 5min
    this->setSupportedLang(QStringList{"zh_CN","en_US"});
    
    foreach(QString lang,this->getSupportedLang()){
        translators[lang] = new QTranslator(this);
        if(translators[lang]->load(QString(":/translations/AsulKit_%1.qm").arg(lang))){
            //ToDo: Need Do Something But Not Now; 
            ;
        }
    }
    

    this->setRegisterSettings(new QSettings(getProgramOrganization(),getTargetName(),this));

    Qt::ColorScheme scheme = qApp->styleHints()->colorScheme();
    if(scheme == Qt::ColorScheme::Dark){
        eTheme->setThemeMode(ElaThemeType::Dark);
    }else eTheme->setThemeMode(ElaThemeType::Light);

    connect(qApp->styleHints(),&QStyleHints::colorSchemeChanged,[=](){
        if(!this->getEnableThemeModeSyncWithSystem()) return;

        Qt::ColorScheme scheme = qApp->styleHints()->colorScheme();
        if(scheme == Qt::ColorScheme::Dark){
            eTheme->setThemeMode(ElaThemeType::Dark);
        }else eTheme->setThemeMode(ElaThemeType::Light);
        GlobalFunc::updateThemeUI();
    });
    connect(aApp,&AsulApplication::themeChanged,[=](){
        GlobalFunc::updateThemeUI();
    });

}
void GlobalSettings::destroy(){

    delete this->getGLoc();
}

QString GlobalSettings::getProgramName()
{
    return jsonObj["programName"].toString();
}

QString GlobalSettings::getProgramVersion()
{
    return jsonObj["programVersion"].toString();
}

QString GlobalSettings::getTargetName()
{
    return jsonObj["targetName"].toString();
}

QString GlobalSettings::getProgramDescription()
{
    return jsonObj["programDescription"].toString();
}

QString GlobalSettings::getProgramAuthor()
{
    return jsonObj["programAuthor"].toString();
}

QString GlobalSettings::getProgramLicense()
{
    return jsonObj["programLicense"].toString();
}

QString GlobalSettings::getProgramRepository()
{
    return jsonObj["programRepository"].toString();
}

QString GlobalSettings::getProgramOrganization()
{
    return jsonObj["programOrganization"].toString();
}
