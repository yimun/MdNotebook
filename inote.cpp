#include "inote.h"
#include <QSqlQuery>
#include <boost/regex.hpp>
using namespace boost;
INote::INote()
{

}

INote::~INote()
{

}


QDateTime INote::getUpdateTime() const
{
    return updateTime;
}

void INote::setUpdateTime(const QDateTime &value)
{
    updateTime = value;
}

void INote::updateFromContent()
{
    // title
    regex ptitle("^#([^#]+?)$");
    string cs = content.toStdString();
    smatch match;
    if(regex_search(cs,match,ptitle)){
        title = QString::fromStdString(match[1]);
    }else
    {
        title = "untitled";
    }

    // describe

}

QDateTime INote::getCreateTime() const
{
    return createTime;
}

void INote::setCreateTime(const QDateTime &value)
{
    createTime = value;
}

int INote::getId() const
{
    return id;
}

void INote::setId(int value)
{
    id = value;
}



QString INote::getTags() const
{
    return tags;
}

void INote::setTags(const QString &value)
{
    tags = value;
}



QString INote::getContent() const
{
    return content;
}

void INote::setContent(const QString &value)
{
    content = value;
}

QString INote::getDescribe() const
{
    return describe;
}

void INote::setDescribe(const QString &value)
{
    describe = value;
}

QString INote::getTitle() const
{
    return title;
}

void INote::setTitle(const QString &value)
{
    title = value;
}


bool INote::dbInsert()
{
    QSqlQuery query;
    query.prepare("INSERT INTO tbl_note (content, tags) VALUES (:content, :tags)");
    query.bindValue(":content", this->content);
    query.bindValue(":tags", this->tags);
    bool result = query.exec();
    return result;
}

bool INote::dbUpdate()
{
    QSqlQuery query;
    query.prepare("UPDATE tbl_note SET content=:content,tags=:tags, \
                  title=:title,describe=:describe,\
                  updatetime=datetime(CURRENT_TIMESTAMP,'localtime') where id=:id");
    query.bindValue(":content", this->content);
    query.bindValue(":tags", this->tags);
    query.bindValue(":title", this->title);
    query.bindValue(":describe", this->describe);
    query.bindValue(":id",this->id);
    bool result = query.exec();
    return result;
}

bool INote::dbDelete()
{
    QSqlQuery query;
    query.prepare("DELETE FROM tbl_note where id=:id");
    query.bindValue(":id",this->id);
    return query.exec();
}


QList<INote>* INote::queryAllNote()
{
    QList<INote> *list = new QList<INote>;
    QSqlQuery query;
    query.exec("SELECT * FROM tbl_note ORDER BY updatetime DESC");
    while(query.next())
    {
        INote *item = new INote;
        item->setId(query.value(0).toInt());
        item->setTitle(query.value(1).toString());
        item->setDescribe(query.value(2).toString());
        item->setContent(query.value(3).toString());
        item->setTags(query.value(4).toString());
        item->setUpdateTime(query.value(5).toDateTime());
        item->setCreateTime(query.value(6).toDateTime());
        list->append(*item);
    }
    return list;
}

