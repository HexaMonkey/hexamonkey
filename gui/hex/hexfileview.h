#ifndef HEXFILEVIEW_H
#define HEXFILEVIEW_H

#include <QTableView>
#include "hexfiledelegate.h"

/*!
 * \brief The HexFileView class
 */
class HexFileView : public QTableView
{
    Q_OBJECT
public:
    explicit HexFileView(QWidget *parent = 0);

    void setModel(QAbstractItemModel *model);
    void setHeaderCharCount(int value);
    void setColumnWidths();


    unsigned int headerWidth();
    unsigned int hexWidth();
    unsigned int charWidth();

    virtual QSize sizeHint () const;
    
signals:
    void selected(QModelIndex);
    void focusedIn();
    void focusedOut();

public slots:

protected:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void focusInEvent (QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void windowActivationChange(bool oldActive);
    void keyPressEvent ( QKeyEvent * event );

private:
    unsigned int _headerWidth;
    unsigned int _hexWidth;
    unsigned int _charWidth;

    unsigned int _headerSeparationWidth;
    unsigned int _hexSeparationWidth;
    unsigned int _hexCharSeparationWidth;

    HexFileDelegate _delegate;

    
};

#endif // HEXFILEVIEW_H
