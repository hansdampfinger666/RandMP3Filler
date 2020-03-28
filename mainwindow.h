#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLineEdit>

#include <config_loader.h>
#include <file_browser.h>
#include <file_handler.h>
#include <clickable_label.h>

#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void OnBrowseFromDirButtonClicked();
    void OnBrowseToDirButtonClicked();
    void OnToDirTransferDialChanged();
    void OnToDirTransferSizeTextDoubleClick();
//    void OnToDirTransferSizeTextBoxChanged();
    void OnMakeTransferListButtonClicked();
    void OnSendFilesButtonClicked();

    void UpdateFromDir();
    void UpdateToDir();
    void UpdateToDirSpace();

private:
    float fac_gb_to_byte = 1000000000;
    float fac_byte_to_gb = 0.000000001;

    Ui::MainWindow *ui;
    FileBrowser *filebrowser_;
    ConfigLoader *configloader_;
    FileHandler *filehandler_;

    QWidget *testwidget_;
    ClickableLabel *clickablelabel_;

    void SetActionButtonsDisabled();
};
#endif // MAINWINDOW_H
