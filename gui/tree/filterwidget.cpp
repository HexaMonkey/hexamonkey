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

#include <iostream>

#include "gui/tree/filterwidget.h"

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent), lineEdit(new QLineEdit(this))
{
    QLabel* label = new QLabel("Filter :",this);
    QHBoxLayout* layout = new QHBoxLayout(this);

    setContentsMargins(0,0,0,0);
    layout->setContentsMargins(3,0,0,5);

    setLayout(layout);
    layout->addWidget(label);
    layout->addWidget(lineEdit);

    connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(emitChanged()));
    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(neutralizeText()));
}

void FilterWidget::changeText(QString text)
{
    lineEdit->setText(text);
    commitedText = text;
    neutralizeText();
}

void FilterWidget::invalidateText()
{
    std::cout<<"invalidate text"<<std::endl;
    if(lineEdit->text().size() != 0)
    {
        lineEdit->setStyleSheet("QLineEdit{background: #ffaaaa;}");
    }
}

void FilterWidget::neutralizeText()
{
    lineEdit->setStyleSheet("QLineEdit{background: white;}");
}

void FilterWidget::emitChanged()
{
    if(commitedText != lineEdit->text())
    {
        commitedText = lineEdit->text();
        emit textChanged(lineEdit->text());
    }
}
