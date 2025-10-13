#include "T_Home.h"
#include "Def.h"
#include "ElaImageCard.h"
#include "ElaAcrylicUrlCard.h"
#include "ElaTheme.h"
#include "ElaToolTip.h"
#include "ElaPopularCard.h"
#include "ElaFlowLayout.h"
#include "QDesktopServices"
#include "../SystemKit/BaseInclude.h"

#include "../Global/GlobalFunc.h"
#include <ElaInteractiveCard.h>
#include "../Global/GlobalSettings.h"


T_Home::T_Home(QWidget *parent)
    : BaseScrollPage{parent}
{
    // this->initWidget(tr("项目演示"), tr("1.0.0"), tr(""));

    // QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    // mainLayout->setAlignment(Qt::AlignCenter);

    // // 演示面板

    // QFrame *demoPanel = new QFrame(this);
    // demoPanel->setFixedSize(600, 400);
    // demoPanel->setStyleSheet("background: rgba(255,255,255,0.9); border-radius: 15px;");

    // QVBoxLayout *panelLayout = new QVBoxLayout(demoPanel);
    // QLabel *infoLabel = new QLabel(tr(" 这是一个快速开始项目 \n\n AsulKit 开发框架 \n\n 你可以从本项目基础上快速二开"), demoPanel);
    // infoLabel->setAlignment(Qt::AlignCenter);
    // infoLabel->setStyleSheet("font-size: 24px; color: #333;");

    // ElaPushButton *startBtn = new ElaPushButton(tr("转到设置"), demoPanel);
    // startBtn->setFixedSize(200, 50);
    // // startBtn->setStyleClass("defaultButton");

    // panelLayout->addWidget(infoLabel);
    // panelLayout->addWidget(startBtn, 0, Qt::AlignHCenter);
    // GlobalFunc::addThemeSyncList(startBtn);
    // mainLayout->addWidget(demoPanel);

    // connect(startBtn, &ElaPushButton::clicked, [=] {
    //     emit showSettingPage();
    //     GlobalFunc::showInfo("提示", "欢迎使用示例项目 (设置中)");
    // });


    // 预览窗口标题
    // this->initWidget(tr("主页"),tr("Home")," ");
        setWindowTitle("Home");

    setTitleVisible(false);
    setContentsMargins(2, 2, 0, 0);
    // 标题卡片区域
    ElaText* desText = new ElaText("                    ", this);
    desText->setTextPixelSize(18);
    ElaText* titleText = new ElaText(gSets->getProgramName(), this);
    titleText->setTextPixelSize(35);

    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setContentsMargins(30, 60, 10, 0);
    titleLayout->setAlignment(Qt::AlignRight);
    titleLayout->addWidget(desText);
    titleLayout->addWidget(titleText);

    ElaImageCard* backgroundCard = new ElaImageCard(this);
    backgroundCard->setBorderRadius(10);
    backgroundCard->setFixedHeight(400);
    backgroundCard->setMaximumAspectRatio(1.7);
    backgroundCard->setCardImage(QImage(":/Sources/icon/Home_Background.png"));

    ElaAcrylicUrlCard* urlCard1 = new ElaAcrylicUrlCard(this);
    urlCard1->setCardPixmapSize(QSize(62, 62));
    urlCard1->setFixedSize(195, 225);
    urlCard1->setTitlePixelSize(17);
    urlCard1->setTitleSpacing(25);
    urlCard1->setSubTitleSpacing(13);
    urlCard1->setUrl(gSets->getProgramRepository());

    urlCard1->setCardPixmap(QPixmap(":/Sources/icon/github.png"));
    urlCard1->setTitle("ASDK Github");
    urlCard1->setSubTitle("Use AsulSwiftDevelopKit To Create A Cool Project");
    ElaToolTip* urlCard1ToolTip = new ElaToolTip(urlCard1);
    urlCard1ToolTip->setToolTip(gSets->getProgramRepository());
    ElaAcrylicUrlCard* urlCard2 = new ElaAcrylicUrlCard(this);
    urlCard2->setCardPixmapSize(QSize(62, 62));
    urlCard2->setFixedSize(195, 225);
    urlCard2->setTitlePixelSize(17);
    urlCard2->setTitleSpacing(25);
    urlCard2->setSubTitleSpacing(13);
    urlCard2->setUrl("https://space.bilibili.com/511843089");
    urlCard2->setCardPixmap(QPixmap(":/Sources/icon/author.jpg"));
    urlCard2->setTitle(gSets->getProgramAuthor());
    urlCard2->setSubTitle("3191522462@qq.com");

    ElaScrollArea* cardScrollArea = new ElaScrollArea(this);
    cardScrollArea->setWidgetResizable(true);
    cardScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cardScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cardScrollArea->setIsGrabGesture(true, 0);
    cardScrollArea->setIsOverShoot(Qt::Horizontal, true);
    QWidget* cardScrollAreaWidget = new QWidget(this);
    cardScrollAreaWidget->setStyleSheet("background-color:transparent;");
    cardScrollArea->setWidget(cardScrollAreaWidget);
    QHBoxLayout* urlCardLayout = new QHBoxLayout();
    urlCardLayout->setSpacing(15);
    urlCardLayout->setContentsMargins(30, 0, 0, 6);
    urlCardLayout->addWidget(urlCard1);
    urlCardLayout->addWidget(urlCard2);
    urlCardLayout->addStretch();
    QVBoxLayout* cardScrollAreaWidgetLayout = new QVBoxLayout(cardScrollAreaWidget);
    cardScrollAreaWidgetLayout->setContentsMargins(0, 0, 0, 0);
    cardScrollAreaWidgetLayout->addStretch();
    cardScrollAreaWidgetLayout->addLayout(urlCardLayout);

    QVBoxLayout* backgroundLayout = new QVBoxLayout(backgroundCard);
    backgroundLayout->setContentsMargins(0, 0, 0, 0);
    backgroundLayout->addLayout(titleLayout);
    backgroundLayout->addWidget(cardScrollArea);

    // 推荐卡片
    ElaText* flowText = new ElaText("跳转", this);
    flowText->setTextPixelSize(20);
    QHBoxLayout* flowTextLayout = new QHBoxLayout();
    flowTextLayout->setContentsMargins(33, 0, 0, 0);
    flowTextLayout->addWidget(flowText);
    
    // ElaFlowLayout
    ElaPopularCard* homeCard = new ElaPopularCard(this);
    connect(homeCard, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        QDesktopServices::openUrl(QUrl(gSets->getProgramRepository()));
    });
    homeCard->setCardPixmap(QPixmap(":/Sources/icon/github.png"));
    homeCard->setTitle("ASDK Github");
    homeCard->setSubTitle(gSets->getProgramName());
    homeCard->setInteractiveTips("转到Github");
    homeCard->setDetailedText(gSets->getProgramDescription());

    ElaPopularCard* homeCard1 = new ElaPopularCard(this);
    connect(homeCard1, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        emit showSettingPage();
    });
    connect(eTheme,&ElaTheme::themeModeChanged,[=](ElaThemeType::ThemeMode themeMode){
        if(themeMode==ElaThemeType::Light)
            homeCard1->setCardPixmap(QPixmap(":/Sources/icon/splash_light.png"));
        else
            homeCard1->setCardPixmap(QPixmap(":/Sources/icon/splash_dark.png"));    
    });
    if(eTheme->getThemeMode()==ElaThemeType::Light)
        homeCard1->setCardPixmap(QPixmap(":/Sources/icon/splash_light.png"));
    else
        homeCard1->setCardPixmap(QPixmap(":/Sources/icon/splash_dark.png"));
    homeCard1->setTitle("ASDK Settings");
    homeCard1->setSubTitle(gSets->getProgramName());
    homeCard1->setCardButtontext("跳转");
    homeCard1->setInteractiveTips("转到设置");
    homeCard1->setDetailedText(gSets->getProgramName()+" 快速设置界面");

    ElaPopularCard* homeCard3 = new ElaPopularCard(this);
    connect(homeCard3, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        emit showLoginPage();
    });
    connect(eTheme,&ElaTheme::themeModeChanged,[=](ElaThemeType::ThemeMode themeMode){
        if(themeMode==ElaThemeType::Light)
            homeCard3->setCardPixmap(QPixmap(":/Sources/icon/splash_light.png"));   
        else
            homeCard3->setCardPixmap(QPixmap(":/Sources/icon/splash_dark.png"));    
    });
    if(eTheme->getThemeMode()==ElaThemeType::Light)
        homeCard3->setCardPixmap(QPixmap(":/Sources/icon/splash_light.png"));
    else
        homeCard3->setCardPixmap(QPixmap(":/Sources/icon/splash_dark.png"));
    homeCard3->setTitle("NEEPU Login");
    homeCard3->setSubTitle(gSets->getProgramName());
    homeCard3->setCardButtontext("跳转");
    homeCard3->setInteractiveTips("转到登录");
    homeCard3->setDetailedText(gSets->getProgramName()+" 快速设置界面");

    ElaFlowLayout* flowLayout = new ElaFlowLayout(0, 5, 5);
    flowLayout->setContentsMargins(30, 0, 0, 0);
    flowLayout->setIsAnimation(true);
    flowLayout->addWidget(homeCard);
    flowLayout->addWidget(homeCard1);
    flowLayout->addWidget(homeCard3);


    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("Home");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setSpacing(0);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(backgroundCard);
    centerVLayout->addSpacing(20);
    centerVLayout->addLayout(flowTextLayout);
    centerVLayout->addSpacing(10);
    centerVLayout->addLayout(flowLayout);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget);

}

