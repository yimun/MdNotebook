#include "searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    // 信息在主窗口和查找窗口间的传递
    connect(this,SIGNAL(sendSearch(SearchMsg &)),parent,
            SLOT(on_action_SearchDialog_triggered(SearchMsg &)));

}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_lineEdit_textChanged(const QString &arg1)
{
    str_search = arg1;
}

void SearchDialog::on_pbCancel_clicked()
{
    this->close();
}

void SearchDialog::on_pbSearch_clicked()
{
    SearchMsg msg;
    msg.isBack = ui->cbBackForward->isChecked();
    msg.isCaseSensitive = ui->cbCaseSenstive->isChecked();
    msg.str = ui->lineEdit->text();
    emit sendSearch(msg);
}

void SearchDialog::on_cbCaseSenstive_toggled(bool checked)
{
    if(checked)
        QMessageBox::information(NULL, "Title", "checked", QMessageBox::Yes, QMessageBox::Yes);
    else
        QMessageBox::information(NULL, "Title", "no checked", QMessageBox::Yes, QMessageBox::Yes);
}
