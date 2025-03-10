#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hexitemdelegate.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->hexTable->setVisible(false);
    ui->saveFileButton->setVisible(false);
    ui->openedFileLineEdit->setReadOnly(true);

    connect(ui->loadFileButton, &QPushButton::clicked, this, &MainWindow::loadFile);
    connect(ui->saveFileButton, &QPushButton::clicked, this, &MainWindow::saveFile);

    ui->hexTable->setFont(QFont("Courier", 10));
    ui->hexTable->setModel(&model);

    // calculate table cells sizes with dummy data
    QByteArray dummyData = "0000000000000000";
    model.setFileData(dummyData);
    ui->hexTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->hexTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->hexTable->resizeColumnsToContents();
    ui->hexTable->resizeRowsToContents();
    ui->hexTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->hexTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->hexTable->setItemDelegate(new HexItemDelegate(model, ui->hexTable));

    model.setFileData("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (model.hasUnsavedChanges())
    {
        QMessageBox msgBox;
        msgBox.setText(QString("\"%1\" was changed. Save Changes?").arg(filePath));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        switch (ret)
        {
        case QMessageBox::Yes:
        {
            saveFile();
            event->accept();
            break;
        }
        case QMessageBox::No:
        {
            event->accept();
            break;
        }
        case QMessageBox::Cancel:
        {
            event->ignore();
            break;
        }
        default:
        {
            break;
        }
        }
    }
    else
    {
        event->accept();
    }
}

void MainWindow::loadFile()
{
    filePath = QFileDialog::getOpenFileName(this, tr("Open File"), ".");
    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray fileData = file.readAll();
            model.setFileData(fileData);
            ui->hexTable->setVisible(true);
            ui->saveFileButton->setVisible(true);
            ui->openedFileLineEdit->setText(filePath);
            file.close();
        }
    }
}

void MainWindow::saveFile()
{
    QFile file(filePath);
    if (file.open(QFile::WriteOnly | QFile::Truncate))
    {
        file.write(model.getFileData());
        model.setFileData(model.getFileData());
        file.close();
    }
    else
    {
        QMessageBox::critical(nullptr, "Error", "Failed to open file for writing.");
    }
}

