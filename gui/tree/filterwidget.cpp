#include "filterwidget.h"
#include <iostream>
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
