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

#ifndef HEXFILEHEADER_H
#define HEXFILEHEADER_H

#include "gui/hex/hexfileview.h"

/**
 * @brief Header for the \link HexFileWidget hex widget\endlink
 *
 * Display the byte numbers modulo 16 as hexadecimal digits.
 */
class HexFileHeader : public QWidget
{
    Q_OBJECT
public:
    explicit HexFileHeader(HexFileView* view, QWidget *parent = 0);

    virtual void paintEvent(QPaintEvent *);
    void setHighlight(int position);
    void focusIn();
    void focusOut();
    
signals:
    
public slots:

private:
    HexFileView* _view;
    int _height;
    int _hexWidth;
    int _charWidth;
    int _hlPosition;
    bool _hasFocus;
    
};

#endif // HEXFILEHEADER_H
