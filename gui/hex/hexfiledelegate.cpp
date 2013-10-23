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

#include <QBrush>
#include <QPainterPath>
#include <iostream>

#include "gui/hex/hexfiledelegate.h"

HexFileDelegate::HexFileDelegate(unsigned int hexWidth, unsigned int charWidth)
    : headerWidth(0),
      hexWidth(hexWidth),
      charWidth(charWidth)
{
}

void HexFileDelegate::setHeaderWidth(unsigned int value)
{
    headerWidth = value;
}

const QPen HexFileDelegate::thickSelectionPen = QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
const QPen HexFileDelegate::thinSelectionPen  = QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

bool hl(QModelIndex index)
{
    return index.isValid() && index.data(Qt::UserRole).toBool();
}

void HexFileDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //QStyledItemDelegate::paint(painter, option, index);
    if(index.isValid())
    {
        const int column = index.column();
        const int row = index.row();

        QString text = index.data().toString();
        QRect textRect;
        if(column == 0)
            textRect = painter->boundingRect(option.rect, 0, QString("0").repeated(text.size()));
        else
            textRect = painter->boundingRect(option.rect, 0, text);

        unsigned int width;
        if(column == 0)
            width = headerWidth;
        else if(column < 17)
            width = hexWidth;
        else
            width = charWidth;

        bool highlight = index.data(Qt::UserRole).toBool();

        QRect hlRect = option.rect;
        if(column == 16)
            hlRect.setWidth(width);



        QRect boundingRect = option.rect;
        boundingRect.setWidth(width);

        if(highlight)
            painter->fillRect(hlRect, QBrush(QColor(0xa0,0xa0,0xff,0x50)));


        QBrush backgroundBrush = index.data(Qt::BackgroundRole).value<QBrush>();
        if(backgroundBrush.color()!=Qt::white)
            painter->fillRect(boundingRect, backgroundBrush);

        QBrush foreground = index.data(Qt::ForegroundRole).value<QBrush>();

        painter->setPen(QPen(foreground.color(),0,Qt::SolidLine));

        QPoint textPosition;
        if(text == "m" || text == "M" || text == "w" || text == "W")
            textPosition = QPoint(option.rect.x() - 1,
                            option.rect.y() + option.rect.height() - (option.rect.height() - textRect.height())/2);
        else
            textPosition = QPoint(option.rect.x() + (width - textRect.width())/2,
                            option.rect.y() + option.rect.height() - (option.rect.height() - textRect.height())/2);

        if(text != "")
        {
            if(text == "sq")
                painter->fillRect(QRect(option.rect.left() + width/2 - 1, option.rect.top() + option.rect.height()/2 + 3, 2, 2), foreground.color());
            else
                painter->drawText(textPosition, index.data().toString());
        }

        QModelIndex leftCell   = (column !=  1  && column != 17)? index.sibling(row, column - 1) : QModelIndex();
        QModelIndex rightCell  = (column !=  16 && column != 32)? index.sibling(row, column + 1) : QModelIndex();
        QModelIndex topCell    = index.sibling(row - 1, column);
        QModelIndex bottomCell = index.sibling(row + 1, column);

        bool leftHl   = hl(leftCell);
        bool rightHl  = hl(rightCell);
        bool topHl    = hl(topCell);
        bool bottomHl = hl(bottomCell);



        if((highlight)? !leftHl : leftHl)
        {
            if(leftCell.isValid())
                painter->setPen(thinSelectionPen);
            else
                painter->setPen(thickSelectionPen);

            painter->drawLine(hlRect.topLeft(), hlRect.bottomLeft());
        }

        if((highlight)? !rightHl : rightHl)
        {
            if(rightCell.isValid())
                painter->setPen(thinSelectionPen);
            else
                painter->setPen(thickSelectionPen);

            painter->drawLine(hlRect.topRight(), hlRect.bottomRight());
        }

        if((highlight)? !topHl : topHl)
        {
            if(topCell.isValid())
                painter->setPen(thinSelectionPen);
            else
                painter->setPen(thickSelectionPen);

            painter->drawLine(hlRect.topLeft(), hlRect.topRight());
        }

        if((highlight)? !bottomHl : bottomHl)
        {
            if(bottomCell.isValid())
                painter->setPen(thinSelectionPen);
            else
                painter->setPen(thickSelectionPen);

            painter->drawLine(hlRect.bottomLeft(), hlRect.bottomRight());
        }
    }
}

