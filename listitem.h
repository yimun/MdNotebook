#ifndef LISTITEM_H
#define LISTITEM_H

#include "inote.h"

class INote;

namespace Ui {
class ListItem;
}

class ListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ListItem(QWidget *parent = 0);
    ~ListItem();
    void bindDate(INote &note);

private:
    Ui::ListItem *ui;
};

#endif // LISTITEM_H
