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

#ifndef HEXFILESEARCHWIDGET_H
#define HEXFILESEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QRadioButton>
#include <QPushButton>
#include <QByteArray>

/**
 * @brief Wigdet responsible for entering a search string
 *
 * The string can either be entered as ASCII or hexadecimal.
 */

class HexFileSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HexFileSearchWidget(QWidget *parent = 0);
    
signals:
    void searchRequested(QByteArray, bool);

public slots:
    void reset();
    void focusSearch();

private slots:
    void search();

private:
    QLineEdit* lineEdit;
    QRadioButton* hexButton;
    QRadioButton* asciiButton;
    QPushButton* nextButton;

    QByteArray pattern;
};

#endif // HEXFILESEARCHWIDGET_H
