#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QStandardItemModel;
class QItemSelectionModel;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QLabel *labelCurFile;
    QLabel *labelCellPos;
    QLabel *labelCellText;
    const int fixedColumCount=6;
    QStandardItemModel *m_model;
    QItemSelectionModel *m_selection;
private:
    void do_currentChanged(const QModelIndex &current,const QModelIndex &previous);
private:
    void iniModelData(QStringList &aFileContent);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actModelData_triggered();

    void on_actAppend_triggered();

    void on_actInsert_triggered();

    void on_actDelete_triggered();

    void on_actAlignLeft_triggered();

    void on_actAlignCenter_triggered();

    void on_actAlignRight_triggered();

    void on_actTextBold_triggered(bool checked);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
