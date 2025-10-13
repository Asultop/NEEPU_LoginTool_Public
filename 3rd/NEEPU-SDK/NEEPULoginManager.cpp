#include "NEEPULoginManager.h"
#include <QUrl>
#include <QDebug>

LoginData LoginData::fromJson(const QJsonObject &jsonObj) {
    LoginData data;
    data.user = jsonObj["user"].toString();
    data.pass = jsonObj["pass"].toString();
    data.code = jsonObj["code"].toString();
    data.authmode = jsonObj["authmode"].toInt(0);
    data.pool = jsonObj["pool"].toString();
    data.isp_id = jsonObj["isp_id"].toInt(0);
    data.pxyacct = jsonObj["pxyacct"].toString();
    if (jsonObj.contains("wxwork_state")) {
        data.wxwork_state = jsonObj["wxwork_state"].toString();
    }
    return data;
}
NEEPULoginManager::NEEPULoginManager(QObject *parent) : QObject(parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    m_host = "http://202.198.8.232/";

    connect(m_networkManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
        if (reply->error() != QNetworkReply::NoError) {
            QString errorMsg = "网络错误: " + reply->errorString();
            emit error(errorMsg);
            reply->deleteLater();
        }
    });
}

NEEPULoginManager::~NEEPULoginManager()
{
    delete m_networkManager;
}

void NEEPULoginManager::setHost(const QString &host)
{
    m_host = host;
    if (!m_host.endsWith("/")) {
        m_host += "/";
    }
}

void NEEPULoginManager::login(const LoginData &data)
{
    m_currentLoginData = data;

    QJsonObject params;
    params["user"] = data.user;
    params["pass"] = data.pass;
    params["code"] = data.code;
    params["authmode"] = data.authmode;
    params["pool"] = data.pool;
    params["isp_id"] = data.isp_id;
    params["pxyacct"] = data.pxyacct;
    
    if (!data.wxwork_state.isEmpty()) {
        params["wxwork_state"] = data.wxwork_state;
    }
    
    portalLogin(params);
}

void NEEPULoginManager::logout()
{
    QString url = m_host + "api/logoff.php" + buildUrlParam(m_urlParams);
    QNetworkRequest request((QUrl(url)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    
    QNetworkReply *reply = m_networkManager->post(request, QByteArray());
    connect(reply, &QNetworkReply::finished, this, &NEEPULoginManager::onLogoutReplyFinished);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred),
            this, &NEEPULoginManager::onNetworkError);
}

void NEEPULoginManager::getStatus()
{
    QString url = m_host + "api/stat.php" + buildUrlParam(m_urlParams);
    QNetworkRequest request((QUrl(url)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    
    QJsonObject params;
    params["user"] = m_currentLoginData.user;
    params["pass"] = m_currentLoginData.pass;
    
    QNetworkReply *reply = m_networkManager->post(request, buildUrlParam(params).toUtf8());
    connect(reply, &QNetworkReply::finished, this, &NEEPULoginManager::onStatusReplyFinished);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred),
            this, &NEEPULoginManager::onNetworkError);
}

void NEEPULoginManager::portalLogin(const QJsonObject &params)
{
    portalAuth(params);
}

void NEEPULoginManager::portalAuth(const QJsonObject &params)
{
    QString url = m_host + "api/login.php" + buildUrlParam(m_urlParams);
    QNetworkRequest request((QUrl(url)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    
    QNetworkReply *reply = m_networkManager->post(request, buildUrlParam(params).toUtf8());
    connect(reply, &QNetworkReply::finished, this, &NEEPULoginManager::onAuthReplyFinished);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred),
            this, &NEEPULoginManager::onNetworkError);
}

void NEEPULoginManager::getAcct(const QJsonObject &params)
{
    QString url = m_host + "api/getacct.php" + buildUrlParam(m_urlParams);
    QNetworkRequest request((QUrl(url)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    
    QNetworkReply *reply = m_networkManager->post(request, buildUrlParam(params).toUtf8());
    connect(reply, &QNetworkReply::finished, this, &NEEPULoginManager::onGetAcctReplyFinished);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred),
            this, &NEEPULoginManager::onNetworkError);
}

