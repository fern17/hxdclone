#ifndef CONTINUOUSEDITIONTABLEVIEW_H
#define CONTINUOUSEDITIONTABLEVIEW_H

#include <QTableView>

class ContinuousEditionTableView : public QTableView
{
    Q_OBJECT
public:
    ContinuousEditionTableView(QWidget *parent = nullptr);

protected:
    bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event) override;
};

#endif // CONTINUOUSEDITIONTABLEVIEW_H
