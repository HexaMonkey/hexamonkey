#include "errorwindow.h"

#include <QScrollBar>
#include <iostream>

ErrorWindow::ErrorWindow(QWidget* parent): QPlainTextEdit(parent)
{
    setWindowTitle("Log");
}

void ErrorWindow::update(std::string errorRaised)
{
    std::cout<<"toto : "<<errorRaised<<std::endl;
    this->appendPlainText(QString::fromStdString(errorRaised+"\n")); // Adds the message to the widget
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()); // Scrolls to the bottom
}

