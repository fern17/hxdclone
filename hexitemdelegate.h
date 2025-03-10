#ifndef HEXITEMDELEGATE_H
#define HEXITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QTableView>

class HexModel;

class HexItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    HexItemDelegate(const HexModel &tableModel, QTableView *tableView, QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

private:
    const HexModel &tableModel;
    QTableView *tableView;
};

#endif // HEXITEMDELEGATE_H
