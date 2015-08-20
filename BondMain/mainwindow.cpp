#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Init_ui();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init_ui() {
    ui->pushButton_tb_info->setText(tr("Treasury Bond Info"));
    connect(ui->pushButton_tb_info,SIGNAL(clicked(bool)),this,SLOT(slot_pushButton_tb_info_clicked()));
}

void MainWindow::slot_pushButton_tb_info_clicked() {
//    ui->pushButton_tb_info->setEnabled(false);
    QProcess* process = new QProcess();
    process->startDetached("./app/BondInfo", QStringList()<<"000");
}
