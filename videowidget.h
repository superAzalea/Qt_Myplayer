#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QTimer>

class videoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit videoWidget(QWidget *parent = 0);
    ~videoWidget();

    void setMplayerPath(QString);   //allow user to choose where the video application of mplayer residents
signals:
    void positionChanged(int, int);  //don't need to be implemented. report the status of video while it's running to the process slide bar
                                //so, we can connect this signal to our selfdefined slot to catch this status. and we can set the position of slide bar

protected:
    void paintEvent(QPaintEvent *);

public slots:
    void pause();   //pause video playing
    void mute();   //enable volum silent
    void next();    //play next video
    void previous();    //play previous video
    void seek(int); //set the playing position
    void play(QString); //choose one video play from local files
    void play(QStringList); //choose more than one video play form local files
    void stop();    //terminate playing
    void requestPos();
    void readPos();
private:
    QProcess *mplayer_process;
    QString mplayer_path;   //the path of mplayer installed in local host
    QString currrent_mediafile; //default video, stored on behalf of last played video
    QStringList list_mediafiles;    //accepted videos
    bool play_status;
    bool mute_status;
    QTimer *timer;
};

#endif // VIDEOWIDGET_H
