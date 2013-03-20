#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>

#include <string>

/*!
 * \brief The FilterWidget class
 */
class FilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FilterWidget(QWidget *parent = 0);
    
private:
    QLineEdit* lineEdit;

signals:
    void textChanged(QString text);

public slots:
    void changeText(QString text);
    void invalidateText();
    void neutralizeText();

private slots:
    void emitChanged();

private:
    QString commitedText;

};

#endif // FILTERWIDGET_H
