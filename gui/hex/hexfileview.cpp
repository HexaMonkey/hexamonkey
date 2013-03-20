#include "hexfileview.h"
#include <QKeyEvent>
#include <QHeaderView>
#include <QScrollBar>
#include <iostream>

HexFileView::HexFileView(QWidget *parent) :
    QTableView(parent),
    _headerWidth(0),
    _hexWidth(fontMetrics().width("bd")+2),
    _charWidth(fontMetrics().width("m")+1),
    _headerSeparationWidth(15),
    _hexSeparationWidth(6),
    _hexCharSeparationWidth(15),
    _delegate(_hexWidth, _charWidth)
{
    setFocusPolicy(Qt::StrongFocus);
    setSelectionMode( QAbstractItemView::SingleSelection );
    setShowGrid(false);
    horizontalHeader()->hide();
    verticalHeader()->hide();
    setItemDelegate(&_delegate);
}

void HexFileView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
    setColumnWidths();
}

void HexFileView::setHeaderCharCount(int value)
{
    _headerWidth = fontMetrics().width(QString("0").repeated(value))+4;
    _delegate.setHeaderWidth(_headerWidth);
    setColumnWidths();
}

unsigned int HexFileView::headerWidth()
{
    return _headerWidth;
}

unsigned int HexFileView::hexWidth()
{
    return _hexWidth;
}

unsigned int HexFileView::charWidth()
{
    return _charWidth;
}

void HexFileView::setColumnWidths()
{
    setColumnWidth(0, _headerWidth + _headerSeparationWidth);

    //Set the column width
    for(int i = 0; i<16; ++i)
    {
        unsigned int width = _hexWidth;
        if(i == 15)
            width += _hexCharSeparationWidth;
        else if(i%4==3)
            width += _hexSeparationWidth;

        setColumnWidth(i+1,width);
        setColumnWidth(i+17,_charWidth);
    }
    setFixedWidth(_headerWidth
                    + _headerSeparationWidth
                    + 16*_hexWidth
                    + 3*_hexSeparationWidth
                    + _hexCharSeparationWidth
                    + 16*_charWidth
                    + 32);
}

void HexFileView::keyPressEvent ( QKeyEvent * event )
{
    if (
        ( event->matches(QKeySequence::SelectNextWord) ||
          event->matches(QKeySequence::SelectNextChar)  ||
          event->matches(QKeySequence::Forward)  ||
          event->matches(QKeySequence::MoveToNextWord) ||
          event->matches(QKeySequence::MoveToNextChar))
            && (currentIndex().column() == 16 || currentIndex().column() == 32))
    {
        if(currentIndex().sibling(currentIndex().row()+1,0).isValid())
        {
            setCurrentIndex(currentIndex().sibling(currentIndex().row()+1,1));
        }
    }
    else if (
             (
                 event->matches(QKeySequence::SelectPreviousWord) ||
                 event->matches(QKeySequence::SelectPreviousChar)  ||
                 event->matches(QKeySequence::MoveToPreviousWord) ||
                 event->matches(QKeySequence::MoveToPreviousChar) )
             && (currentIndex().column() == 1 || currentIndex().column() == 17))
    {
        if(currentIndex().sibling(currentIndex().row()-1,1).isValid())
        {
            setCurrentIndex(currentIndex().sibling(currentIndex().row()-1,16));
        }
    }
    else
        QAbstractItemView::keyPressEvent ( event );
}

QSize HexFileView::sizeHint () const
{

    QSize size = QTableView::sizeHint();
    size.setWidth( _headerWidth
                   + _headerSeparationWidth
                   + 16*_hexWidth
                   + 3*_hexSeparationWidth
                   + _hexCharSeparationWidth
                   + 16*_charWidth
                   + 32);
    return size;
}

void HexFileView::currentChanged(const QModelIndex &current, const QModelIndex & previous)
{
    QTableView::currentChanged(current, previous);
    emit selected(current);
}

void HexFileView::focusInEvent (QFocusEvent *event)
{
    QTableView::focusInEvent(event);
    emit focusedIn();
}

void HexFileView::focusOutEvent(QFocusEvent *event)
{
    QTableView::focusOutEvent(event);
    emit focusedOut();
}

void HexFileView::windowActivationChange (bool oldActive)
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
