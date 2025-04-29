#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QDebug>
#include <unistd.h>
#include <fcntl.h>
#include <QSocketNotifier>
#include <linux/input.h>

#define DEVFILE "/dev/input/event0"
#define KEY_SWITCH_DEVICE_DIR "/dev/buttons" //三档钥匙开关 滚轮
#define KEY_SWITCH_ENABLE_DEVICE_DIR "/dev/buttonstop"   //使能按键

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent  *event);

private slots:
    void getKeyMatrixValue();
    void get_switch_key_value();
    void get_key_enable_value();

private:
    Ui::MainWindow *ui;
    int fd;
    int key_switch_fd_;
    int switch_enablekey_fd_;

    QSocketNotifier* matrix_sock;
    QSocketNotifier  *sockswitchkey;//模式切换
    QSocketNotifier  *sockenablekey;//使能开关
    QSocketNotifier  *sock_key_rooler_;      //滚轮
};

#endif // MAINWINDOW_H
