#include "lineeditwithoverwrite.h"

LineEditWithOverwrite::LineEditWithOverwrite(bool isAscii, QWidget *parent)
    : QLineEdit(parent),
    isAscii(isAscii),
    maxLength(isAscii ? 16 : 2)
{}

bool LineEditWithOverwrite::getIsAscii() const
{
    return isAscii;
}

void LineEditWithOverwrite::setTextAndTruncate(const QString &text)
{
    QString truncatedText = text.left(maxLength);
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
        QString truncatedText = currentText.left(maxLength);
        setText(truncatedText);
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
