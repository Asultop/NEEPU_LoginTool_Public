#include "T_SettingScreen.h"
#include "../Global/GlobalFunc.h"
#include "../Global/GlobalSettings.h"
#include <ElaToggleSwitch.h>
#include <qboxlayout.h>
#include <qpixmap.h>
#include <qstylehints.h>
#include "../SystemKit/AsulApplication.h"
#include "Def.h"
#include "ElaApplication.h"

#include "ElaColorDialog.h"
#include "ElaPlainTextEdit.h"
#include "ElaPushButton.h"

#include "ElaScrollPageArea.h"

T_SettingScreen::T_SettingScreen(QWidget *parent)
    : BaseScrollPage{parent}
{
    this->initWidget(tr("设置"),tr("设置"),tr(""));


    QVBoxLayout *centerVLayout=new QVBoxLayout(centralWidget);

    ElaToggleSwitch *closeThemeColorSync=new ElaToggleSwitch(this);
    closeThemeColorSync->setIsToggled(gSets->getEnableThemeColorSyncWithSystem());
        connect(closeThemeColorSync,&ElaToggleSwitch::toggled,[=](bool checked){
        gSets->setEnableThemeColorSyncWithSystem(checked);
        if(checked) GlobalFunc::updateThemeUI();
    });

    ElaToggleSwitch *closeThemeModeSync=new ElaToggleSwitch(this);
    closeThemeModeSync->setIsToggled(gSets->getEnableThemeModeSyncWithSystem());
    connect(closeThemeModeSync,&ElaToggleSwitch::toggled,[=](bool checked){
        gSets->setEnableThemeModeSyncWithSystem(checked);
        if(checked){
            Qt::ColorScheme scheme = qApp->styleHints()->colorScheme();
            if(scheme == Qt::ColorScheme::Dark){
                eTheme->setThemeMode(ElaThemeType::Dark);
            }else eTheme->setThemeMode(ElaThemeType::Light);
        }
    });

    AsulComboBox *SwitchDisplayMode=new AsulComboBox(this);
    SwitchDisplayMode->addItems(QString{"Normal ElaMica Mica MicaAlt Acrylic DWMBlur"}.split(" "));
    SwitchDisplayMode->setCurrentText("Acrylic");
    eApp->setWindowDisplayMode(ElaApplicationType::Acrylic);
    
    connect(SwitchDisplayMode,&AsulComboBox::currentIndexChanged,[=](int index){
        QString filePath = gSets->getGLoc()->path()+"/mica.png";
        switch (index){
            case 0:
                // eApp->setElaMicaImagePath(filePath);
                eApp->setWindowDisplayMode(ElaApplicationType::Normal);
                break;
            case 1:
                // eApp->setElaMicaImagePath(filePath);
                eApp->setWindowDisplayMode(ElaApplicationType::ElaMica);
                break;
            case 2:
                // eApp->setElaMicaImagePath(filePath);
                eApp->setWindowDisplayMode(ElaApplicationType::Mica);
                break;
            case 3:
                // eApp->setElaMicaImagePath(filePath);
                eApp->setWindowDisplayMode(ElaApplicationType::MicaAlt);
                break;
            case 4:
                // eApp->setElaMicaImagePath(filePath);
                eApp->setWindowDisplayMode(ElaApplicationType::Acrylic);
                break;
            case 5:
                // eApp->setElaMicaImagePath(filePath);
                eApp->setWindowDisplayMode(ElaApplicationType::DWMBlur);
                break;
        }
    });

    

    //切换主题
    AsulComboBox * changeThemeMode = new AsulComboBox(this);
    changeThemeMode->addItems(QString{"Light Dark"}.split(" "));
    changeThemeMode->setCurrentText(eTheme->getThemeMode()==ElaThemeType::Dark?"Dark":"Light");
        connect(changeThemeMode,&AsulComboBox::currentIndexChanged,[=](int index){
        if(index==0){
            eTheme->setThemeMode(ElaThemeType::Light);
        }else eTheme->setThemeMode(ElaThemeType::Dark);
    });

    ElaPushButton * ColorPickBtn=new ElaPushButton(this);

    ElaColorDialog * colorDialog=new ElaColorDialog(this);

    GlobalFunc::addThemeSyncList(ColorPickBtn);
    ColorPickBtn->setText(tr("选择颜色"));

    colorDialog->setWindowTitle(" ");
    connect(ColorPickBtn,&ElaPushButton::clicked,[=](){
        // qDebug()<<"ColorPick Clicked"<<Qt::endl;
        colorDialog->exec();
    });
    connect(colorDialog, &ElaColorDialog::colorSelected, this, [=](const QColor& color) {
        GlobalFunc::updateThemeUI(color);
    });

    AsulComboBox * switchLanguage = new AsulComboBox(this);
    switchLanguage->addItems(gSets->getSupportedLang());
    switchLanguage->setCurrentText(gSets->getRegisterSettings()->value("lang").toString());

    connect(switchLanguage,&AsulComboBox::currentTextChanged,[=](QString val){
        gSets->getRegisterSettings()->setValue("lang",val);
        qApp->installTranslator(gSets->translators[val]);
        if(GlobalFunc::askDialog(this,tr("语言"),tr("是否重启以更换语言"))){
            qApp->quit();
            QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        }else{GlobalFunc::showInfo(tr("语言"),tr("重启以更换语言"),0x3f3f3f3f);}
    });
    

    ElaToggleSwitch *closeLogPreview=new ElaToggleSwitch(this);
    closeLogPreview->setIsToggled(gSets->getEnableLogPreview());
    connect(closeLogPreview,&ElaToggleSwitch::toggled,[=](bool checked){
        gSets->setEnableLogPreview(checked);
    });

    ElaLineEdit *ReconnectedIntervalEdit=new ElaLineEdit(this);
    ReconnectedIntervalEdit->setPlaceholderText("msec");
    ReconnectedIntervalEdit->setText(QString::number(gSets->getReconnectedInterval()));
    connect(ReconnectedIntervalEdit,&ElaLineEdit::textChanged,[=](QString val){
        gSets->setReconnectedInterval(val.toLongLong());
    });
    ReconnectedIntervalEdit->setFixedWidth(280);
    ReconnectedIntervalEdit->setFixedHeight(35);

    centerVLayout->addWidget(GlobalFunc::GenerateArea(this,new ElaText(tr("开启颜色同步"),this),new ElaText(tr("同步 Windows 的主题色"),this),closeThemeColorSync,false));
    centerVLayout->addWidget(GlobalFunc::GenerateArea(this,new ElaText(tr("开启明暗同步"),this),new ElaText(tr("同步 Windows 的明暗模式"),this),closeThemeModeSync,false));
    centerVLayout->addWidget(GlobalFunc::GenerateArea(this,new ElaText(tr("显示模式"),this),new ElaText(tr("切换窗口显示模式"),this),SwitchDisplayMode,false));
    centerVLayout->addWidget(GlobalFunc::GenerateArea(this,new ElaText(tr("切换主题"),this),new ElaText(tr("切换 Dark/Light"),this),changeThemeMode,false));
    centerVLayout->addWidget(GlobalFunc::GenerateArea(this,ElaIconType::Display,new ElaText(tr("选择颜色"),this),new ElaText(tr("选择自定义颜色"),this),ColorPickBtn,false));
    centerVLayout->addWidget(GlobalFunc::GenerateArea(this,ElaIconType::Globe,new ElaText(tr("切换语言"),this),new ElaText(tr("切换语言"),this),switchLanguage,false));
    centerVLayout->addWidget(GlobalFunc::GenerateArea(this,ElaIconType::Note,new ElaText(tr("开启日志预览"),this),new ElaText(tr("开启日志预览"),this),closeLogPreview,false));
    centerVLayout->addWidget(GlobalFunc::GenerateArea(this,ElaIconType::Clock,new ElaText(tr("重连间隔"),this),new ElaText(tr("重连间隔（毫秒）"),this),ReconnectedIntervalEdit,false));
    
    centerVLayout->addStretch();
}
