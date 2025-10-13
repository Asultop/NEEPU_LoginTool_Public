#ifndef NEEPULoginManager_H
#define NEEPULoginManager_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QString>

// 登录请求数据结构
struct LoginData {
    QString user;     // 用户名
    QString pass;     // 密码（已加密）
    QString code;     // 验证码（可选）
    int authmode;     // 认证模式
    QString pool;     // 运营商池（可选）
    int isp_id;       // ISP ID
    QString pxyacct;  // 代理账号
    QString wxwork_state; // 微信企业状态（可选）

    // 默认构造函数
    LoginData() : authmode(0), isp_id(0) ,wxwork_state(""), pxyacct("") ,code(""){}
    static LoginData fromJson(const QJsonObject &jsonObj);
};

// 登录结果结构
struct LoginResult {
    bool success = false;     // 是否成功
    int retCode;      // 返回码
    QString msg;      // 消息
    QString succ_url; // 成功后跳转URL
    QJsonObject data; // 附加数据
};

class NEEPULoginManager : public QObject
{
    Q_OBJECT
public:
    explicit NEEPULoginManager(QObject *parent = nullptr);
    ~NEEPULoginManager();

    // 设置host地址
    void setHost(const QString &host);
    
    // 登录函数
    void login(const LoginData &data);
    
    // 注销函数
    void logout();
    
    // 获取登录状态
    void getStatus();

signals:
    // 登录结果信号
    void loginResult(const LoginResult &result);
    
    // 注销结果信号
    void logoutResult(bool success, const QString &msg);
    
    // 状态结果信号
    void statusResult(const QJsonObject &status);
    
    // 错误信号
    void error(const QString &errorMsg);

private slots:
    // 网络请求完成处理
    void onLoginReplyFinished();
    void onLogoutReplyFinished();
    void onStatusReplyFinished();
    void onGetAcctReplyFinished();
    void onAuthReplyFinished();
    
    // 网络错误处理
    void onNetworkError(QNetworkReply::NetworkError error);

private:
    // 核心方法
    void portalLogin(const QJsonObject &params);
    void portalAuth(const QJsonObject &params);
    void getAcct(const QJsonObject &params);
    
    // 构建URL参数
    QString buildUrlParam(const QJsonObject &params);
    
    // 发送POST请求
    void sendPostRequest(const QString &url, const QJsonObject &params, QObject *receiver, const char *slot);

    // 成员变量
    QNetworkAccessManager *m_networkManager;
    QString m_host;               // 服务器地址
    LoginData m_currentLoginData; // 当前登录数据
    QJsonObject m_urlParams;      // URL参数
};

#endif // NEEPULoginManager_H