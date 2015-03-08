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

#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>

#include <string>

/**
 * @brief Widget responsible for entering the expression for \link Filter filters\endlink
 *
 * The filters are used by the \link TreeWidget tree widget\endlink to filter the nodes
 * displayed by evaluating an HMDL evaluation on their \link Object object\endlink.
 */
class FilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FilterWidget(QWidget *parent = 0);
    
private:
    QLineEdit* lineEdit;

signals:
    void textChanged(QString text);

public slots:
    void changeText(QString text);
    void invalidateText();
    void neutralizeText();

private slots:
    void emitChanged();

private:
    QString commitedText;

};

#endif // FILTERWIDGET_H
