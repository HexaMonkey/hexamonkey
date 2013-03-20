#ifndef HTMLDELEGATE_H
#define HTMLDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

/*!
 * \brief The HTMLDelegate class
 */
class HTMLDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit HTMLDelegate(QObject *parent = 0);

protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

signals:
    
public slots:
    
};

#endif // HTMLDELEGATE_H
