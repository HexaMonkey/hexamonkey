#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QtWidgets/QPlainTextEdit>
#include <string>


#include "core/log/logger.h"

class LogWidget : public QPlainTextEdit, public Logger
{
    Q_OBJECT
public:
    LogWidget(QWidget* parent = 0);
    virtual void update(const std::string& str, LogLevel level) override;
};

#endif // LOGWIDGET_ H

