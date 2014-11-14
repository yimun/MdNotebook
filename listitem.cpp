#include "listitem.h"
#include "ui_listitem.h"

ListItem::ListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListItem)
{
    ui->setupUi(this);
}

ListItem::~ListItem()
{
    delete ui;
}

void ListItem::bindDate(INote &note)
{
    ui->labelTitle->setText(note.getTitle());
    ui->labelContent->setText(note.getContent());
    ui->labelTime->setText(note.getUpdateTime().toString(tr("yyyy-M-d")));
}
