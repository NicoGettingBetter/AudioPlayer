#include "listwidget.hpp"

ListWidget::ListWidget(QWidget *parent):
    QListWidget(parent)
{

}

ListWidget::~ListWidget()
{

}

void ListWidget::dropEvent(QDropEvent *event)
{
    QListWidget::dropEvent(event);
    emit dropped();
}
