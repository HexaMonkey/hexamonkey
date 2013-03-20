#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeView>

/*!
 * \brief The TreeView class
 */
class TreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit TreeView(QWidget *parent = 0);
    
signals:
    void selected(QModelIndex);
    
public slots:

protected:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
    
};

#endif // TREEVIEW_H
