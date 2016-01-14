#include "player.hpp"

#include <QFileDialog>
#include <QFileInfo>
#include <QMediaPlayer>

Player::Player(QWidget *parent)
    : QWidget(parent),
      mPlayer(new QMediaPlayer),
      mPlaylist(new QMediaPlaylist),
      nameOfPlayer(new QLabel("NicoPlayer", this)),
      name(new QLabel(this)),
      closeButton(new QPushButton(this)),
      minmaxButton(new QPushButton(this)),
      minButton(new QPushButton(this)),
      addButton(new QPushButton(this)),
      stopButton(new QPushButton(this)),
      playButton(new QPushButton(this)),
      pauseButton(new QPushButton(this)),
      nextButton(new QPushButton(this)),
      prevButton(new QPushButton(this)),
      listButton(new QPushButton(this)),
      listWidget(new ListWidget(this)),
      volume(new QSlider(Qt::Horizontal, this)),
      volumePic(new QLabel(this)),
      route(new QSlider(Qt::Horizontal, this))
{
    drawWidget();

    connect(minButton, SIGNAL(clicked()), SLOT(showMinimized()));
    connect(closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(minmaxButton, SIGNAL(clicked()), SLOT(minmaxWindow()));
    connect(addButton, SIGNAL(clicked()), SLOT(addSong()));
    connect(playButton, SIGNAL(clicked()), SLOT(play()));
    connect(stopButton, SIGNAL(clicked()), SLOT(stop()));
    connect(pauseButton, SIGNAL(clicked()), SLOT(pause()));
    connect(nextButton, SIGNAL(clicked()), SLOT(next()));
    connect(prevButton, SIGNAL(clicked()), SLOT(prev()));
    connect(listButton, SIGNAL(clicked()), SLOT(showList()));
    connect(volume, SIGNAL(valueChanged(int)), SLOT(changeVolume(int)));
    connect(mPlayer, SIGNAL(positionChanged(qint64)), SLOT(changeValueOfRoute(qint64)));
    connect(route, SIGNAL(sliderReleased()), SLOT(changePositionOfSong()));
    connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(changeSong(QListWidgetItem*)));
    connect(listWidget, SIGNAL(itemActivated(QListWidgetItem*)),SLOT(changeSong(QListWidgetItem*)));
    connect(listWidget, SIGNAL(dropped()), SLOT(replaceSong()));
    connect(mPlaylist, SIGNAL(currentMediaChanged(QMediaContent)), SLOT(songChanged()));
}

Player::~Player()
{
    delete mPlayer;
    delete mPlaylist;
    delete name;
    delete addButton;
    delete stopButton;
    delete playButton;
    delete pauseButton;
    delete nextButton;
    delete prevButton;
    delete listButton;
    delete listWidget;
    delete volume;
    delete volumePic;
    delete route;
    delete nameOfPlayer;
    delete closeButton;
    delete minmaxButton;
    delete minButton;
}

void Player::addSong()
{
    QStringList filePathes = QFileDialog::getOpenFileNames(this,
        "Select one or more files to open",
        "../",
        "Music (*.aac   *.aif   *.aiff  *.aob   *.ape   *.asf   *.aud   *.bin\
        *.bwg   *.cdr   *.flac  *.gpx   *.ics   *.iff   *.m     *.m3u   *.m3u8\
        *.m4a   *.m4b   *.m4p   *.m4r   *.mid   *.midi  *.mod   *.mp3   *.mpa\
        *.mpp   *.msv   *.mts   *.nkc   *.ogg   *.ps    *.ra    *.sdf   *.sib\
        *.sln   *.spl   *.srt   *.srt   *.temp  *.vb    *.wav   *.wave  *.wm\
        *.wma   *.wpd");

    foreach (auto filePath, filePathes)
    {
        if(!filePath.isEmpty())
        {
            QUrl adress(QFileInfo(filePath).absoluteFilePath());
            bool doubleUsage = false;
            for (int i = 0; i < mPlaylist->mediaCount(); ++i)
                if (mPlaylist->media(i) == QMediaContent(adress))
                    doubleUsage = true;
            if (!doubleUsage)
                mPlaylist->addMedia(QMediaContent(adress));
        }
    }
    for (int i = 0; i < mPlaylist->mediaCount(); ++i)
        listWidget->addItem(mPlaylist->media(i).canonicalUrl().fileName());
    play();
}

void Player::play()
{
    if (!mPlaylist->isEmpty())
    {
        mPlayer->play();
        playButton->hide();
        pauseButton->show();
    }
}

void Player::pause()
{
    mPlayer->pause();
    playButton->show();
    pauseButton->hide();
}

void Player::stop()
{
    mPlayer->stop();
    playButton->show();
    pauseButton->hide();
}

void Player::next()
{
    mPlaylist->next();
}

void Player::prev()
{
    mPlaylist->previous();
}

void Player::showList()
{
    if (height() > 100)
    {
        setGeometry(pos().rx(), pos().ry(), width(), 100);
        listButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarShadeButton));
    }
    else
    {
        setGeometry(pos().rx(), pos().ry(), width(), 400);
        listButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarUnshadeButton));
    }
}

void Player::changeVolume(int vol)
{
    mPlayer->setVolume(vol);
}

void Player::changeValueOfRoute(qint64 value)
{
    route->setValue(static_cast<double>(value)/mPlayer->duration()*1000);
}

void Player::changePositionOfSong()
{
    mPlayer->setPosition(static_cast<double>(route->value())*mPlayer->duration()/1000);
}

void Player::changeSong(QListWidgetItem *item)
{
    mPlaylist->setCurrentIndex(item->listWidget()->currentRow());
    play();
}

