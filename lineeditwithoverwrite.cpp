#include "lineeditwithoverwrite.h"
#include "hexmodel.h"
#include <QTableView>

LineEditWithOverwrite::LineEditWithOverwrite(bool isAscii,
                                             QTableView *tableView,
                                             const QModelIndex &index,
                                             QWidget *parent)
    : QLineEdit(parent),
    isAscii(isAscii)
{
    connect(this, &LineEditWithOverwrite::endOfCellReached, this, [this, tableView, index]() {
        int rowCount = tableView->model()->rowCount();
        int col = index.column();
        int row = index.row();
        if (col >= HexModel::ASCII_COLUMN_START)
        {
            if (col < (HexModel::COLUMNS_PER_LINE - 1))
            {
                ++col;
            }
            else if (row < (rowCount - 1))
            {
                col = HexModel::ASCII_COLUMN_START;
                ++row;
            }
        }
        else
        {
            if (col < (HexModel::ASCII_COLUMN_START - 1))
            {
                ++col;
            }
            else if (row < (rowCount - 1))
            {
                col = 0;
                ++row;
            }
        }
        QModelIndex nextIndex(tableView->model()->index(row, col));
        if (nextIndex.isValid() && nextIndex != index)
        {
            tableView->setCurrentIndex(nextIndex);
        }
    });
}

bool LineEditWithOverwrite::getIsAscii() const
{
    return isAscii;
}

void LineEditWithOverwrite::setTextAndTruncate(const QString &text)
{
    QString truncatedText = text.left(isAscii ? 1 : 2);
    QLineEdit::setText(truncatedText);
}

void LineEditWithOverwrite::keyPressEvent(QKeyEvent *event)
{
    if (event->key() >= Qt::Key_Space && event->key() <= Qt::Key_AsciiTilde)
    {
        int pos = cursorPosition();
        QString currentText = text();
        if (pos >= currentText.length())
        {
            emit endOfCellReached();
            return;
        }
        currentText.remove(pos, 1);
        currentText.insert(pos, event->text());
        setTextAndTruncate(currentText);
        ++pos;
        setCursorPosition(pos);
        if (pos == currentText.length())
        {
            emit endOfCellReached();
        }
    }
}

void LineEditWithOverwrite::focusInEvent(QFocusEvent *event)
{
    deselect();
    setCursorPosition(0);
    QLineEdit::focusInEvent(event);
}
