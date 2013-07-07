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

#ifndef HEXFILEVIEW_H
#define HEXFILEVIEW_H

#include <QTableView>
#include "hexfiledelegate.h"

/**
 * @brief Widget managing the display of the table in the \link HexFileWidget hex widget\endlink
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
