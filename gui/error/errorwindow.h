#ifndef ERRORWINDOW_H
#define ERRORWINDOW_H

#include <string>
#include <QPlainTextEdit>
#include "core/error/errorobserver.h"



class ErrorWindow : public QPlainTextEdit, public ErrorObserver
{
    Q_OBJECT
public:
    ErrorWindow(QWidget* parent = 0);
    void update(std::string errorRaised, LogLevel level);
};

#endif // ERRORWINDOW_ H

