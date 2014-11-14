#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H
#include "stable.h"

typedef struct {
    QString str;
    bool isCaseSensitive;
    bool isBack;
}SearchMsg;

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = 0);
    ~SearchDialog();

signals:
    void sendSearch(SearchMsg &msg);

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_pbCancel_clicked();

    void on_pbSearch_clicked();

    void on_cbCaseSenstive_toggled(bool checked);

private:
    Ui::SearchDialog *ui;
    QString str_search;

};

#endif // SEARCHDIALOG_H
