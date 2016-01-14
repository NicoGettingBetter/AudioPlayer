#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QDebug>

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <listwidget.hpp>
#include <QKeyEvent>
#include <QLabel>

class Player : public QWidget
{
    Q_OBJECT

public:
    Player(QWidget *parent = 0);
    ~Player();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void addSong();
    void play();
    void pause();
    void stop();
    void next();
    void prev();
    void showList();
    void changeVolume(int);
    void changeValueOfRoute(qint64);
    void changePositionOfSong();
    void changeSong(QListWidgetItem*);
    void replaceSong();
    void songChanged();
    void minmaxWindow();

public slots:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    void drawWidget();

    QPoint mpos;
    bool fmpos;
    QMediaPlayer *mPlayer;
    QMediaPlaylist *mPlaylist;
    QLabel *nameOfPlayer;
    QLabel *name;
    QPushButton *closeButton;
    QPushButton *minmaxButton;
    QPushButton *minButton;
    QPushButton *addButton;
    QPushButton *stopButton;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QPushButton *listButton;
    ListWidget *listWidget;
    QSlider *volume;
    QLabel *volumePic;
    QSlider *route;
};

#endif // PLAYER_HPP
