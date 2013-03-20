#include "hexfiledockwidget.h"

HexFileDockWidget::HexFileDockWidget(QWidget *parent) :
    QDockWidget(tr("hex"), parent)
{
    setFeatures(QDockWidget::DockWidgetMovable
               |QDockWidget::DockWidgetFloatable
               |QDockWidget::DockWidgetClosable);
    setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    resize(0,0);
}


void HexFileDockWidget::setWidget(HexFileWidget *widget)
{
    QDockWidget::setWidget(widget);
    this->widget = widget;

    connect(this,SIGNAL(focusedIn()),this->widget, SLOT(focusIn()));
    connect(this,SIGNAL(focusedOut()),this->widget, SLOT(focusOut()));
}

void HexFileDockWidget::focusInEvent (QFocusEvent *event)
{
    QDockWidget::focusInEvent(event);
    emit focusedIn();
}

void HexFileDockWidget::focusOutEvent(QFocusEvent *event)
{
    QDockWidget::focusOutEvent(event);
    emit focusedOut();
}

void HexFileDockWidget::windowActivationChange (bool oldActive)
{
    if(!oldActive)
    {
        emit focusedIn();
    }
    else
    {
        emit focusedOut();
    }
}
