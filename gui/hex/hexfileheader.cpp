//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <QPainter>
#include <QApplication>
#include <QLayout>
#include <iostream>

#include "core/util/strutil.h"
#include "gui/hex/hexfileheader.h"

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
