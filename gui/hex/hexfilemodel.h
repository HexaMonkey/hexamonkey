#ifndef HEXFILEMODEL_H
#define HEXFILEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QStringList>
#include <QFile>
#include <QPalette>

/*!
 * \brief The HexFileModel class
 */
class HexFileModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    HexFileModel(QWidget *parent);
    ~HexFileModel();

    qint64 focusPosition;
    bool   focused;
    qint64 hlPosition;
    qint64 hlSize;

    QModelIndex modelIndex(qint64 pos) const;
    qint64 position(QModelIndex i) const;
    qint64 search(QByteArray pattern, qint64 beginningPos);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

public slots:
    void setFile(const QString &path);

    void changeData(qint64 pos);
    void changeData(qint64 pos1, qint64 pos2);

    void focusIn();
    void focusOut();


private:
    QFile* file;
    qint64 fileSize;
    int headerCharCount;
};

#endif
