#ifndef ERRORWINDOW_H
#define ERRORWINDOW_H

#include <string>
#include <QMessageBox>
#include "errorobserver.h"



class ErrorWindow : public QMessageBox, public ErrorObserver
{
    Q_OBJECT
public:
    ErrorWindow(QWidget* parent = 0);
    void update(std::string errorRaised);
};

#endif // ERRORWINDOW_ H

