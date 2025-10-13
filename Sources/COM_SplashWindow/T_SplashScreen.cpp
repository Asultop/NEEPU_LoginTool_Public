#include "T_SplashScreen.h"
#include <windows.h>
#include <QThreadPool>

T_SplashScreen::T_SplashScreen(QWidget *parent,const QString productName,const QString productImg)
    : ElaWindow{parent}{

    BaseScrollPage* splashPage=new BaseScrollPage(this);
    splashPage->initWidget("",tr("请等待..."),"");


    this->setFixedSize(QSize(400,244));
    QHBoxLayout * icoLayout=new QHBoxLayout();
    ElaText *ico=new ElaText(this);
    if(productImg.contains("${THEME}")){
        ico->setPixmap(QPixmap(QString(productImg).replace("${THEME}",eTheme->getThemeMode()==ElaThemeType::Light?"light":"dark")));
    }else{
        ico->setPixmap(QPixmap(QString(productImg)));
    }

    ElaText *icoText=new ElaText(productName,this);
    icoText->setTextStyle(ElaTextType::Title);
    icoLayout->addWidget(ico);
    icoLayout->addWidget(icoText);
    icoLayout->addSpacing(15);
    QHBoxLayout * launcherLayout=new QHBoxLayout();
    ElaText * launchingText=new ElaText(tr("正在启动 ..."));
    launchingText->setTextStyle(ElaTextType::Subtitle);
    ElaProgressBar *launchingBar=new ElaProgressBar(this);
    launchingBar->setRange(0, 0);
    launchingBar->setMinimumHeight(50);
    launcherLayout->addWidget(launchingText);
    launcherLayout->addWidget(launchingBar);

    QVBoxLayout *centerVLayout = new QVBoxLayout(splashPage->centralWidget);
    centerVLayout->setContentsMargins(5,5,20,30);
    centerVLayout->addLayout(icoLayout);
    centerVLayout->addLayout(launcherLayout);

    this->setWindowButtonFlags(ElaAppBarType::CloseButtonHint);
    this->setWindowButtonFlag(ElaAppBarType::CloseButtonHint,false);
    this->setUserInfoCardVisible(false);
    this->setIsStayTop(false);
    this->setIsFixedSize(true);
    this->setFixedSize(QSize(400,300));
    this->setIsNavigationBarEnable(false);
    this->setNavigationBarDisplayMode(ElaNavigationType::Minimal);
    this->setWindowButtonFlags(ElaAppBarType::CloseButtonHint | ElaAppBarType::MinimizeButtonHint);
    this->addPageNode("None",splashPage,ElaIconType::AddressCard);
}


T_SplashScreen::~T_SplashScreen(){

}
