#ifndef LINEEDITWITHOVERWRITE_H
#define LINEEDITWITHOVERWRITE_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QMouseEvent>

class ContinuousEditionTableView;

class LineEditWithOverwrite : public QLineEdit
{
    Q_OBJECT
public:
    LineEditWithOverwrite(bool isAscii, ContinuousEditionTableView *tableView, const QModelIndex &index, QWidget *parent = nullptr);
    bool getIsAscii() const;
    void setTextAndTruncate(const QString &text);

signals:
    void endOfCellReached();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event);

private:
    bool isAscii;
};

#endif // LINEEDITWITHOVERWRITE_H
