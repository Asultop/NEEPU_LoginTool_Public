#ifndef ASULAPPLICATION_H
#define ASULAPPLICATION_H
#include <QApplication>

class AsulApplication : public QApplication {
    Q_OBJECT
public:

    using QApplication::QApplication;

    // 重写 event() 方法
    bool event(QEvent *event) override ;
signals:
    void themeChanged(); // 自定义信号，供外部连接
};

#define aApp (static_cast<AsulApplication *>(QCoreApplication::instance()))

#endif // ASULAPPLICATION_H
