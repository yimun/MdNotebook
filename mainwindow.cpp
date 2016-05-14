#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QtWebKitWidgets>

#include "listitem.h"
#include "Md2HtmlFormat.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化文件为未保存状态
    isUntitled = true;

    // 初始化文件名为"未命名.txt"
    curFile = zh("木易笔记");

    // 初始化窗口标题为文件名
    setWindowTitle(curFile);
    initEditor();
    listNotes = INote::queryAllNote();
    initLeftList();

    this->ui->textEdit->setVisible(false);


}

MainWindow::~MainWindow()
{
    delete ui;
	//delete[]listNotes;
	//delete highlighter;
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        isUntitled = true;
        curFile = zh("未命名.txt");
        setWindowTitle(curFile);
        ui->textEdit->clear();
        ui->textEdit->setVisible(true);
    }
}

bool MainWindow::maybeSave()
{
    // 如果文档被更改了
    if (ui->textEdit->document()->isModified()) {

        // 自定义一个警告对话框
        QMessageBox box;
        box.setWindowTitle(zh("警告"));
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile + zh(" 尚未保存，是否保存？"));
        QPushButton *yesBtn = box.addButton(zh("是(&Y)"),
                        QMessageBox::YesRole);
        box.addButton(zh("否(&N)"), QMessageBox::NoRole);
        QPushButton *cancelBut = box.addButton(zh("取消"),
                        QMessageBox::RejectRole);
        box.exec();
        if (box.clickedButton() == yesBtn)
            return save();
        else if (box.clickedButton() == cancelBut)
            return false;

    }

    // 如果文档没有被更改，则直接返回true
    return true;
}

bool MainWindow::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                         zh("另存为"),curFile);
    if (fileName.isEmpty()) return false;
    return saveFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {

        // %1和%2分别对应后面arg两个参数，/n起换行的作用
        QMessageBox::warning(this, zh("多文档编辑器"),
                   zh("无法写入文件 %1：/n %2")
                  .arg(fileName).arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    // 鼠标指针变为等待状态
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << ui->textEdit->toPlainText();
    // 鼠标指针恢复原来的状态
    QApplication::restoreOverrideCursor();
    isUntitled = false;
    // 获得文件的标准路径
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName); // 新建QFile对象
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, zh("多文档编辑器"),
                             zh("无法读取文件 %1:\n%2.")
                             .arg(fileName).arg(file.errorString()));
        return false; // 只读方式打开文件，出错则提示，并返回false
    }

    QTextStream in(&file); // 新建文本流对象
    QApplication::setOverrideCursor(Qt::WaitCursor); // 设置鼠标状态为等待状态
    ui->textEdit->setPlainText(in.readAll());  // 读取文件的全部文本内容，并添加到编辑器中
    QApplication::restoreOverrideCursor(); // 恢复鼠标状态

    // 设置当前文件
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

// 初始化左侧的列表
void MainWindow::initLeftList()
{
    ui->listWidget->clear();
	qDebug() << "load records :" << listNotes->size() << endl;
    if(listNotes->size() == 0)
        return;
    qDebug()<<listNotes->size()<<listNotes->at(0).getUpdateTime().toString();
    QList<INote>::iterator it = listNotes->begin();
    while(it != listNotes->end()){
        QListWidgetItem *containner = new QListWidgetItem(ui->listWidget);
        ui->listWidget->addItem(containner);
        containner->setSizeHint(QSize(0,94));
        (*it).listWidgetItem = containner;
        ListItem *viewHolder = new ListItem(ui->listWidget);
		connect(viewHolder, SIGNAL(customContextMenuRequested(const QPoint &)), 
			this, SLOT(on_listItem_createContextMenu(const QPoint &)));
        viewHolder->bindDate(*it);
		(*it).viewHolder = viewHolder;
        ui->listWidget->setItemWidget(containner,viewHolder);
        it++;
    }
//    emit ui->listWidget->itemClicked(listNotes->at(0).listWidgetItem);
    currentIndex = 0;
    updateStatus(QString("load %1 records").arg(listNotes->size()));

}

// 自动更新数据库
void MainWindow::autoSave()
{
     if (ui->textEdit->document()->isModified()) {
         QString content = ui->textEdit->document()->toPlainText();
         INote item = listNotes->at(currentIndex);
         item.setContent(content);
         item.updateFromContent();
         item.viewHolder->bindDate(item);
		 listNotes->replace(currentIndex, item);
         if (item.dbUpdate())
             updateStatus(zh("更新成功"));
     }

}

void MainWindow::initEditor()
{
    highlighter = new Highlighter(ui->textEdit->document());

}

void MainWindow::selectIndex(int index)
{
    updateStatus(QString("%1").arg(index));
    if(listNotes != NULL && index < listNotes->size())
    {
        autoSave();
        currentIndex = index;
        QString content = listNotes->at(index).getContent();
        ui->textEdit->setPlainText(content);
        ui->textEdit->setFocus();

        Md2HtmlFormat f;
        string target = f.format(content.toStdString());
        ui->webView->setHtml(QString(target.c_str()));
    }


}

