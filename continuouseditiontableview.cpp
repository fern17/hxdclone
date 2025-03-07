#include "continuouseditiontableview.h"
#include "lineeditwithoverwrite.h"
#include <QHeaderView>

ContinuousEditionTableView::ContinuousEditionTableView(QWidget *parent)
    : QTableView(parent)
{
    QFont font("Courier", 10);
    setFont(font);
}

bool ContinuousEditionTableView::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event)
{
    QTableView::edit(index, trigger, event);
    LineEditWithOverwrite *lineEdit = dynamic_cast<LineEditWithOverwrite *>(indexWidget(index));
    if (lineEdit)
    {
        connect(lineEdit, &LineEditWithOverwrite::endOfCellReached, this, [this, index]() {
            int rowCount = model()->rowCount();
            int columnCount = model()->columnCount();
            int col = index.column();
            int row = index.row();
            if (col == (columnCount - 1))
            {
                ++row; // move to the next ascii row
            }
            else // any hex column
            {
                if (col < (columnCount - 2))
                {
                    ++col;
                }
                else if (row < (rowCount - 1))
                {
                    col = 0;
                    ++row;
                }
            }
            QModelIndex nextIndex(model()->index(row, col));
            if (nextIndex.isValid() && nextIndex != index)
            {
                setCurrentIndex(nextIndex);
            }
        });
        return true;
    }
    return false;
}
