#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTreeView>
#include <QGridLayout>
#include <QString>

#include "treemodel.h"
#include "treeview.h"
#include "htmldelegate.h"
#include "filterwidget.h"
#include "resourcemanager.h"

class InterpreterConstructor;
/*!
 * \brief The TreeWidget class
 */
class TreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TreeWidget(const InterpreterConstructor& interpreterConstructor, QWidget *parent = 0);
    QSize sizeHint() const;
    void setColumnsWidths();

signals:
    void pathChanged(QString);
    void positionChanged(qint64, qint64);
    void eventDropped(QDropEvent*);

public slots:
    QModelIndex addObject(Object& object);
    void updatePath(QModelIndex currentIndex);
    void updatePosition(QModelIndex currentIndex);
    void setCurrentIndex(QModelIndex index);

protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);

private:
    TreeView* view;
    TreeModel* model;
    HTMLDelegate* delegate;
    FilterWidget* filterWidget;

    const InterpreterConstructor& interpreterConstructor;

    QString path;
    quint64 position;
    quint64 size;
};

#endif // TREEWIDGET_H
