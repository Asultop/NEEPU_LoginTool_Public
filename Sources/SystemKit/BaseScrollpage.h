#ifndef BASESCROLLPAGE_H
#define BASESCROLLPAGE_H

#include <QWidget>
#include "ElaScrollPage.h"

#include "BaseInclude.h"
#include "ElaScrollPageArea.h"
#include <ElaListView.h>


class BaseScrollPage : public ElaScrollPage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit BaseScrollPage(QWidget *parent=nullptr);
    ~BaseScrollPage();
    void createCustomWidget(QString desText);
    QWidget* centralWidget,*debugWidget;
    // QVBoxLayout* centerVLayout;
    void addTopWidget(QWidget *widget);
    void initWidget(QString title,QString subTitle,QString content);

};

#endif // BASESCROLLPAGE_H
