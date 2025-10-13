#include "GlobalFunc.h"
#include "Def.h"
#include "ElaComboBox.h"
#include "ElaContentDialog.h"
#include "ElaPlainTextEdit.h"
#include "ElaLineEdit.h"
#include "ElaText.h"
#include "GlobalSettings.h"
#include "qboxlayout.h"
#include <QApplication>
#include <ElaTheme.h>
#include <ElaPushButton.h>
#include <qnamespace.h>
#include <qprocess.h>
#include <qsize.h>
#include <qstylehints.h>
#include <QStandardPaths>
#include "../SystemKit/AsulApplication.h"

Q_SINGLETON_CREATE_CPP(GlobalFunc);
GlobalFunc::GlobalFunc(QObject *parent)
    : QObject{parent}{}

QColor GlobalFunc::getDarkerColor(const QColor &color, int factor){
    QColor result = color;
    result.setHslF(
        result.hslHueF(),
        result.hslSaturationF(),
        qMax(0.0, result.lightnessF() * (factor / 100.0)),
        result.alphaF()
        );
    return result;
}
void GlobalFunc::updateThemeUI(){
    emit gFunc->updateThemeUISignal();

    QColor bgColor = qApp->palette().color(QPalette::Highlight);
    eTheme->setThemeColor(ElaThemeType::Dark, ElaThemeType::PrimaryNormal, bgColor);
    eTheme->setThemeColor(ElaThemeType::Dark, ElaThemeType::PrimaryHover, GlobalFunc::getLighterColor(bgColor));
    eTheme->setThemeColor(ElaThemeType::Dark, ElaThemeType::PrimaryPress, GlobalFunc::getLighterColor(bgColor,105));
    eTheme->setThemeColor(ElaThemeType::Light, ElaThemeType::PrimaryNormal, bgColor);
    eTheme->setThemeColor(ElaThemeType::Light, ElaThemeType::PrimaryHover, GlobalFunc::getDarkerColor(bgColor));
    eTheme->setThemeColor(ElaThemeType::Light, ElaThemeType::PrimaryPress, GlobalFunc::getDarkerColor(bgColor,95));

}
bool GlobalFunc::UnzipFile(const QString &archivePath, const QString &extractDir){
    QString program;
#ifdef Q_OS_WIN
    program = QCoreApplication::applicationDirPath()+"/3rd/7z.exe"; // 默认安装路径
    if (!QFile::exists(program)) {
        program = QStandardPaths::findExecutable("7z.exe");

    }
#else
    program = QStandardPaths::findExecutable("7z");
#endif

    if (program.isEmpty()) {
        qWarning() << "7-Zip not found!";
        return false;
    }

    QProcess process;
    process.setProgram(program);
    process.setArguments({
        "x",
        archivePath,
        "-o" + extractDir,
        "-y" // 覆盖已存在文件
    });

    process.start();
    if (!process.waitForFinished(30000)) { // 30秒超时
        qWarning() << "[Process] timeout:" << process.errorString();
        return false;
    }

    if (process.exitCode() != 0) {
        qWarning() << "Extraction failed. Exit code:"
                   << process.exitCode()
                   << "Error:"
                   << process.readAllStandardError();
        return false;
    }

    return true;
}
void GlobalFunc::updateThemeUI(QColor customColor){
    emit gFunc->updateThemeUISignalByCustom(customColor);
    eTheme->setThemeColor(ElaThemeType::Dark, ElaThemeType::PrimaryNormal, customColor);
    eTheme->setThemeColor(ElaThemeType::Dark, ElaThemeType::PrimaryHover, GlobalFunc::getLighterColor(customColor));
    eTheme->setThemeColor(ElaThemeType::Dark, ElaThemeType::PrimaryPress, GlobalFunc::getLighterColor(customColor,105));
    eTheme->setThemeColor(ElaThemeType::Light, ElaThemeType::PrimaryNormal, customColor);
    eTheme->setThemeColor(ElaThemeType::Light, ElaThemeType::PrimaryHover, GlobalFunc::getDarkerColor(customColor));
}


