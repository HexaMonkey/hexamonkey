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

#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>
#include <QWidget>
#include "hexfilemodel.h"
#include "hexfileview.h"
#include "hexfiledelegate.h"
#include "hexfileheader.h"
#include "hexfilesearchwidget.h"

/**
 * @brief Widget responsible for displaying the content of the file
 *
 * The content is displayed as a table with bytes written in
 * hexidecimal on the 16 first cells of a line and in ASCII on
 * the 16 last cells of a line.
 *
 * The current file is displayed, and the area of the file corresponding
 * to the \link Object object\endlink of the current \link TreeObjectItem
 * item \endlink the \link TreeWidget tree\endlink is highlighted.
 *
 * The hex widget is managed by two main classes : the HexFileModel responsible
 * for managing the data model of the table and the HexFileView responsible
 * for displaying it.
 *
 * The hex widget also contains a \link HexFileSearchWidget search widget\endlink that
 * allows the search for strings.
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
