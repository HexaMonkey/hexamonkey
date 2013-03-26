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

#include "hexfilemodel.h"
#include <QApplication>
#include <QMessageBox>
#include "strutil.h"

HexFileModel::HexFileModel(QWidget *parent)
    : QAbstractTableModel(parent)
    , focusPosition(-1)
    , focused(true)
    , hlPosition(-1)
    , hlSize(0)
    , headerCharCount(0)
{
    beginInsertColumns(QModelIndex(),0,columnCount(QModelIndex()));
    endInsertColumns();
}

QModelIndex HexFileModel::modelIndex(qint64 pos) const
{
    return index(pos/16, pos%16 + 1);
}

qint64 HexFileModel::position(QModelIndex i) const
{
    if(i.isValid())
        return 16*i.row()+(i.column()-1)%16;
    else
        return 0;
}

qint64 HexFileModel::search(QByteArray pattern, qint64 beginningPos)
{

    if(pattern.isEmpty())
        return -1;

    QVector<int> prefixes(pattern.size(), -1);

    int k = -1;
    for (int i = 1; i < pattern.size(); ++i)
    {
        while (k > -1 && pattern[k+1] != pattern[i])
            k = prefixes[k];
        if (pattern[i] == pattern[k+1])
            k++;
        prefixes[i] = k;
    }

    file.seek(beginningPos);

    k = -1;
    char c;
    while(file.getChar(&c))
    {
        while (k > -1 && pattern[k+1] != c)
            k = prefixes[k];
        if (c == pattern[k+1])
            ++k;
        if (k == pattern.size() - 1)
        {
            qint64 pos = file.pos()-pattern.size();
            return 8*pos;
        }
    }

    if(beginningPos != 0)
    {
        int ret = QMessageBox::question(static_cast<QWidget*>(static_cast<QObject*>(this)->parent()), tr("End of file"),
                                        tr("End of file reached.\n"
                                           "Do you want to search from the beginning?"),
                                        QMessageBox::Ok | QMessageBox::Cancel);
        if(ret == QMessageBox::Ok)
            return search(pattern, 0);
        else
            return -1;
    }
    else
    {
        QMessageBox::information(static_cast<QWidget*>(static_cast<QObject*>(this)->parent()), tr("Not Found"),
                                        tr("Pattern not found.\n"),
                                        QMessageBox::Ok);
        return -1;
    }
}


//Number of lines
int HexFileModel::rowCount(const QModelIndex & /* parent */) const
{
    if (file.isOpen())
        return (fileSize/16)+1; //because 16 bytes per line
    return 0;
}

//Number of columns
int HexFileModel::columnCount(const QModelIndex & /* parent */) const
{
    return 33;//1 (header) + 16 (hex area) + 16 (char area)
}


//Specify how to get the data for the lines and columns
//Currently reading byte per byte on file
//ToDo add ~100 line buffer to read in RAM
QVariant HexFileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    if(index.column() == 0)
    {

        if(role == Qt::DisplayRole)
        {
            return QVariant(QString(toHex(index.row()*16, headerCharCount).c_str()));
        }

        if (role == Qt::ForegroundRole)
        {
            if(focusPosition >= 0 && index.row() == focusPosition/16)
            {
                if (focused)
                    return QBrush(qApp->palette().brightText());
            }
        }
        if (role == Qt::BackgroundRole)
        {
            if(focusPosition >= 0 && index.row() == focusPosition/16)
                if(focused)
                    return qApp->palette().highlight();
                else
                    return qApp->palette().dark();
            else
                return qApp->palette().alternateBase();
        }

        if (role == Qt::UserRole)
        {
            return false;
        }

        return QVariant();
    }


    //Compute position
    qint64 pos = position(index);

    //Nothing to display if position not in file
    if (pos >= fileSize)
        return QVariant("");

    //Read and display byte
    if (role == Qt::DisplayRole)
    {
        //As hex
        if(index.column() < 17)
        {
            file.seek(pos);
            QByteArray byte = file.read(1);
            return QVariant(byte.toHex());
        }
        //As char
        else
        {
            file.seek(pos);
            char ch;
            file.read(&ch, 1);
            if (std::isprint(ch))
            {
                return QVariant(QString::fromLatin1(&ch, 1));
            }
            else
            {
                return QVariant("sq");
            }
        }
    }

    //HL
    if (role == Qt::ForegroundRole)
    {
        if(pos == focusPosition)
        {
            if (focused)
                return QBrush(qApp->palette().brightText());
        }
    }
    if (role == Qt::BackgroundRole)
    {
        if(pos == focusPosition)
            if(focused)
                return qApp->palette().highlight();
            else
                return QBrush(qApp->palette().dark());
        else
            return qApp->palette().base();
    }

    //Alignment
    if (role == Qt::TextAlignmentRole)
    {
            return Qt::AlignCenter;
    }

    if (role == Qt::UserRole)
    {
        if (hlPosition <= pos && pos < (hlPosition + hlSize))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return QVariant();
}

//Specify how to get the line and column data
QVariant HexFileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            //Hex area
            if(section>=1 && section < 17)
            {
                if(section < 11)
                {
                    return QVariant(section-1);
                }
                else
                {
                    char ch = 'A' + (section-11);
                    return QVariant(QString::fromLatin1(&ch,1));
                }
            }
        }
        else
        {
                return QVariant(QString(toHex(section*16).c_str()));
        }
    }
    if (role == Qt::TextAlignmentRole)
    {
            return Qt::AlignLeft;
    }
    return QVariant();
}

Qt::ItemFlags HexFileModel::flags(const QModelIndex &index) const
{

    if (!index.isValid() || index.column()==0)
        return 0;

    qint64 pos = position(index);
    if(pos >= fileSize)
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


void HexFileModel::setFile(const QString &path)
{
    if (file.fileName() != path)
    {
        if (file.isOpen())
            file.close();
        beginRemoveRows(QModelIndex(),0,rowCount(QModelIndex()));
        endRemoveRows();
        file.setFileName(path);
        file.open(QIODevice::ReadOnly);
        fileSize = file.size();
#if 1
        if (file.size() > 1000000000)
            fileSize = 100000000;
#endif
        headerCharCount = toHex(fileSize).size();
        beginInsertRows(QModelIndex(),0,rowCount(QModelIndex()));
        endInsertRows();
    }
}

void HexFileModel::changeData(qint64 pos)
{
    changeData(pos, pos);
}

void HexFileModel::changeData(qint64 pos1, qint64 pos2)
{
    qint64 line1 = pos1/16;
    qint64 line2 = pos2/16;

    emit dataChanged(index(line1, 0), index(line2, 32));
}

void HexFileModel::focusIn()
{
    focused = true;
    changeData(focusPosition);
}

void HexFileModel::focusOut()
{
    focused = false;
    changeData(focusPosition);
}