QString NEEPULoginManager::buildUrlParam(const QJsonObject &params)
{
    if (params.isEmpty()) {
        return "";
    }
    
    QStringList paramList;
    for (auto it = params.constBegin(); it != params.constEnd(); ++it) {
        QString key = it.key();
        QJsonValue value = it.value();
        
        QString valueStr;
        if (value.isString()) {
            valueStr = value.toString();
        } else if (value.isDouble()) {
            valueStr = QString::number(value.toDouble());
        } else if (value.isBool()) {
            valueStr = value.toBool() ? "true" : "false";
        }
        
        paramList << key + "=" + QUrl::toPercentEncoding(valueStr);
    }
    
    return "?" + paramList.join("&");
}

void NEEPULoginManager::sendPostRequest(const QString &url, const QJsonObject &params, QObject *receiver, const char *slot)
{
    QNetworkRequest request((QUrl(url)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    
    QNetworkReply *reply = m_networkManager->post(request, buildUrlParam(params).toUtf8());
    // connect(reply, &QNetworkReply::finished, receiver, slot);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred),
            this, &NEEPULoginManager::onNetworkError);
}

void NEEPULoginManager::onLoginReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = jsonDoc.object();
    
    LoginResult result;
    result.retCode = jsonObj["ret"].toInt();
    // result.success = (result.retCode == 0 || result.retCode == 3);
    result.success = result.retCode == 3;
    result.msg = jsonObj["msg"].toString();
    result.succ_url = jsonObj["succ_url"].toString();
    if (jsonObj.contains("data")) {
        result.data = jsonObj["data"].toObject();
    }
    
    if (result.success) {
        QString url = m_host + "api/ack_auth.php" + buildUrlParam(m_urlParams);
        QNetworkRequest request((QUrl(url)));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        
        QJsonObject params;
        params["user"] = m_currentLoginData.user;
        params["pass"] = m_currentLoginData.pass;
        
        m_networkManager->post(request, buildUrlParam(params).toUtf8());
    }
    
    emit loginResult(result);
    reply->deleteLater();
}

void NEEPULoginManager::onLogoutReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = jsonDoc.object();
    
    bool success = (jsonObj["ret"].toInt() == 0);
    QString msg = jsonObj["msg"].toString();
    if (msg.isEmpty()) {
        msg = success ? "下线成功！" : "下线失败！";
    }
    
    emit logoutResult(success, msg);
    reply->deleteLater();
}

void NEEPULoginManager::onStatusReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = jsonDoc.object();
    
    emit statusResult(jsonObj);
    reply->deleteLater();
}

void NEEPULoginManager::onGetAcctReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = jsonDoc.object();
    if (jsonObj["ret"].toInt() == 0 && jsonObj.contains("data")) {
        QJsonObject dataObj = jsonObj["data"].toObject();
        if (dataObj.contains("rows")) {
            qDebug() << "获取到代拨账号列表";
        }
    }
    portalAuth({
        {"user", m_currentLoginData.user},
        {"pass", m_currentLoginData.pass},
        {"code", m_currentLoginData.code},
        {"authmode", m_currentLoginData.authmode},
        {"pool", m_currentLoginData.pool},
        {"isp_id", m_currentLoginData.isp_id},
        {"pxyacct", m_currentLoginData.pxyacct}
    });
    
    reply->deleteLater();
}

void NEEPULoginManager::onAuthReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    
    // 首先检查网络错误
    if (reply->error() != QNetworkReply::NoError) {
        LoginResult result;
        result.success = false;
        result.retCode = -1;
        result.msg = "网络请求失败: " + reply->errorString();
        emit loginResult(result);
        reply->deleteLater();
        return;
    }
    
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    
    // 检查JSON解析是否成功
    if (jsonDoc.isNull() || jsonDoc.isEmpty()) {
        LoginResult result;
        result.success = false;
        result.retCode = -2;
        result.msg = "无效的响应数据";
        emit loginResult(result);
        reply->deleteLater();
        return;
    }
    
    QJsonObject jsonObj = jsonDoc.object();
    
    LoginResult result;
    result.retCode = jsonObj["ret"].toInt();
    result.success = (result.retCode == 0 || result.retCode == 3);
    result.msg = jsonObj["msg"].toString();
    result.succ_url = jsonObj["succ_url"].toString();
    if (jsonObj.contains("data")) {
        result.data = jsonObj["data"].toObject();
    }
    if (result.success) {
        getStatus();
    }
    
    emit loginResult(result);
    reply->deleteLater();
}

void NEEPULoginManager::onNetworkError(QNetworkReply::NetworkError err)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        QString errorMsg = "网络请求失败: " + reply->errorString();
        emit error(errorMsg);
        reply->deleteLater();
    }
}