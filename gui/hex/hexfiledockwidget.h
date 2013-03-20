#ifndef HEXFILEDOCKWIDGET_H
#define HEXFILEDOCKWIDGET_H

#include <QDockWidget>
#include "hexfilewidget.h"

/*!
 * \brief The HexFileDockWidget class
 */
class HexFileDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit HexFileDockWidget(QWidget *parent = 0);
    void setWidget(HexFileWidget *widget);

    HexFileWidget* widget;

signals:
    void focusedIn();
    void focusedOut();
    
public slots:

protected:
    void focusInEvent (QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void windowActivationChange(bool oldActive);
    
};

#endif // HEXFILEDOCKWIDGET_H