void Player::replaceSong()
{
    int pos = mPlayer->position();
    QMediaContent current = mPlayer->currentMedia();
    QList<QMediaContent> *temp = new QList<QMediaContent>();
    for (auto i = 0; i < mPlaylist->mediaCount(); ++i)
        temp->push_back(mPlaylist->media(i));
    mPlaylist->removeMedia(0, mPlaylist->mediaCount()-1);
    for (auto i = 0; i < listWidget->count(); ++i)
    {
        QMediaContent tempMedia;
        for (int j = 0; j < temp->count(); ++j)
        {
            if (listWidget->item(i)->text() == temp->at(j).canonicalUrl().fileName())
            {
                tempMedia = temp->at(j);
                temp->removeAt(j);
            }
        }
        mPlaylist->insertMedia(i, tempMedia);
    }
    for (auto i = 0; i < mPlaylist->mediaCount(); ++i)
        if (mPlaylist->media(i) == current)
            mPlaylist->setCurrentIndex(i);
    mPlayer->setPosition(pos);
    play();
    delete temp;
}

void Player::songChanged()
{
    name->setText(mPlaylist->currentMedia().canonicalUrl().fileName());
    if (mPlayer->state() == QMediaPlayer::StoppedState)
        stop();
}

void Player::minmaxWindow()
{
    if (isMaximized())
        showNormal();
    else
        showMaximized();
}

void Player::paintEvent(QPaintEvent *)
{
    nameOfPlayer->setGeometry((width() - 194)/2, 2, 100, 20);
    closeButton->setGeometry(width() - 32, 2, 30, 10);
    minmaxButton->setGeometry(width() - 62, 2, 30, 10);
    minButton->setGeometry(width() - 92, 2, 30, 10);
    route->setGeometry(35, 75, width() - 45, 20);
    listWidget->setGeometry(10, 100, width() - 20, height() - 110);
    for (int i = 0; i < listWidget->count(); ++i)
        listWidget->item(i)->setSizeHint(QSize(width() - 45, 15));    
    name->setGeometry(10, 20, width() - 20, 20);
}

void Player::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Delete:
        if (mPlaylist->media(listWidget->currentRow()) == mPlaylist->currentMedia())
        {
            mPlaylist->removeMedia(listWidget->currentRow());
            listWidget->takeItem(listWidget->currentRow());
        } else
        {
            int pos = mPlayer->position();
            mPlaylist->removeMedia(listWidget->currentRow());
            if (mPlaylist->isEmpty())
                stop();
            mPlayer->setPosition(pos);
            listWidget->takeItem(listWidget->currentRow());
        }
        break;
    }
}

void Player::drawWidget()
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    QPalette p;
    p.setColor(QPalette::Window, QColor(30, 30, 30));
    p.setColor(QPalette::Button, QColor(85, 85, 85));
    p.setColor(QPalette::Highlight, QColor(0, 200, 50));
    p.setColor(QPalette::ButtonText, QColor(0, 255, 50));
    p.setColor(QPalette::WindowText, QColor(0, 255, 50));
    p.setColor(QPalette::Base, QColor(35, 35, 35));
    p.setColor(QPalette::Text, QColor(155, 200, 155));
    setPalette(p);

    setGeometry(100, 50, 300, 400);
    setMinimumSize(300, 100);
    mPlayer->setPlaylist(mPlaylist);
    listWidget->setDragDropMode(QAbstractItemView::DragDrop);
    listWidget->setDefaultDropAction(Qt::MoveAction);
    nameOfPlayer->setPalette(QColor(0, 0, 0));
    nameOfPlayer->setFont(QFont("Serif", 12));

    closeButton->setPalette(QPalette(QColor(255, 0, 0)));
    minmaxButton->setPalette(QPalette(QColor(255, 255, 0)));
    minButton->setPalette(QPalette(QColor(0, 255, 0)));
    addButton->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    addButton->setGeometry(10, 45, 30, 25);
    addButton->show();
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playButton->setGeometry(40, 40, 30, 30);
    playButton->show();
    pauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    pauseButton->setGeometry(playButton->geometry());
    pauseButton->hide();
    stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    stopButton->setGeometry(70, 45, 30, 25);
    prevButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    prevButton->setGeometry(100, 45, 30, 25);
    nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    nextButton->setGeometry(130, 45, 30, 25);
    listButton->setGeometry(10, 78, 20, 14);
    listButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarUnshadeButton));

    mPlayer->setVolume(50);
    volume->setGeometry(190, 45, 100, 25);
    volume->setValue(50);
    volume->setMaximum(100);
    volume->setMinimum(0);
    volumePic->setPixmap(style()->standardIcon(QStyle::SP_MediaVolume).pixmap(QSize(20, 20)));
    volumePic->setGeometry(170, 47, 20, 20);

    route->setValue(0);
    route->setMinimum(0);
    route->setMaximum(1000);
    route->setPageStep(10);
}

void Player::mousePressEvent(QMouseEvent *event)
{
    if (   !isMaximized() && ((event->pos().rx() > 0 && event->pos().ry() > 0 &&
            event->pos().rx() < width() - 90 && event->pos().ry() < 20) ||
            (event->pos().rx() > width() - 90 && event->pos().rx() > 10 &&
            event->pos().rx() < width() && event->pos().rx() < 20)))
    {
        mpos = event->pos();
        fmpos = true;
    }
    else
        fmpos = false;
}

void Player::mouseMoveEvent(QMouseEvent *event)
{
    if (fmpos && event->buttons() == Qt::LeftButton)
    {
        QPoint diff = event->pos() - mpos;
        QPoint newpos = pos() + diff;
        move(newpos);
    }
}
