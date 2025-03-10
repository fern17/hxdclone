#include "continuouseditiontableview.h"
#include "lineeditwithoverwrite.h"
#include "hexmodel.h"
#include <QHeaderView>

ContinuousEditionTableView::ContinuousEditionTableView(QWidget *parent)
    : QTableView(parent)
{
    QFont font("Courier", 10);
    setFont(font);
}

bool ContinuousEditionTableView::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event)
{
    return QTableView::edit(index, trigger, event);
}