void MainWindow::updateFormatView()
{
    if (ui->textEdit->document()->isModified()){
        QString sourcecode = ui->textEdit->document()->toPlainText();
        string stdsrc = sourcecode.toStdString();
        Md2HtmlFormat f;
        string target = f.format(stdsrc);
        ui->webView->setHtml(QString(target.c_str()));
    }

}

// 新建动作
void MainWindow::on_action_New_triggered()
{
//    newFile();
    INote newItem;
    newItem.setContent("#title\n-----");
    newItem.setTitle("title");
    newItem.setUpdateTime(QDateTime::currentDateTime());
    QListWidgetItem *containner = new QListWidgetItem(ui->listWidget);
    containner->setSizeHint(QSize(0,94));
    newItem.listWidgetItem = containner;
    ListItem *viewHolder = new ListItem(ui->listWidget);
    connect(viewHolder, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(on_listItem_createContextMenu(const QPoint &)));
    viewHolder->bindDate(newItem);
    newItem.viewHolder = viewHolder;
    listNotes->insert(0,newItem);
    ui->listWidget->setItemWidget(containner,viewHolder);
    initLeftList();
    selectIndex(0);
    on_action_EditMode_triggered();
}

// 保存动作
void MainWindow::on_action_Save_triggered()
{
//    save();
    autoSave();
}

// 另存为动作
void MainWindow::on_action_SaveAs_triggered()
{
    saveAs();
}

// 打开动作
void MainWindow::on_action_Open_triggered()
{
    if (maybeSave()) {

        QString fileName = QFileDialog::getOpenFileName(this);

        // 如果文件名不为空，则加载文件
        if (!fileName.isEmpty()) {
            loadFile(fileName);
            ui->textEdit->setVisible(true);
        }
    }
}

// 关闭动作
void MainWindow::on_action_Close_triggered()
{
//    if (maybeSave()) {
//        ui->textEdit->clear();
//    }
    autoSave();
}

// 退出动作
void MainWindow::on_action_Exit_triggered()
{
    // 先执行关闭操作，再退出程序
    // qApp是指向应用程序的全局指针
    on_action_Close_triggered();
    qApp->quit();
}

// 撤销动作
void MainWindow::on_action_Undo_triggered()
{
    ui->textEdit->undo();
}

// 剪切动作
void MainWindow::on_action_Cut_triggered()
{
    ui->textEdit->cut();
}

// 复制动作
void MainWindow::on_action_Copy_triggered()
{
    ui->textEdit->copy();
}

// 粘贴动作
void MainWindow::on_action_Paste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_action_Find_triggered()
{
//    SearchDialog s(this);
//    s.show();
//    s.exec(); // 进入消息循环,等待退出
    // 搜索框获取焦点
    ui->editSearch->setFocus();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    // 如果maybeSave()函数返回true，则关闭程序
//    if (maybeSave()) {
//        event->accept();
//    } else {   // 否则忽略该事件
//        event->ignore();
//    }
    autoSave();
}

void MainWindow::updateStatus(QString s)
{
    ui->statusBar->showMessage(QTime::currentTime().toString()
                               + tr("  ")+ s);
}

void MainWindow::on_action_SearchDialog_triggered(SearchMsg &msg)
{
    updateStatus(msg.str);
}

// 当数据库发生改变时
void MainWindow::onDbChanged()
{

}

// 编辑框光标移动了
void MainWindow::on_textEdit_cursorPositionChanged()
{
    QTextCursor cursor = ui->textEdit->textCursor();
//    qDebug()<<cursor.columnNumber()<<cursor.position()<<cursor.anchor()<<cursor.blockNumber();
}

// 搜索框输入完成回车
void MainWindow::on_editSearch_returnPressed()
{
    QString s = ui->editSearch->text();
    updateStatus(s);
}

// 左侧列表项被单击选中
void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    int pos = index.row();
    selectIndex(pos);

}

// 编辑框文本改变了
void MainWindow::on_textEdit_textChanged()
{
    if(isDoubleMode)
        updateFormatView();

}

void MainWindow::on_listItem_createContextMenu(const QPoint &point)
{
	qDebug()<< point.x() << point.y();
}


void MainWindow::on_action_EditMode_triggered()
{
    ui->listWidget->setVisible(true);
    ui->textEdit->setVisible(true);
    ui->webView->setVisible(false);
    isDoubleMode = false;

}

void MainWindow::on_actionFormat_triggered()
{
    QString sourcecode = listNotes->at(currentIndex).getContent();
    string stdsrc = sourcecode.toStdString();
    Md2HtmlFormat f;
    string target = f.format(stdsrc);
    this->ui->textEdit->setPlainText(QString(target.c_str()));

}

// 切换到视图模式
void MainWindow::on_action_ViewMode_triggered()
{
    autoSave();
    ui->listWidget->setVisible(true);
    ui->textEdit->setVisible(false);
    ui->webView->setVisible(true);
    isDoubleMode = false;
    updateFormatView();
}

void MainWindow::on_actionDouble_triggered()
{
    ui->listWidget->setVisible(false);
    ui->textEdit->setVisible(true);
    ui->webView->setVisible(true);
    isDoubleMode = true;
}
