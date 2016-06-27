#include "videowidget.h"
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QFile>
#include <QMessageBox>
#include <QDebug>


videoWidget::videoWidget(QWidget *parent) :
    QWidget(parent)
{
    mplayer_process = new QProcess(this);
    mplayer_path = "/usr/bin/mplayer";

    play_status = false;
    mute_status = false;
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(requestPos()));
    connect(mplayer_process, SIGNAL(readyRead()), this, SLOT(readPos()));
}

videoWidget::~videoWidget()
{
    delete mplayer_process;
}

void videoWidget::paintEvent(QPaintEvent *)
{
    QStyleOption op;
    op.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &op, &p, this);
    //this->setStyleSheet("background-color:black;");
}

void videoWidget::setMplayerPath(QString path)
{
    mplayer_path = path;
}

void videoWidget::pause()
{
    if (play_status == true) {
        play_status = false;
        timer->stop();
    } else {
        play_status = true;
        timer->start(1000);
    }
    mplayer_process->write("pause\n");
}

void videoWidget::mute()
{
    if (mute_status == true) {
        mute_status = false;
        mplayer_process->write("mute 1\n");
    } else {
        mute_status = true;
        mplayer_process->write("mute 0\n");
    }
}

void videoWidget::next()
{
    mplayer_process->write("pt_step 1\n");
}

void videoWidget::previous()
{
    mplayer_process->write("pt_step -1\n");
}

void videoWidget::seek(int pos)
{
    mplayer_process->write(QString("seek "+QString::number(pos)+" 1\n").toLocal8Bit());
}

void videoWidget::play(QString video)
{
    QFile tmpfile(video);
    if (!tmpfile.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, "Error", "file does not exist!!!");
        return;
    }
    currrent_mediafile = video;
    QStringList arguments;
    arguments << "-slave" << "-quiet";
    arguments << "-wid" << QString::number(this->winId());

    arguments <<video;
    mplayer_process->start(mplayer_path, arguments);    //run another program
    mplayer_process->waitForStarted(20000);
    play_status = true;

    timer->start(1000);
}

void videoWidget::play(QStringList filelist)
{
    QStringList arguments;

    arguments << "-slave" << "-quiet";

    QFile tmpfile("videolist");
    tmpfile.open(QIODevice::WriteOnly);
    for (int i = 0; i < filelist.count(); ++i) {
        tmpfile.write(filelist[i].toLocal8Bit());
        tmpfile.write("\n");
    }
    tmpfile.close();

    arguments << "-wid" << QString::number(this->winId());
    arguments << "-playlist" << "videolist";

    mplayer_process->start(mplayer_path, arguments);
    mplayer_process->waitForStarted(20000);
    play_status = true;
    timer->start(1000);
}

void videoWidget::stop()
{
    mplayer_process->kill();
    mplayer_process->waitForFinished(2000);
    play_status = false;
    timer->stop();
}

void videoWidget::requestPos()
{
    mplayer_process->write("get_time_pos\n");
    mplayer_process->write("get_time_length\n");
}

void videoWidget::readPos()
{
    QString tmpstr = mplayer_process->readAll();
    //qDebug() << tmpstr;
    static int timetotlen = 1;
    static int timecurlen;

    QStringList tmpstrlist;
    if (tmpstr.contains("ANS_TIME_POSITION=") && tmpstr.contains("ANS_LENGTH=")) {
        tmpstr.chop(1);
        tmpstrlist = tmpstr.split("\n");
        if (tmpstrlist[0].contains("ANS_LENGTH=")) {
            timetotlen = (int)(tmpstrlist[0].remove("ANS_LENGTH=").toDouble());
        } else {
            timecurlen = (int)(tmpstrlist[1].remove("ANS_TIME_POSITION=").toDouble());
        }

        if (tmpstrlist[0].contains("ANS_TIME_POSITION=")) {
            timecurlen = (int)(tmpstrlist[0].remove("ANS_TIME_POSITION=").toDouble());
        } else {
            timetotlen = (int)(tmpstrlist[1].remove("ANS_LENGTH=").toDouble());
        }
        emit positionChanged(timecurlen, timetotlen);
    }
    else if (tmpstr.contains("ANS_TIME_POSITION=") || tmpstr.contains("ANS_LENGTH=")) {
        tmpstr.chop(1);
        if (tmpstr.contains("ANS_LENGTH=")) {
            timetotlen = (int)(tmpstr.remove("ANS_LENGTH=").toDouble());
        } else if (tmpstr.contains("ANS_TIME_POSITION=")) {
            timecurlen = (int)(tmpstr.remove("ANS_TIME_POSITION=").toDouble());
        }
        emit positionChanged(timecurlen, timetotlen);
    }
}
