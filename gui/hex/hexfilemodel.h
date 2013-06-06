//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef HEXFILEMODEL_H
#define HEXFILEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QStringList>
#include <QFile>
#include <QPalette>

/*!
 * @brief The HexFileModel class
 */
class HexFileModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    HexFileModel(QWidget *parent);

    qint64 focusPosition;
    bool   focused;
    qint64 hlPosition;
    qint64 hlSize;

    QModelIndex modelIndex(qint64 pos) const;
    qint64 position(QModelIndex i) const;
    qint64 search(QByteArray pattern, qint64 beginningPos);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

public slots:
    void setFile(const QString &path);

    void changeData(qint64 pos);
    void changeData(qint64 pos1, qint64 pos2);

    void focusIn();
    void focusOut();


private:
    mutable QFile file;
    qint64 fileSize;
    int headerCharCount;
};

#endif
