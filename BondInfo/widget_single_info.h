#ifndef WIDGET_SINGLE_INFO_H
#define WIDGET_SINGLE_INFO_H

#include <QWidget>

#include "basebond.h"

namespace Ui {
class Widget_Single_Info;
}

class Widget_Single_Info : public QWidget
{
    Q_OBJECT

public:
    explicit Widget_Single_Info(QWidget *parent = 0);
    ~Widget_Single_Info();

private:
    Ui::Widget_Single_Info *ui;

public slots:
    void slot_refresh_data(const BaseBond &bond);


};

#endif // WIDGET_SINGLE_INFO_H
