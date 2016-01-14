#ifndef LISTWIDGET_HPP
#define LISTWIDGET_HPP

#include <QListWidget>



class ListWidget : public QListWidget
{
    Q_OBJECT

public:
    ListWidget(QWidget *parent = 0);
    ~ListWidget();
    void dropEvent(QDropEvent *);

signals:
    void dropped();

};

#endif // LISTWIDGET_HPP
