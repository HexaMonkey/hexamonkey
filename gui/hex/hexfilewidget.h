#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>
#include <QWidget>
#include "hexfilemodel.h"
#include "hexfileview.h"
#include "hexfiledelegate.h"
#include "hexfileheader.h"
#include "hexfilesearchwidget.h"

class HexFileDockWidget;

/*!
 * \brief The HexFileWidget class
 */
class HexFileWidget : public QWidget
{
    Q_OBJECT



public:
    HexFileWidget(QWidget *parent = 0);

    HexFileView *view;
    HexFileHeader *header;
    HexFileSearchWidget *searchWidget;

    unsigned int hexWidth() const;
    unsigned int charWidth() const;

signals:
    void focusedIn();
    void focusedOut();

public slots:
    void setFile(const QString& path);
    void gotoPosition(qint64 position);
    void selectPosition(qint64 position);
    void highlight(qint64 position, qint64 size);

    void focus(const QModelIndex& index);
    void focus(qint64 position);

    void focusIn();
    void focusOut();

    void search(QByteArray pattern, bool next);
    void focusSearch();

protected:
    void focusInEvent (QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void windowActivationChange(bool oldActive);

private:
    HexFileModel* model;
    QScrollBar*   scrollBar;

    unsigned int _hexWidth;
    unsigned int _charWidth;
};

#endif
