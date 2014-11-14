#ifndef INOTE_H
#define INOTE_H
#include "stable.h"
#include "listitem.h"
#include <QListWidgetItem>

class ListItem;
class INote
{
//signals:
//    void sendDbChanged();

private:
    QString title;
    QString describe;
    QString content;
    QDateTime updateTime;
    QDateTime createTime;
    QString tags;
    int id;


public:
    INote();
    ~INote();

    static QList<INote> *queryAllNote();

    bool dbInsert();
    bool dbUpdate();
    bool dbDelete();


    QString getTitle() const;
    void setTitle(const QString &value);
    QString getDescribe() const;
    void setDescribe(const QString &value);
    QString getContent() const;
    void setContent(const QString &value);

    QString getTags() const;
    void setTags(const QString &value);

    int getId() const;
    void setId(int value);


    QDateTime getCreateTime() const;
    void setCreateTime(const QDateTime &value);
    QDateTime getUpdateTime() const;
	void setUpdateTime(const QDateTime &value);
    ListItem *viewHolder = nullptr;
    QListWidgetItem* listWidgetItem = nullptr;

    void updateFromContent();

};

#endif // INOTE_H
