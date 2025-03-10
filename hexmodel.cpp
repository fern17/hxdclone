#include "hexmodel.h"

HexModel::HexModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void HexModel::setFileData(const QByteArray &data)
{
    beginResetModel();
    fileData = data;
    clearModifiedBytes();
    endResetModel();
}

const QByteArray &HexModel::getFileData() const
{
    return fileData;
}

bool HexModel::isByteModified(int row, int column) const
{
    if (column >= ASCII_COLUMN_START)
    {
        column -= BYTES_PER_LINE;
    }
    qsizetype offset = row * BYTES_PER_LINE + column;
    return modifiedBytes.contains(offset);
}

void HexModel::clearModifiedBytes()
{
    modifiedBytes.clear();
}

bool HexModel::hasUnsavedChanges() const
{
    return !modifiedBytes.empty();
}

QVariant HexModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Vertical)
        {
            qsizetype offset = BYTES_PER_LINE * section;
            QString s;
            QTextStream ts(&s);
            ts << qSetFieldWidth(16) << qSetPadChar('0') << Qt::hex << offset;
            return s;
        }
        if (orientation == Qt::Horizontal)
        {
            if (section < BYTES_PER_LINE)
            {
                return QString::number(section, 16);
            }
            else
            {
                return QString(tr(" "));
            }
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

Qt::ItemFlags HexModel::flags(const QModelIndex &modelIndex) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(modelIndex);
    flags |= Qt::ItemIsEditable;
    return flags;
}

QVariant HexModel::data(const QModelIndex &modelIndex, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int row = modelIndex.row();
        int column = modelIndex.column();
        qsizetype offset = row * BYTES_PER_LINE + column;
        if (column < BYTES_PER_LINE)
        {
            if (offset < fileData.size())
            {
                unsigned char uc = static_cast<unsigned char>(fileData[offset]);
                return QString("%1").arg(uc, 2, 16, QLatin1Char('0'));
            }
        }
        else if (column < COLUMNS_PER_LINE)
        {
            offset -= BYTES_PER_LINE;
            if (offset < fileData.size())
            {
                char c = fileData[offset];
                if (c >= Qt::Key_Space && c <= Qt::Key_AsciiTilde)
                {
                    return QString(c);
                }
                else
                {
                    return ".";
                }
            }
        }
    }
    return QVariant();
}

bool HexModel::setData(const QModelIndex &modelIndex, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        int row = modelIndex.row();
        int column = modelIndex.column();
        if (column < COLUMNS_PER_LINE)
        {
            qsizetype offset = row * BYTES_PER_LINE + column;
            bool isAscii = false;
            if (column >= BYTES_PER_LINE)
            {
                offset -= BYTES_PER_LINE;
                isAscii = true;
            }
            if (offset < fileData.size())
            {
                bool ok = false;
                unsigned int byte = 0;
                if (isAscii)
                {
                    QString valueAsString = value.toString();
                    if (!valueAsString.isEmpty())
                    {
                        byte = valueAsString.toUtf8()[0];
                        ok = true;
                    }
                }
                else
                {
                    byte = value.toUInt(&ok);
                }

                if (ok)
                {
                    unsigned char uByte = static_cast<unsigned char>(byte);
                    if (fileData[offset] != uByte)
                    {
                        fileData[offset] = uByte;
                        modifiedBytes.insert(offset);
                        emit dataChanged(modelIndex, modelIndex);
                    }
                }
                return true;
            }
        }
    }
    return false;
}

int HexModel::rowCount(const QModelIndex &) const
{
    return (fileData.size() / BYTES_PER_LINE);
}

int HexModel::columnCount(const QModelIndex &) const
{
    return COLUMNS_PER_LINE;
}
