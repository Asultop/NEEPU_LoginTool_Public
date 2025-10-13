#ifndef T_EXAMPLE_H
#define T_EXAMPLE_H

#include <QWidget>
#include <qtmetamacros.h>
#include "../SystemKit/BaseScrollPage.h"
#include "../CTL_AsulComboBox/AsulComboBox.h"
#include "ElaDrawerArea.h"
#include "../../3rd/NEEPU-SDK/NEEPULoginManager.h"
class T_Example : public BaseScrollPage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Example(QWidget *parent = nullptr);
    ~T_Example();
private:
    ElaLineEdit *m_accountEdit;
    ElaLineEdit *m_passwordEdit;
    ElaLineEdit *m_codeEdit;
    ElaLineEdit *m_ispIdEdit;
    ElaLineEdit *m_pxyacctEdit;
    ElaLineEdit *m_wxworkStateEdit;
    ElaLineEdit *m_authmodeEdit;
    ElaPushButton *m_loginBtn;
    ElaPlainTextEdit *m_logEdit;
    ElaComboBox *m_networkPool;
    ElaDrawerArea *m_drawerArea;
    NEEPULoginManager *m_loginManager;
    QMap<QString, QString> m_networkMap;
    LoginData m_data;
    bool nothingUpdate=true;
signals:
    void componentsChangeable(bool);

};

#endif // T_EXAMPLE_H
