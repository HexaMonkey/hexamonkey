#include "hexfilesearchwidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>

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