void GlobalFunc::addThemeSyncList(QWidget* controler){

    if(ElaPushButton * pb = qobject_cast<ElaPushButton*>(controler)){
        QColor bgColor = qApp->palette().color(QPalette::Highlight);

        connect(aApp,&AsulApplication::themeChanged,[=](){
            if(!gSets->getEnableThemeColorSyncWithSystem()) return;
            QColor bgColor = qApp->palette().color(QPalette::Highlight);
            pb->setDarkDefaultColor(bgColor);
            pb->setLightDefaultColor(bgColor);
            pb->setDarkHoverColor(GlobalFunc::getLighterColor(bgColor));
            pb->setDarkPressColor(GlobalFunc::getLighterColor(bgColor, 105));
            pb->setLightHoverColor(GlobalFunc::getDarkerColor(bgColor));
            pb->setLightPressColor(GlobalFunc::getDarkerColor(bgColor, 95));
        });
        connect(gFunc,&GlobalFunc::updateThemeUISignal,[=](){
            QColor bgColor = qApp->palette().color(QPalette::Highlight);
            pb->setDarkDefaultColor(bgColor);
            pb->setLightDefaultColor(bgColor);
            pb->setDarkHoverColor(GlobalFunc::getLighterColor(bgColor));
            pb->setDarkPressColor(GlobalFunc::getLighterColor(bgColor, 105));
            pb->setLightHoverColor(GlobalFunc::getDarkerColor(bgColor));
            pb->setLightPressColor(GlobalFunc::getDarkerColor(bgColor, 95));
        });
        connect(gFunc,&GlobalFunc::updateThemeUISignalByCustom,[=](QColor customColor){

            pb->setDarkDefaultColor(customColor);
            pb->setLightDefaultColor(customColor);
            pb->setDarkHoverColor(GlobalFunc::getLighterColor(customColor));
            pb->setDarkPressColor(GlobalFunc::getLighterColor(customColor, 105));
            pb->setLightHoverColor(GlobalFunc::getDarkerColor(customColor));
            pb->setLightPressColor(GlobalFunc::getDarkerColor(customColor, 95));
        });
        if(!gSets->getEnableThemeColorSyncWithSystem()) return;
        pb->setDarkDefaultColor(bgColor);
        pb->setLightDefaultColor(bgColor);
        pb->setDarkHoverColor(GlobalFunc::getLighterColor(bgColor));
        pb->setDarkPressColor(GlobalFunc::getLighterColor(bgColor, 105));
        pb->setLightHoverColor(GlobalFunc::getDarkerColor(bgColor));
        pb->setLightPressColor(GlobalFunc::getDarkerColor(bgColor, 95));

    }
}
QColor GlobalFunc::getLighterColor(const QColor &color, int factor){
    QColor result = color;
    result.setHslF(
        result.hslHueF(),
        result.hslSaturationF(),
        qMin(1.0, result.lightnessF() * (factor / 100.0)),
        result.alphaF()
        );
    return result;
}
void GlobalFunc::showSuccess(QString title,QString content,int mSec){
    ElaMessageBar::success(ElaMessageBarType::Top,title,content,mSec==-1?GlobalFunc::getReadTime(content):mSec);
}
void GlobalFunc::showErr(QString title,QString content,int mSec){
    ElaMessageBar::error(ElaMessageBarType::Top,title,content,mSec==-1?GlobalFunc::getReadTime(content):mSec);
}
void GlobalFunc::showInfo(QString title,QString content,int mSec){
    ElaMessageBar::information(ElaMessageBarType::Top,title,content,mSec==-1?GlobalFunc::getReadTime(content):mSec);
}
void GlobalFunc::showWarn(QString title,QString content,int mSec){
    ElaMessageBar::warning(ElaMessageBarType::Top,title,content,mSec==-1?GlobalFunc::getReadTime(content):mSec);
}

