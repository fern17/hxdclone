#include "hexitemdelegate.h"
#include <QLineEdit>
#include <QKeyEvent>
#include <QPainter>
#include <QFontMetrics>
#include "lineeditwithoverwrite.h"
#include "hexmodel.h"

HexItemDelegate::HexItemDelegate(const HexModel &tableModel, ContinuousEditionTableView *tableView, QObject *parent)
    : QStyledItemDelegate(parent),
    tableModel(tableModel),
    tableView(tableView)
{
}

void HexItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString text = tableModel.data(index, Qt::DisplayRole).toString();
    QPen redPen(Qt::red);
    if (tableModel.isByteModified(index.row(), index.column()))
    {
        painter->save();
        painter->setPen(redPen);
        painter->drawText(option.rect, Qt::AlignCenter, text);
        painter->restore();
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QWidget *HexItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    bool isAscii = index.column() >= HexModel::ASCII_COLUMN_START;
    LineEditWithOverwrite *lineEdit = new LineEditWithOverwrite(isAscii, tableView, index, parent);
    lineEdit->setTextAndTruncate(index.model()->data(index, Qt::DisplayRole).toString());
    lineEdit->setCursorPosition(0);
    return lineEdit;
}

void HexItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    LineEditWithOverwrite *lineEdit = dynamic_cast<LineEditWithOverwrite *>(editor);
    if (lineEdit)
    {
        lineEdit->setTextAndTruncate(index.model()->data(index, Qt::DisplayRole).toString());
        lineEdit->setCursorPosition(0);
    }
}

void HexItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    LineEditWithOverwrite *lineEdit = dynamic_cast<LineEditWithOverwrite *>(editor);
    if (lineEdit)
    {
        QString text = lineEdit->text();
        if (lineEdit->getIsAscii())
        {
            model->setData(index, text, Qt::EditRole);
        }
        else
        {
            bool valid = false;
            unsigned int value = text.toUInt(&valid, 16);
            if (valid)
            {
                quint8 byte = static_cast<quint8>(value);
                model->setData(index, byte, Qt::EditRole);
            }
        }
    }
}
