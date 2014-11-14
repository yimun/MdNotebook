
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "stable.h"
#include "searchdialog.h"
#include "inote.h"
#include "highlighter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void newFile();   // 新建操作
    bool maybeSave(); // 判断是否需要保存
    bool save();      // 保存操作
    bool saveAs();    // 另存为操作
    bool saveFile(const QString &fileName); // 保存文件

    bool loadFile(const QString &fileName); // 加载文件

    void initLeftList();  // 初始化左侧列表栏

    void autoSave(); // 判断是否更新并保存

    void initEditor();

    void selectIndex(int index);

    void updateFormatView();


private slots:

    void on_action_New_triggered();

    void on_action_Save_triggered();

    void on_action_SaveAs_triggered();

    void on_action_Open_triggered();

    void on_action_Close_triggered();

    void on_action_Exit_triggered();

    void on_action_Undo_triggered();

    void on_action_Cut_triggered();

    void on_action_Copy_triggered();

    void on_action_Paste_triggered();

    void on_action_Find_triggered();

    void on_textEdit_cursorPositionChanged();

    void on_editSearch_returnPressed();

    void on_listWidget_clicked(const QModelIndex &index);

    void on_textEdit_textChanged();

    void on_action_SearchDialog_triggered(SearchMsg &msg);

    void onDbChanged();
	void on_listItem_createContextMenu(const QPoint &);

    void on_action_EditMode_triggered();

    void on_actionFormat_triggered();

    void on_action_ViewMode_triggered();

    void on_actionDouble_triggered();

protected:
    void closeEvent(QCloseEvent *event); // 关闭事件

private:
    Ui::MainWindow *ui;

    // 为真表示文件没有保存过，为假表示文件已经被保存过了
    bool isUntitled;

    // 保存当前文件的路径
    QString curFile;

    void updateStatus(QString &s);

    QList<INote> *listNotes;

    int currentIndex;
	INote currentINote;
    Highlighter *highlighter;
    bool isDoubleMode;


};

#endif // MAINWINDOW_H
