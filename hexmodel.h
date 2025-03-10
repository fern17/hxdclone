#ifndef HEXMODEL_H
#define HEXMODEL_H

#include <QAbstractTableModel>

class HexModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    static const int BYTES_PER_LINE = 16;
    static const int ASCII_COLUMN_START = BYTES_PER_LINE;
    static const int COLUMNS_PER_LINE = 2 * BYTES_PER_LINE;

    HexModel(QObject *parent = nullptr);

    void setFileData(const QByteArray &data);
    const QByteArray &getFileData() const;
    bool isByteModified(int row, int column) const;
    void clearModifiedBytes();
    bool hasUnsavedChanges() const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &modelIndex) const override;
    QVariant data(const QModelIndex &modelIndex, int role) const override;
    bool setData(const QModelIndex &modelIndex, const QVariant &value, int role) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    QByteArray fileData;
    QSet<qsizetype> modifiedBytes; // list of offsets
};

#endif // HEXMODEL_H