double GlobalFunc::getReadTime(QString context){
    QFont font("Microsoft YaHei"); // 获取字体
    QFontMetrics metrics(font); // 使用字体创建字体度量对象
    int textWidth = metrics.horizontalAdvance(context); // 获取文本宽度
    QWidget* activeWindow = QApplication::activeWindow();
    // int screenWidth = window()->width(); // 获取屏幕宽度（或阅读区域宽度）
    int screenWidth = activeWindow ? activeWindow->width() : 800;
    // 计算阅读时间（秒）
    double readingTimeInSeconds = static_cast<double>(textWidth) / screenWidth * 60 * gSets->getCharactersPerMinute();
    // QLOG_DEBUG()<<"[Read_Time]  Context: "<<context<<" Time(ms): "<<readingTimeInSeconds;
    return readingTimeInSeconds;
}
double GlobalFunc::getReadWidth(ElaText *context){
    if(nullptr == context) return 0;
    QFont font=context->font();
    QFontMetrics metrics(font);
    int textWidth = metrics.horizontalAdvance(context->text());
    return textWidth;
}

bool GlobalFunc::askDialog(QWidget *parent,QString title,QString content){
    ElaContentDialog *askWatch=new ElaContentDialog(parent,false);
    QWidget * askWatchWidget=new QWidget(parent);
    ElaText *askText=new ElaText(title,parent);
    ElaPlainTextEdit *askLine =new ElaPlainTextEdit(parent);
    QVBoxLayout *askLayout=new QVBoxLayout();
    askLayout->addSpacing(5);
    askLayout->addWidget(askText,40);
    askLayout->addSpacing(10);
    askLayout->addWidget(askLine,60);
    askWatchWidget->setLayout(askLayout);
    askLine->setMinimumHeight(45);
    QFont tmFnt(askLine->font());
    tmFnt.setPixelSize(15);
    askLine->setFont(tmFnt);
    askLine->setReadOnly(true);
    askLine->appendPlainText(content);
    askText->setTextPointSize(20);
    askText->setTextPixelSize(20);
    askWatch->setRightButtonText(tr("是"));
    askWatch->setMiddleButtonText("");
    askWatch->setLeftButtonText(tr("否"));

    bool RET;
    connect(askWatch,&ElaContentDialog::rightButtonClicked,[&]{
        askWatch->close();
        RET=true;
    });
    connect(askWatch,&ElaContentDialog::leftButtonClicked,[&](){
        askWatch->close();
        RET=false;
    });
    askWatch->setCentralWidget(askWatchWidget);
    askWatch->exec();

    return RET;
}
bool GlobalFunc::askDialog(QString title,QString content){
    return askDialog(nullptr,title,content);
}
void GlobalFunc::confirmDialog(QWidget *parent,QString title, QString content){
    ElaContentDialog *askWatch=new ElaContentDialog(parent,false,false);
    QWidget * askWatchWidget=new QWidget(parent);
    ElaText *askText=new ElaText(title,parent);
    ElaPlainTextEdit *askLine =new ElaPlainTextEdit(parent);
    QVBoxLayout *askLayout=new QVBoxLayout();
    askLayout->addSpacing(5);
    askLayout->addWidget(askText,40);
    askLayout->addSpacing(10);
    askLayout->addWidget(askLine,60);
    askWatchWidget->setLayout(askLayout);
    askLine->setMinimumHeight(45);
    QFont tmFnt(askLine->font());
    tmFnt.setPixelSize(15);
    askLine->setFont(tmFnt);
    askLine->setReadOnly(true);
    askLine->appendPlainText(content);
    askText->setTextPointSize(20);
    askText->setTextPixelSize(20);
    askWatch->setRightButtonText(tr("是"));
    askWatch->setMiddleButtonText("");
    askWatch->setLeftButtonText(tr(""));
    askWatch->setCentralWidget(askWatchWidget);
    askWatch->exec();
}
void GlobalFunc::confirmDialog(QString title, QString content){
    confirmDialog(nullptr,title,content);
}



