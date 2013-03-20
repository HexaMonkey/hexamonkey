#ifndef HEXFILESEARCHWIDGET_H
#define HEXFILESEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QRadioButton>
#include <QPushButton>
#include <QByteArray>

class HexFileSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HexFileSearchWidget(QWidget *parent = 0);
    
signals:
    void searchRequested(QByteArray, bool);

public slots:
    void reset();
    void focusSearch();

private slots:
    void search();

private:
    QLineEdit* lineEdit;
    QRadioButton* hexButton;
    QRadioButton* asciiButton;
    QPushButton* nextButton;

    QByteArray pattern;
};

#endif // HEXFILESEARCHWIDGET_H
