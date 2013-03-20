#ifndef HEXFILEDELEGATE_H
#define HEXFILEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPen>
#include <QPainter>

/*!
 * \brief The HexFileDelegate class
 */
class HexFileDelegate : public QStyledItemDelegate        
{
public:
    HexFileDelegate(unsigned int hexWidth, unsigned int charWidth);
    void setHeaderWidth(unsigned int value);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
    unsigned int headerWidth;
    unsigned int hexWidth;
    unsigned int charWidth;
    static const QPen thickSelectionPen;
    static const QPen thinSelectionPen;
};

#endif // HEXFILEDELEGATE_H