ElaScrollPageArea * GlobalFunc::GenerateArea(QWidget *parent,ElaText * title, ElaText * subtitle, QWidget * widget, bool convert){
    ElaScrollPageArea *configArea=new ElaScrollPageArea(parent);
    QHBoxLayout * configHLayout=new QHBoxLayout(configArea);
    QVBoxLayout* configVLayout=new QVBoxLayout();

    configHLayout->setContentsMargins(10,20,20,20);


    QFont font=title->font();
    font.setBold(true);
    title->setFont(font);
    title->setTextPixelSize(15);
    subtitle->setTextPixelSize(12);
    subtitle->setMinimumWidth((int)getReadWidth(subtitle));
    configVLayout->addWidget(title,7);
    configVLayout->addWidget(subtitle,4);

    configHLayout->addLayout(configVLayout);
    configHLayout->addWidget(widget);
    if(!convert && qobject_cast<ElaComboBox*> (widget)){
        widget->setFixedWidth(300);
    }
    return configArea;
}
ElaScrollPageArea * GlobalFunc::GenerateArea(QWidget *parent,ElaIconType::IconName iconType,ElaText * title, ElaText * subtitle, QWidget * widget, bool convert){

    ElaScrollPageArea *configArea=new ElaScrollPageArea(parent);
    QHBoxLayout * configHLayout=new QHBoxLayout();
    QVBoxLayout* configVLayout=new QVBoxLayout();
    QHBoxLayout * HLayout=new QHBoxLayout(configArea);
    // ElaText *titleText=new ElaText(title);
    // ElaText *detailText=new ElaText(subtitle);
    HLayout->setContentsMargins(10,0,0,0);

    configHLayout->setContentsMargins(10,20,20,20);


    QFont font=title->font();
    font.setBold(true);
    title->setFont(font);
    title->setTextPixelSize(15);
    subtitle->setTextPixelSize(12);
    subtitle->setMinimumWidth((int)getReadWidth(subtitle));
    configVLayout->addWidget(title,7);
    configVLayout->addWidget(subtitle,4);

    ElaText *iconWidget = new ElaText(parent);
    iconWidget->setFixedSize(QSize(50,50));
    iconWidget->setElaIcon(iconType);


    HLayout->addWidget(iconWidget);
    configHLayout->addLayout(configVLayout);

    configHLayout->addStretch();
    if (nullptr != widget) {
        configHLayout->addWidget(widget);
    }
    
    HLayout->addLayout(configHLayout);
    if(!convert ){
        if(qobject_cast<ElaComboBox*> (widget))
            widget->setFixedWidth(300);
        if(qobject_cast<ElaPushButton*> (widget))
            widget->setFixedWidth(300);

    }
    return configArea;
}
ElaScrollPageArea * GlobalFunc::GenerateArea(QWidget *parent,QString iconPath,ElaText * title, ElaText * subtitle, QWidget * widget, bool convert){
    ElaScrollPageArea *configArea=new ElaScrollPageArea(parent);
    QHBoxLayout * configHLayout=new QHBoxLayout();
    QVBoxLayout* configVLayout=new QVBoxLayout();
    QHBoxLayout * HLayout=new QHBoxLayout(configArea);
    // ElaText *titleText=new ElaText(title);
    // ElaText *detailText=new ElaText(subtitle);
    HLayout->setContentsMargins(10,0,0,0);

    configHLayout->setContentsMargins(10,20,20,20);


    QFont font=title->font();
    font.setBold(true);
    title->setFont(font);
    title->setTextPixelSize(15);
    subtitle->setTextPixelSize(12);
    subtitle->setMinimumWidth((int)getReadWidth(subtitle));
    configVLayout->addWidget(title,7);
    configVLayout->addWidget(subtitle,4);

    ElaText *iconWidget = new ElaText(parent);
    iconWidget->setFixedSize(QSize(50,50));
    
    if(iconPath.contains("${THEME}")){
        QPixmap icon(QString(iconPath).replace("${THEME}",eTheme->getThemeMode()==ElaThemeType::Dark?"dark":"light"));
        QPixmap simg = icon.scaled(iconWidget->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        iconWidget->setPixmap(simg);
        connect(eTheme,&ElaTheme::themeModeChanged,[=](ElaThemeType::ThemeMode themeMode){
            QPixmap icon(QString(iconPath).replace("${THEME}",themeMode==ElaThemeType::Dark?"dark":"light"));
            QPixmap simg = icon.scaled(iconWidget->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            iconWidget->setPixmap(simg);
        });
    }else{
        QPixmap icon(iconPath);
        QPixmap simg = icon.scaled(iconWidget->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        iconWidget->setPixmap(simg);
    }
    HLayout->addWidget(iconWidget);
    configHLayout->addLayout(configVLayout);
    configHLayout->addWidget(widget);
    HLayout->addLayout(configHLayout);
    if(!convert ){
        if(qobject_cast<ElaComboBox*> (widget))
            widget->setFixedWidth(300);
        if(qobject_cast<ElaPushButton*> (widget))
            widget->setFixedWidth(300);
    }
    return configArea;
}
ElaScrollPageArea * GlobalFunc::GenerateArea(QWidget *parent,ElaIconType::IconName iconType,ElaText * title, ElaText * subtitle, QHBoxLayout * widget, bool convert){

    ElaScrollPageArea *configArea=new ElaScrollPageArea(parent);
    QHBoxLayout * configHLayout=new QHBoxLayout();
    QVBoxLayout* configVLayout=new QVBoxLayout();
    QHBoxLayout * HLayout=new QHBoxLayout(configArea);
    // ElaText *titleText=new ElaText(title);
    // ElaText *detailText=new ElaText(subtitle);
    HLayout->setContentsMargins(10,0,0,0);

    configHLayout->setContentsMargins(10,20,20,20);


    QFont font=title->font();
    font.setBold(true);
    title->setFont(font);
    title->setTextPixelSize(15);
    subtitle->setTextPixelSize(12);
    subtitle->setMinimumWidth((int)getReadWidth(subtitle));
    configVLayout->addWidget(title,7);
    configVLayout->addWidget(subtitle,4);

    ElaText *iconWidget = new ElaText(parent);
    iconWidget->setFixedSize(QSize(50,50));
    iconWidget->setElaIcon(iconType);


    HLayout->addWidget(iconWidget);
    configHLayout->addLayout(configVLayout);

    configHLayout->addStretch();
    configHLayout->addLayout(widget);
    HLayout->addLayout(configHLayout);
    if(!convert ){
        for (int i = 0; i < widget->count(); ++i) {
            QLayoutItem* item = widget->itemAt(i);
            if (QWidget* wget = item->widget()) {
                if(qobject_cast<ElaComboBox*> (wget))
                    wget->setFixedWidth(300);
                if(qobject_cast<ElaPushButton*> (wget))
                    wget->setFixedWidth(300);
                if(qobject_cast<ElaLineEdit*> (widget))
                    wget->setFixedHeight(30);
            }
        }

    }
    return configArea;
}
ElaScrollPageArea * GlobalFunc::GenerateArea(QWidget *parent,QString iconPath,ElaText * title, ElaText * subtitle, QHBoxLayout * widget, bool convert){
    ElaScrollPageArea *configArea=new ElaScrollPageArea(parent);
    QHBoxLayout * configHLayout=new QHBoxLayout();
    QVBoxLayout* configVLayout=new QVBoxLayout();
    QHBoxLayout * HLayout=new QHBoxLayout(configArea);
    // ElaText *titleText=new ElaText(title);
    // ElaText *detailText=new ElaText(subtitle);
    HLayout->setContentsMargins(10,0,0,0);

    configHLayout->setContentsMargins(10,20,20,20);


    QFont font=title->font();
    font.setBold(true);
    title->setFont(font);
    title->setTextPixelSize(15);
    subtitle->setTextPixelSize(12);
    subtitle->setMinimumWidth((int)getReadWidth(subtitle));
    configVLayout->addWidget(title,7);
    configVLayout->addWidget(subtitle,4);

    ElaText *iconWidget = new ElaText(parent);
    iconWidget->setFixedSize(QSize(50,50));

    if(iconPath.contains("${THEME}")){
        QPixmap icon(QString(iconPath).replace("${THEME}",eTheme->getThemeMode()==ElaThemeType::Dark?"dark":"light"));
        QPixmap simg = icon.scaled(iconWidget->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        iconWidget->setPixmap(simg);
        connect(eTheme,&ElaTheme::themeModeChanged,[=](ElaThemeType::ThemeMode themeMode){
            QPixmap icon(QString(iconPath).replace("${THEME}",themeMode==ElaThemeType::Dark?"dark":"light"));
            QPixmap simg = icon.scaled(iconWidget->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            iconWidget->setPixmap(simg);
        });
    }else{
        QPixmap icon(iconPath);
        QPixmap simg = icon.scaled(iconWidget->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        iconWidget->setPixmap(simg);
    }
    HLayout->addWidget(iconWidget);
    configHLayout->addLayout(configVLayout);
    configHLayout->addLayout(widget);
    HLayout->addLayout(configHLayout);
    if(!convert ){
        for (int i = 0; i < widget->count(); ++i) {
            QLayoutItem* item = widget->itemAt(i);
            if (QWidget* wget = item->widget()) {
                if(qobject_cast<ElaComboBox*> (wget))
                    wget->setFixedWidth(300);
                if(qobject_cast<ElaPushButton*> (wget))
                    wget->setFixedWidth(300);
            }
        }

    }
    return configArea;
}
ElaScrollPageArea * GlobalFunc::GenerateArea(QWidget *parent,ElaText * title, QWidget * widget, bool convert){
    ElaScrollPageArea * Area=new ElaScrollPageArea(parent);
    QHBoxLayout * HLayout=new QHBoxLayout(Area);
    title->setMinimumHeight(45);
    title->setTextPixelSize(15);

    if(convert){
        HLayout->addWidget(title,7);
        HLayout->addWidget(widget,2);
        widget->setMinimumHeight(45);

    }else{
        HLayout->addWidget(title);
        HLayout->addWidget(widget);
        if(qobject_cast<ElaComboBox*> (widget))
            widget->setFixedWidth(300);
        if(qobject_cast<ElaPushButton*> (widget))
            widget->setFixedWidth(300);
    }

    return Area;
}
ElaScrollPageArea * GlobalFunc::GenerateArea(QWidget *parent,ElaText * title, QHBoxLayout * widget, bool convert){
    ElaScrollPageArea * Area=new ElaScrollPageArea(parent);
    QHBoxLayout * HLayout=new QHBoxLayout(Area);
    if(convert){
        title->setMinimumHeight(45);
        title->setTextPixelSize(15);
        for (int i = 0; i < widget->count(); ++i) {
            QLayoutItem* item = widget->itemAt(i);
            if (QWidget* wget = item->widget()) {
                wget->setMinimumHeight(45);
            }
        }
    }
    HLayout->addWidget(title);
    HLayout->addLayout(widget);
    return Area;
}
ElaScrollPageArea *GlobalFunc::GenerateArea(QWidget *parent, QWidget *title, QHBoxLayout *widget, bool convert){
    ElaScrollPageArea * Area=new ElaScrollPageArea(parent);
    QHBoxLayout * HLayout=new QHBoxLayout(Area);
    if(convert){
        for (int i = 0; i < widget->count(); ++i) {
            QLayoutItem* item = widget->itemAt(i);
            if (QWidget* wget = item->widget()) {
                wget->setMinimumHeight(45);
            }
        }
    }
    HLayout->addWidget(title);
    HLayout->addLayout(widget);
    return Area;
}
ElaScrollPageArea *GlobalFunc::GenerateArea(QWidget *parent, QHBoxLayout *title, QHBoxLayout *widget, bool convert){
    ElaScrollPageArea * Area=new ElaScrollPageArea(parent);
    QHBoxLayout * HLayout=new QHBoxLayout(Area);
    if(convert){
        for (int i = 0; i < widget->count(); ++i) {
            QLayoutItem* item = widget->itemAt(i);
            if (QWidget* wget = item->widget()) {
                wget->setMinimumHeight(45);
            }
        }
    }
    HLayout->addLayout(title);
    HLayout->addLayout(widget);
    return Area;
}
void GlobalFunc::saveResourceFile(QString resProfiex,QString resFileName,QString destFullPathFileName) {
    QString resFile;
    resFile=":/"+resProfiex+"/"+resFileName;
    QFile file;
    file.copy(resFile,destFullPathFileName);
}


std::pair<bool,QString> GlobalFunc::createDirectoryRecursively(const QString& path) {
    QDir dir;
    if (path.isEmpty()) return std::pair<bool,QString>(false,"Path is empty");
    if (dir.exists(path)) return std::pair<bool,QString>(true,"Directory already exists:"+path);
    bool result = dir.mkpath(path);
    return result?std::pair<bool,QString>(true,"Successfully created directory:"+path):std::pair<bool,QString>(false,"Failed to create directory:"+path);
}




