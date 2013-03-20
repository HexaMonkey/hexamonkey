#include "hexfileheader.h"
#include "strutil.h"
#include <QPainter>
#include <QApplication>
#include <QLayout>


#include <iostream>
HexFileHeader::HexFileHeader(HexFileView* _view, QWidget *parent) :
    QWidget(parent),
    _view(_view),
    _height(20),
    _hexWidth(_view->hexWidth()),
    _charWidth(_view->charWidth()),
    _hlPosition(-1),
    _hasFocus(false)
{
    setFixedHeight(_height);
    setContentsMargins(0,0,0,0);
}

void HexFileHeader::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int x = 1;
    int y = _height;
    for(int column=0; column<33; ++column)
    {
        painter.setPen(QPen(qApp->palette().foreground(),0,Qt::SolidLine));
        if(column>=1)
        {
            int width = (column<17)? _hexWidth : _charWidth;

            if((column-1)%16 == _hlPosition)
            {
                QBrush brush;
                if(_hasFocus)
                {
                    brush = qApp->palette().highlight();
                    painter.setPen(QPen(qApp->palette().highlightedText(),0,Qt::SolidLine));
                }
                else
                {
                    brush = qApp->palette().dark();
                }

                painter.fillRect(QRect(x,0,width,_height), brush);
            }

            QString text = toHex((column-1)%16, 1).c_str();
            QRect textRect = painter.boundingRect(x, y, width, _height, 0, text);
            painter.drawText(x + (width   - textRect.width())/2,
                             y - (_height - textRect.height())/2,
                             text);
        }
        x += _view->columnWidth(column);
    }
}

void HexFileHeader::setHighlight(int position)
{
    _hlPosition = position;
    repaint();
}

void HexFileHeader::focusIn()
{
    _hasFocus = true;
    repaint();
}

void HexFileHeader::focusOut()
{
    _hasFocus = false;
    repaint();
}
