#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->widget, SIGNAL(positionChanged(int, int)),
            this, SLOT(updateTime(int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->widget->stop();
    QStringList files = QFileDialog::getOpenFileNames(this, "play list", "/home/Qt",
              "Videos (*.mp4 *.avi *.flv *.rmpv *.mwv *.mkv);;Audios (*.mp3 *.mwa)");
    if (files.count() == 1) {
        ui->widget->play(files[0]);
    } else if (files.count() > 1) {
        ui->widget->play(files);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->widget->pause();
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->widget->mute();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->widget->previous();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->widget->next();
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    ui->widget->seek(ui->horizontalSlider->value());
}

void MainWindow::updateTime(int tmcurlen, int tmtotlen)
{
    int hourTot = tmtotlen / 3600;
    int minuteTot = tmtotlen % 3600 / 60;
    int secondTot = tmtotlen % 60;

    int hourCur = tmcurlen / 3600;
    int minuteCur = tmcurlen % 3600 / 60;
    int secondCur = tmcurlen % 60;

    int perSlider = 100 * tmcurlen / tmtotlen;

    QString tmpstr;
    if (hourCur < 10) {
        tmpstr += QString("0");
    }
    tmpstr += QString(QString::number(hourCur) + ":");
    if (minuteCur < 10) {
       tmpstr += QString("0");
    }
    tmpstr += QString(QString::number(minuteCur) + ":");
    if (secondCur < 10) {
       tmpstr += QString("0");
    }
    tmpstr += QString(QString::number(secondCur));
    tmpstr += QString("/");

    if (hourTot < 10) {
       tmpstr += QString("0");
    }
    tmpstr += QString(QString::number(hourTot) + ":");
    if (minuteTot < 10) {
       tmpstr += QString("0");
    }
    tmpstr += QString(QString::number(minuteTot) + ":");
    if (secondTot < 10) {
       tmpstr += QString("0");
    }
    tmpstr += QString(QString::number(secondTot));

    //QString tmpstr = QString(QString("%1:%2:%3").arg(hourCur).arg(minuteCur).arg(secondCur) + "/" + QString("%1:%2:%3").arg(hourTot).arg(minuteTot).arg(secondTot));
    ui->label->setText(tmpstr);
    ui->horizontalSlider->setValue(perSlider);
}


