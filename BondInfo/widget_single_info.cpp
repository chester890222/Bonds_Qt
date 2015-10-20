#include "widget_single_info.h"
#include "ui_widget_single_info.h"

Widget_Single_Info::Widget_Single_Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget_Single_Info)
{
    ui->setupUi(this);
}

Widget_Single_Info::~Widget_Single_Info()
{
    delete ui;
}


void Widget_Single_Info::slot_refresh_data(const BaseBond &bond) {
    QDate today = QDate::currentDate();
    ui->lineEdit_code->setText(bond.bondCode);
    ui->lineEdit_name->setText(bond.name);
    ui->lineEdit_term->setText(QString::number(bond.term,'f',2));
    ui->lineEdit_issueAmount->setText(QString::number(bond.issueAmount,'f',2));
    ui->lineEdit_carryDate->setText(bond.carryDate.toString("yyyy-MM-dd"));
    ui->lineEdit_accInterest->setText(QString::number(bond.cal_accInterest(today),'f',4));
    ui->lineEdit_ttm->setText(QString::number(bond.cal_timeToMaturity(today),'f',4));
    ui->lineEdit_maturityDate->setText(bond.maturityDate.toString("yyyy-MM-dd"));
    ui->lineEdit_paymentFrequency->setText(QString::number(bond.paymentFrequency,'f',4));
    ui->lineEdit_nextCouponDate->setText(bond.cal_next_payment_date(today).toString("yyyy-MM-dd"));

}
