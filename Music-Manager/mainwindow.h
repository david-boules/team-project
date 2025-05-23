#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void loadUsers();

    ~MainWindow();

private slots:
    void on_PushButton_login_clicked();


    void on_switch2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
