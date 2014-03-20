#include "errorwindow.h"

ErrorWindow::ErrorWindow(QWidget* parent): QMessageBox(parent)
{
    setWindowTitle("Error");
}

void ErrorWindow::update(std::string errorRaised)
{
    setText(QString::fromStdString(errorRaised));
    setVisible(true);
}

