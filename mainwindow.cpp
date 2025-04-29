#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label_3->hide();
    ui->label_5->hide();

    fd=open(DEVFILE, O_RDONLY|O_NONBLOCK);
    if(fd < 0)
    {
        qDebug()<<"/dev/input/event0 open fail.";
        exit(0);
    }
    key_switch_fd_ = open(KEY_SWITCH_DEVICE_DIR,O_RDONLY|O_NONBLOCK);
    if(key_switch_fd_ < 0)
    {
        qDebug()<<"/dev/buttons open fail.";
        exit(0);
    }

    switch_enablekey_fd_ = open(KEY_SWITCH_ENABLE_DEVICE_DIR,O_RDONLY|O_NONBLOCK);
    if(switch_enablekey_fd_<0 )
    {
        qDebug()<<"/dev/buttonstop open fail.";
        exit(0);
    }

    matrix_sock = new QSocketNotifier(fd,QSocketNotifier::Read, this);
    connect(matrix_sock, SIGNAL(activated(int)), this, SLOT(getKeyMatrixValue()));

    sockswitchkey = new QSocketNotifier(key_switch_fd_,QSocketNotifier::Read, this);
//    connect(sockswitchkey, SIGNAL(activated(int)), this, SLOT(get_switch_key_value()));

    sockenablekey = new QSocketNotifier(switch_enablekey_fd_,QSocketNotifier::Read, this);
//    connect(sockenablekey, SIGNAL(activated(int)), this, SLOT(get_key_enable_value()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent  *event)
{
    //qDebug()<<event->key();
//    qDebug() << event->text();
    ui->label_5->setText(QString::number(event->key()));

}

void MainWindow::getKeyMatrixValue()
{
    struct input_event inputEvent;
    unsigned int bytesRead = read(fd, &inputEvent, sizeof(inputEvent));

    if(bytesRead < sizeof(input_event))
    {
        return;
    }

    if(inputEvent.type==EV_KEY)
    {
        if(inputEvent.value==2)
        {
            return;
        }
        ui->label_4->setText(QString::number(inputEvent.code));
        if(inputEvent.value == 0)
        {
            ui->label_10->setText("release");
        }
        else
        {
            ui->label_10->setText("press");
        }
    }

}

void MainWindow::get_switch_key_value()
{
    char switchbuttons[8]={'0','0','0','0','0','0','0','0'};
    if(read(key_switch_fd_,switchbuttons,sizeof(switchbuttons))!=sizeof(switchbuttons))
    {
        return;
    }
    QString CurMode = "";
    for(unsigned int i=0;i<sizeof(switchbuttons);i++)
    {
        CurMode.append(QString(QChar(switchbuttons[i])));
    }
    ui->label_4->setText(CurMode);
}

void MainWindow::get_key_enable_value()
{
    char enablebuttons[8]={'1','1','1','1','1','1','1','1'};
    if(read(switch_enablekey_fd_,enablebuttons,sizeof(enablebuttons))!=sizeof(enablebuttons))
    {
        return;
    }

    QString CurEnable = "";
    for(unsigned int i=0;i<sizeof(enablebuttons);i++)
    {
        CurEnable.append(QString(QChar(enablebuttons[i])));
    }
    ui->label_4->setText(CurEnable);
}
