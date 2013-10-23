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

#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>

#include "gui/hex/hexfilesearchwidget.h"

HexFileSearchWidget::HexFileSearchWidget(QWidget *parent) :
    QWidget(parent),
    lineEdit(new QLineEdit(this)),
    hexButton(new QRadioButton("hex", this)),
    asciiButton(new QRadioButton("ascii", this)),
    nextButton(new QPushButton("next", this))
{
    QLabel* label = new QLabel("Find :",this);
    QHBoxLayout* layout = new QHBoxLayout(this);


    setContentsMargins(0,0,0,0);
    layout->setContentsMargins(3,0,0,2);

    hexButton->setChecked(true);

    setLayout(layout);
    layout->addWidget(label);
    layout->addWidget(lineEdit);
    layout->addWidget(hexButton);
    layout->addWidget(asciiButton);
    layout->addWidget(nextButton);

    //Commit search
    connect(nextButton, SIGNAL(pressed()), this, SLOT(search()));
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(search()));

    //Reset on change
    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(reset()));
    connect(hexButton, SIGNAL(toggled(bool)), this, SLOT(reset()));
    connect(asciiButton, SIGNAL(toggled(bool)), this, SLOT(reset()));
}


void HexFileSearchWidget::reset()
{
    pattern.clear();
}

void HexFileSearchWidget::focusSearch()
{
    lineEdit->setFocus();
}

void HexFileSearchWidget::search()
{
    if(!pattern.isEmpty())
    {
        emit searchRequested(pattern, true);
    }
    else
    {
        QString text = lineEdit->text();
        if(hexButton->isChecked())
        {
            pattern = QByteArray::fromHex(text.toStdString().c_str());
        }
        else
        {
            pattern = text.toStdString().c_str();
        }
        emit searchRequested(pattern, false);
    }
}
