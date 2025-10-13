#ifndef ASULCOMBOBOX_H
#define ASULCOMBOBOX_H

#include "ElaComboBox.h"
#include "qevent.h"
#include <QWidget>

class AsulComboBox:public ElaComboBox{
    Q_OBJECT
public:
    explicit AsulComboBox(QWidget* parent = nullptr);
    ~AsulComboBox();
    void wheelEvent(QWheelEvent *e) override
    {
        //disable the scroll function
        e->ignore();
    }
};

#endif // ASULCOMBOBOX_H
