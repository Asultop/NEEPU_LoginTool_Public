#include "T_Example.h"
#include "../Global/GlobalFunc.h"
#include "../Global/GlobalSettings.h"
#include "../SystemKit/AsulApplication.h"
#include "../../3rd/VDX/VDX.h"
#include "../../3rd/NEEPU-SDK/NEEPULoginManager.h"
#include "Def.h"
#include "ElaComboBox.h"
#include "ElaPushButton.h"
#include "ElaScrollPageArea.h"
#include "ElaDrawerArea.h"
#include "ElaToggleSwitch.h"
#include <qdatetime.h>
#include <qjsondocument.h>
#include <qjsonobject.h>

QString generateRandom(QString source){
    QString randomPool = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString randomStr = "";
    for(int i=0;i<4;i++){
        //randomStr+=randomPool[rand()%randomPool.length()-3];
        randomStr += randomPool[rand()%(randomPool.length()-3)];
    }
    return randomStr+source;
}

T_Example::T_Example(QWidget *parent)
    : BaseScrollPage{parent},
      m_loginManager(nullptr) // 初始化loginManager为nullptr
{

    this->initWidget(tr("登录"),tr("登录"),tr(""));
    QVBoxLayout *centerVLayout=new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0,20,20,20);
    auto gRS=gSets->getRegisterSettings();
    // 初始化成员变量
    m_accountEdit = new ElaLineEdit(this);
    m_accountEdit->setText(gRS->value("Account").toString());
    m_accountEdit->setPlaceholderText(tr("请输入账户"));
    m_accountEdit->setMinimumHeight(35);
    m_accountEdit->setFixedWidth(280);
    connect(m_accountEdit,&ElaLineEdit::textChanged,this,[=](){
        nothingUpdate=false;
    });
    

    
    m_passwordEdit = new ElaLineEdit(this);
    m_passwordEdit->setText(gRS->value("Password").toString());
    m_passwordEdit->setPlaceholderText(tr("请输入密码"));
    m_passwordEdit->setMinimumHeight(35);
    m_passwordEdit->setFixedWidth(200);
    connect(m_passwordEdit,&ElaLineEdit::textChanged,this,[=](){
        nothingUpdate=false;
    });

    ElaPushButton *refreshKeyBtn = new ElaPushButton(this);
    refreshKeyBtn->setText(tr("刷新"));
    refreshKeyBtn->setFixedSize(QSize(65,35));
    connect(refreshKeyBtn,&ElaPushButton::clicked,this,[=](){
        nothingUpdate=false;
    });
    
    connect(refreshKeyBtn,&ElaPushButton::clicked,this,[=](){
        VDX Crypto("5a3b9f207411a8ed");
        m_logEdit->appendPlainText(tr("密钥刷新请求:")+m_passwordEdit->text());
        std::string decryptedPassword;
        try{
            decryptedPassword = (Crypto.decryptToString(VDX::from_hex(m_passwordEdit->text().toStdString())));
        }catch(...){
            GlobalFunc::showErr("错误","密钥刷新失败");
            m_logEdit->appendPlainText(tr("密钥刷新失败: 密钥解密失败"));
            return;
        }

        if(decryptedPassword.length()<=4){
            GlobalFunc::showErr("错误","密钥刷新失败");
            m_logEdit->appendPlainText(tr("密钥刷新失败: 密钥小于 4"));
            return;
        }
        decryptedPassword = decryptedPassword.substr(4,decryptedPassword.length());
        m_passwordEdit->setText(QString::fromStdString(VDX::to_hex(Crypto.encrypt(generateRandom(QString::fromStdString(decryptedPassword)).toStdString()))));
        m_passwordEdit->setEchoMode(ElaLineEdit::Password);
        m_logEdit->appendPlainText(tr("密钥解密成功:")+QString::fromStdString(decryptedPassword));
        m_logEdit->appendPlainText(tr("密钥已刷新:")+m_passwordEdit->text());
        GlobalFunc::showSuccess("成功","密钥已刷新");
    });

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(m_passwordEdit);
    passwordLayout->addWidget(refreshKeyBtn);
    
    //Advanced Settings
    m_codeEdit = new ElaLineEdit(this);
    m_codeEdit->setText(gRS->value("Code").toString());
    m_codeEdit->setPlaceholderText(tr("请输入验证码"));
    m_codeEdit->setMinimumHeight(35);
    m_codeEdit->setFixedWidth(280);
    connect(m_codeEdit,&ElaLineEdit::textChanged,this,[=](){
        nothingUpdate=false;
    });
    
    m_ispIdEdit = new ElaLineEdit(this);
    m_ispIdEdit->setText(gRS->value("IspId").toString());
    m_ispIdEdit->setPlaceholderText(tr("请输入运营商ID"));
    m_ispIdEdit->setMinimumHeight(35);
    m_ispIdEdit->setFixedWidth(280);
    connect(m_ispIdEdit,&ElaLineEdit::textChanged,this,[=](){
        nothingUpdate=false;
    });
    
    m_pxyacctEdit = new ElaLineEdit(this);
    m_pxyacctEdit->setText(gRS->value("PxyAcct").toString());
    m_pxyacctEdit->setPlaceholderText(tr("请输入代理账号"));
    m_pxyacctEdit->setMinimumHeight(35);
    m_pxyacctEdit->setFixedWidth(280);
    connect(m_pxyacctEdit,&ElaLineEdit::textChanged,this,[=](){
        nothingUpdate=false;
    });
    
    m_wxworkStateEdit = new ElaLineEdit(this);
    m_wxworkStateEdit->setText(gRS->value("WxWorkState").toString());
    m_wxworkStateEdit->setPlaceholderText(tr("请输入微信企业状态"));
    m_wxworkStateEdit->setMinimumHeight(35);
    m_wxworkStateEdit->setFixedWidth(280);
    connect(m_wxworkStateEdit,&ElaLineEdit::textChanged,this,[=](){
        nothingUpdate=false;
    });
    
    m_authmodeEdit = new ElaLineEdit(this);
    m_authmodeEdit->setText(gRS->value("AuthMode").toString());
    m_authmodeEdit->setPlaceholderText(tr("请输入认证模式"));
    m_authmodeEdit->setMinimumHeight(35);
    m_authmodeEdit->setFixedWidth(280);
    connect(m_authmodeEdit,&ElaLineEdit::textChanged,this,[=](){
        nothingUpdate=false;
    });
    
    m_loginBtn = new ElaPushButton(this);
    m_loginBtn->setText(tr("登录"));
    m_loginBtn->setMinimumHeight(35);
    m_loginBtn->setStyleSheet("QPushButton{background-color: #0078D4; color: #FFFFFF;}");
    m_loginBtn->setFixedWidth(280);
    
    if(gSets->getRegisterSettings()->contains("Account")){
        m_accountEdit->setText(gSets->getRegisterSettings()->value("Account").toString());
    }
    if(gSets->getRegisterSettings()->contains("Password")){
        m_passwordEdit->setText(gSets->getRegisterSettings()->value("Password").toString());
        m_passwordEdit->setEchoMode(ElaLineEdit::Password);
    }

    m_logEdit = new ElaPlainTextEdit(this);
    m_logEdit->setPlaceholderText(tr("登录日志"));
    m_logEdit->setMinimumHeight(300);
    m_logEdit->setReadOnly(true);
    m_logEdit->setFont(QFont("Microsoft YaHei", 10));

    m_logEdit->setVisible(gSets->getEnableLogPreview());
    connect(gSets,&GlobalSettings::pEnableLogPreviewChanged,this,[=](){
        m_logEdit->setVisible(gSets->getEnableLogPreview());
    });
    
    m_networkPool = new ElaComboBox(this);
    m_networkPool->setMinimumHeight(35);
    m_networkPool->setFixedWidth(280);
    connect(m_networkPool,&ElaComboBox::currentTextChanged,this,[=](){
        nothingUpdate=false;
    });
    
    m_networkPool->addItem("联通");
    m_networkPool->addItem("移动");
    
    // 初始化成员变量networkMap
    m_networkMap["联通"] = "liantong";
    m_networkMap["移动"] = "yidong";

    m_drawerArea = new ElaDrawerArea(this);
    QWidget* drawerHeader = new QWidget(this);
    QHBoxLayout* drawerHeaderLayout = new QHBoxLayout(drawerHeader);
    ElaText* drawerIcon = new ElaText(this);
    drawerIcon->setTextPixelSize(15);
    drawerIcon->setElaIcon(ElaIconType::MessageArrowDown);
    drawerIcon->setFixedSize(25, 25);
    ElaText* drawerText = new ElaText("高级设置", this);
    drawerText->setTextPixelSize(15);

    ElaToggleSwitch* drawerSwitch = new ElaToggleSwitch(this);
    ElaText* drawerSwitchText = new ElaText("关", this);
    drawerSwitchText->setTextPixelSize(15);
    
    connect(drawerSwitch, &ElaToggleSwitch::toggled, this, [=](bool toggled) {
        if (toggled)
        {
            drawerSwitchText->setText("开");
            m_drawerArea->expand();
        }
        else
        {
            drawerSwitchText->setText("关");
            m_drawerArea->collpase();
        }
    });
    
    connect(m_drawerArea, &ElaDrawerArea::expandStateChanged, this, [=](bool isExpand) {
        drawerSwitch->setIsToggled(isExpand);
    });
    
    drawerHeaderLayout->addWidget(drawerIcon);
    drawerHeaderLayout->addWidget(drawerText);
    drawerHeaderLayout->addStretch();
    drawerHeaderLayout->addWidget(drawerSwitchText);
    drawerHeaderLayout->addWidget(drawerSwitch);


    // ElaPushButton *getStatusBtn = new ElaPushButton(this);
    // getStatusBtn->setText(tr("获取状态"));
    // getStatusBtn->setMinimumHeight(35);
    // // getStatusBtn->setStyleSheet("QPushButton{background-color: #0078D4; color: #FFFFFF;}");
    // getStatusBtn->setFixedWidth(280);
    
    // connect(getStatusBtn, &ElaPushButton::clicked, this, [=](){
    //     static bool connected=false;
    //     if(!connected){
    //         connected=true;
    //         connect(m_loginManager, &NEEPULoginManager::statusResult, [=](QJsonObject result) {
    //             m_logEdit->appendPlainText(tr("获取状态结果: %1").arg(QJsonDocument(result).toJson()));
    //         });
    //     }
    //     m_loginManager->getStatus();
        
    // });
    // getStatusBtn->hide();

    m_drawerArea->setDrawerHeader(drawerHeader);
    m_drawerArea->addDrawer(GlobalFunc::GenerateArea(this,ElaIconType::GearComplexCode,new ElaText("code",this),new ElaText("推测为验证码",this),m_codeEdit));
    m_drawerArea->addDrawer(GlobalFunc::GenerateArea(this,ElaIconType::GearComplexCode,new ElaText("isp_id",this),new ElaText("推测为运营商ID",this),m_ispIdEdit));
    m_drawerArea->addDrawer(GlobalFunc::GenerateArea(this,ElaIconType::GearComplexCode,new ElaText("pxyacct",this),new ElaText("推测为代播号",this),m_pxyacctEdit));
    m_drawerArea->addDrawer(GlobalFunc::GenerateArea(this,ElaIconType::GearComplexCode,new ElaText("wxwork_state",this),new ElaText("推测为微信企业状态",this),m_wxworkStateEdit));
    m_drawerArea->addDrawer(GlobalFunc::GenerateArea(this,ElaIconType::GearComplexCode,new ElaText("authmode",this),new ElaText("推测为认证模式 默认为0",this),m_authmodeEdit));
    
    centerVLayout->addWidget(GlobalFunc::GenerateArea(this,ElaIconType::User,new ElaText("账户",this),new ElaText("运营商预留的用户名",this),m_accountEdit));
    centerVLayout->addWidget(GlobalFunc::GenerateArea(this,ElaIconType::Key,new ElaText("密码",this),new ElaText("联通默认密码: 123456",this),passwordLayout));
    centerVLayout->addWidget(GlobalFunc::GenerateArea(this,ElaIconType::NetworkWired,new ElaText("运营商",this),new ElaText("选择您的运营商",this),m_networkPool));
    centerVLayout->addWidget(m_drawerArea);
    centerVLayout->addWidget(GlobalFunc::GenerateArea(this,ElaIconType::Globe,new ElaText("登录",this),new ElaText("登录到系统",this),m_loginBtn));
    // centerVLayout->addWidget(GlobalFunc::GenerateArea(this,ElaIconType::Globe,new ElaText("获取状态",this),new ElaText("获取当前登录状态",this),getStatusBtn));
    
    centerVLayout->addWidget(m_logEdit);
    centerVLayout->addSpacing(300);
    centerVLayout->addStretch();


    connect(m_passwordEdit, &ElaLineEdit::textChanged, [=](QString text){
        if(text.isEmpty()){
            m_passwordEdit->setEchoMode(ElaLineEdit::Normal);
        }
    });
    connect(this,&T_Example::componentsChangeable,this,[=](bool enable){
        m_loginBtn->setEnabled(enable);
        m_networkPool->setEnabled(enable);
        m_drawerArea->setEnabled(enable);
        m_accountEdit->setEnabled(enable);
        m_passwordEdit->setEnabled(enable);
        m_codeEdit->setEnabled(enable);
        m_ispIdEdit->setEnabled(enable);
        m_pxyacctEdit->setEnabled(enable);
        m_wxworkStateEdit->setEnabled(enable);
        m_authmodeEdit->setEnabled(enable);
        refreshKeyBtn->setEnabled(enable);
    });
    static bool loginRunning=false;
    connect(m_loginBtn, &ElaPushButton::clicked, this, [=](){
        if(m_loginBtn->text()!=tr("登录")){
            return;
        }
        if(m_accountEdit->text().isEmpty()) {
            GlobalFunc::confirmDialog(this, "账户", "请输入账户");
            return;
        }
        if(m_passwordEdit->text().isEmpty()) {
            GlobalFunc::confirmDialog(this, "密码", "请输入密码");
            return;
        }
        if(m_passwordEdit->text().length()<=1) {
            GlobalFunc::confirmDialog(this, "密码", "密码无法为 1 位");
            return;
        }
        
        if(m_passwordEdit->text().length() < 32){
            VDX Crypto("5a3b9f207411a8ed");
            m_passwordEdit->setText(QString::fromStdString(VDX::to_hex(Crypto.encrypt(generateRandom(m_passwordEdit->text()).toStdString()))));
            m_passwordEdit->setEchoMode(ElaLineEdit::Password);
        }
        else if(m_passwordEdit->text().length() > 32) {
            GlobalFunc::confirmDialog(this, "密钥", "密钥长度不能超过32位");
            m_passwordEdit->clear();
            return;
        }
        
        QJsonObject loginJson;
        loginJson["user"] = m_accountEdit->text();
        loginJson["pass"] = m_passwordEdit->text();
        loginJson["pool"] = m_networkMap[m_networkPool->currentText()];
        if(m_drawerArea->getIsExpand()){
            if(!m_codeEdit->text().isEmpty()){
                loginJson["code"] = m_codeEdit->text();
            }
            if(!m_authmodeEdit->text().isEmpty()){
                loginJson["authmode"] = m_authmodeEdit->text();
            }
            if(!m_ispIdEdit->text().isEmpty()){
                loginJson["isp_id"] = m_ispIdEdit->text();
            }
            if(!m_pxyacctEdit->text().isEmpty()){
                loginJson["pxyacct"] = m_pxyacctEdit->text();
            }
            if(!m_wxworkStateEdit->text().isEmpty()){
                loginJson["wxwork_state"] = m_wxworkStateEdit->text();
            }
        }
        if (!m_loginManager) {
            m_loginManager = new NEEPULoginManager(this);
            static bool inReconnectedMode=false;
            connect(m_loginManager, &NEEPULoginManager::loginResult, this, [=](const LoginResult &result) {
                m_logEdit->appendPlainText("result.Msg = "+result.succ_url);
                loginRunning=false;
                if (result.success) {
                    componentsChangeable(false);
                    GlobalFunc::showSuccess("登录成功", "登录成功!");
                    m_logEdit->appendPlainText(tr("登录成功!"));
                    if (!result.msg.isEmpty()) {
                        GlobalFunc::showInfo("消息", result.msg);
                    }
                    if(!nothingUpdate&&!inReconnectedMode&&GlobalFunc::askDialog(this,tr("密码"), tr("保存此次登录凭证？"))){
                        auto gRS=gSets->getRegisterSettings();
                        gRS->setValue("Account", m_accountEdit->text());
                        gRS->setValue("Password", m_passwordEdit->text());
                        gRS->setValue("NetworkPool", m_networkPool->currentText());
                        gRS->setValue("IspId", m_ispIdEdit->text());
                        gRS->setValue("PxyAcct", m_pxyacctEdit->text());
                        gRS->setValue("WxworkState", m_wxworkStateEdit->text());
                        gRS->setValue("AuthMode", m_authmodeEdit->text());

                        gRS->sync();
                        GlobalFunc::showSuccess("保存成功", "登录凭证已保存");
                    }
                    static bool asked=false;

                    static QTimer *reconnectedToNEEPUTimer;
                    if(!reconnectedToNEEPUTimer) reconnectedToNEEPUTimer=new QTimer(this);
                    if(!inReconnectedMode&&GlobalFunc::askDialog(this,tr("设置循环登录"),tr("保持登录状态即使掉线也尝试重新登录？"))){\
                        static bool connected=false;
                        reconnectedToNEEPUTimer->setInterval(gSets->getReconnectedInterval());
                        m_loginBtn->setEnabled(true);
                        m_loginBtn->setText(tr("取消循环登录"));
                        if(!connected){
                            connected=true;
                            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
                                connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
                                    if(loginRunning) return;
                                    if (reply->error() == QNetworkReply::NoError) {
                                        GlobalFunc::showSuccess("连接成功", "已成功连接到 互联网");
                                        m_logEdit->appendPlainText(tr("已成功连接到 互联网，下一次检测时间: %1").arg(QDateTime::currentDateTime().addMSecs(gSets->getReconnectedInterval()).toString("yyyy-MM-dd hh:mm:ss")));
                                    }else{
                                        GlobalFunc::showErr("连接失效", "准备重连到 NEEPU 寝室网");
                                        m_logEdit->appendPlainText(tr("无法连接到 NEEPU 寝室网，尝试重连: %1").arg(QDateTime::currentDateTime().addMSecs(gSets->getReconnectedInterval()).toString("yyyy-MM-dd hh:mm:ss")));
                                        loginRunning=true;
                                        m_loginManager->login(m_data);
                                        
                                    }
                            });
                            connect(reconnectedToNEEPUTimer,&QTimer::timeout,this,[=](){
                                inReconnectedMode=true;
                                m_loginBtn->setEnabled(true);
                                m_loginBtn->setText(tr("取消循环登录"));
                                
                                connect(m_loginBtn,&ElaPushButton::clicked,this,[=](){
                                    if(m_loginBtn->text()==tr("取消循环登录")){
                                        GlobalFunc::showInfo("取消循环登录", "已取消循环登录");
                                        m_loginBtn->setText(tr("登录"));
                                    }
                                    reconnectedToNEEPUTimer->stop();
                                    m_loginBtn->setEnabled(true);
                                    componentsChangeable(true);
                                });
                                
                                manager->get(QNetworkRequest(QUrl("http://www.baidu.com")));
                            });
                            manager->get(QNetworkRequest(QUrl("http://www.baidu.com")));
                        }
                        
                        reconnectedToNEEPUTimer->setInterval(gSets->getReconnectedInterval());
                        reconnectedToNEEPUTimer->start();
                        
                    }
                } else {
                    GlobalFunc::showErr("登录失败", QString("登录失败! 错误码: %1").arg(result.retCode));
                    m_logEdit->appendPlainText(tr("登录失败! 错误码: %1").arg(result.retCode));
                    if (!result.msg.isEmpty()) {
                        GlobalFunc::showInfo("错误消息", result.msg);
                    }
                    emit componentsChangeable(true);
                }
            });

            connect(m_loginManager, &NEEPULoginManager::error, this, [=](const QString &errorMsg) {
                loginRunning=false;
                GlobalFunc::showErr("发生错误", errorMsg);
                m_logEdit->appendPlainText(tr("发生错误: %1").arg(errorMsg));
            });
        }
        
        QJsonDocument loginDoc(loginJson);
        QByteArray loginData = loginDoc.toJson();
        m_logEdit->appendPlainText(tr("登录时间: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
        m_logEdit->appendPlainText(tr("账户: %1").arg(m_accountEdit->text()));
        m_logEdit->appendPlainText(tr("登录请求: %1").arg(QString(loginData)));
        m_data=LoginData::fromJson(loginJson);
        emit componentsChangeable(false);
        m_loginManager->login(m_data);
    });
    
    
}

T_Example::~T_Example(){
    if (m_loginManager) {
        delete m_loginManager;
        m_loginManager = nullptr;
    }
}

