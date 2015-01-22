#include "logwidget.h"

#include <QScrollBar>

LogWidget::LogWidget(QWidget* parent): QPlainTextEdit(parent)
{
    setWindowTitle("Log");
}

void LogWidget::update(const std::string &str, LogLevel level)
{
    switch (level) {
        case LogLevel::Info:
            this->appendPlainText(QString::fromStdString("[INFO]    "+str));
            break;

        case LogLevel::Warning:
            this->appendPlainText(QString::fromStdString("[WARNING] "+str));
            break;

        case LogLevel::Error:
            this->appendPlainText(QString::fromStdString("[ERROR]   "+str));
            break;
    }
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()); // Scrolls to the bottom
}

