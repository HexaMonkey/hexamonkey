#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

/*!
 * \brief The TreeItem class
 */
class TreeItem : public QObject
{
    Q_OBJECT
public:
    TreeItem(const QList<QVariant> &data, TreeItem *parent = 0);
    virtual ~TreeItem();

    void appendChild(TreeItem *child);
    void removeChildren();

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parent();

    virtual bool synchronised();

signals:
    bool childrenRemoved();

private:
    QList<TreeItem*> childItems;
    TreeItem* parentItem;
    QList<QVariant> itemData;
};

#endif // TREEITEM_H
