#ifndef HEXITEMDELEGATE_H
#define HEXITEMDELEGATE_H

#include <QStyledItemDelegate>
class HexModel;
class ContinuousEditionTableView;

class HexItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    HexItemDelegate(const HexModel &tableModel, ContinuousEditionTableView *tableView, QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

private:
    const HexModel &tableModel;
    ContinuousEditionTableView *tableView;
};

#endif // HEXITEMDELEGATE_H
