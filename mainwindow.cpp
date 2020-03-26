#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    configloader_ = new ConfigLoader();
    configloader_->LoadConfig();
    filebrowser_= new FileBrowser();
    filehandler_ = new FileHandler(configloader_);

    QObject::connect(this->ui->BrowseInboundButton, &QPushButton::clicked,
                     this, &MainWindow::OnBrowseFromDirButtonClicked);
    QObject::connect(this->ui->BrowseOutboundButton, &QPushButton::clicked,
                     this, &MainWindow::OnBrowseToDirButtonClicked);
    QObject::connect(this->ui->MakeTransferListButton, &QPushButton::clicked,
                     this, &MainWindow::OnMakeTransferListButtonClicked);
    QObject::connect(this->ui->SendFilesButton, &QPushButton::clicked,
                     this, &MainWindow::OnSendFilesButtonClicked);

    QObject::connect(filebrowser_, &FileBrowser::FromDirChanged,
                     this, &MainWindow::UpdateFromDir);
    QObject::connect(filebrowser_, &FileBrowser::ToDirChanged,
                     this, &MainWindow::UpdateToDir);
    QObject::connect(filebrowser_, &FileBrowser::ToDirSpaceDataChanged,
                     this, &MainWindow::UpdateToDirSpace);

    QObject::connect(this->ui->TransferDial, &QDial::valueChanged,
                     this, &MainWindow::OnTransferDialChanged);
    QObject::connect(this->ui->ToDirTransferTextBox, &QLineEdit::returnPressed,
                     this, &MainWindow::OnTransferSizeTextBoxChanged);

    if(!configloader_->GetDefaultPathFrom().empty())
        filebrowser_->SetFromDir(QString::fromStdString(configloader_->GetDefaultPathFrom()));

    if(!configloader_->GetDefaultPathTo().empty())
        filebrowser_->SetToDir(QString::fromStdString(configloader_->GetDefaultPathTo()));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void
MainWindow::OnBrowseFromDirButtonClicked()
{
    filebrowser_->OpenFromDirDialog();
}


void
MainWindow::OnBrowseToDirButtonClicked()
{
    filebrowser_->OpenToDirDialog();
}


void
MainWindow::OnMakeTransferListButtonClicked()
{
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    filehandler_->MakeTransferList(filebrowser_->GetFromDir(),
                                   filebrowser_->GetToDirTotalSpace() *
                                   (float)this->ui->TransferDial->value() / 100);

    QGuiApplication::restoreOverrideCursor();
}


void
MainWindow::OnSendFilesButtonClicked()
{

}


void
MainWindow::OnTransferDialChanged()
{
    this->ui->ToDirTransferBar->setValue(this->ui->TransferDial->value());
}


void
MainWindow::OnTransferSizeTextBoxChanged()
{
    this->ui->ToDirTransferTextBox->blockSignals(true);

    std::string text = this->ui->ToDirTransferTextBox->text().toStdString();

    if(!text.empty())
    {
        for(auto character : text)
        {
            size_t index = text.find_first_not_of("1234567890");
            if(index != std::string::npos)
                text.replace(index, 1, "");
        }
        if(!text.empty())
            if(std::stof(text) * fac_gb_to_byte > filebrowser_->GetToDirFreeSpace())
            {
                std::string max_gb = std::to_string(filebrowser_->GetToDirFreeSpace() * fac_byte_to_gb);
                text = max_gb.substr(0, max_gb.find_first_of("."));
            }

        this->ui->ToDirTransferTextBox->setText(QString::fromStdString(text));
    }
    this->ui->ToDirTransferTextBox->blockSignals(false);
}


void
MainWindow::UpdateFromDir()
{
    this->ui->FromDirLabel->setText(filebrowser_->GetFromDirLabel());

    if(filebrowser_->FromDirOnline())
       this->ui->FromDirOnlineLED->setColor("green");
    else
        SetActionButtonsDisabled();
}


void
MainWindow::SetActionButtonsDisabled()
{
    this->ui->MakeTransferListButton->setDisabled(true);
    this->ui->SendFilesButton->setDisabled(true);
}


void
MainWindow::UpdateToDir()
{
    this->ui->ToDirLabel->setText(filebrowser_->GetToDirLabel());

    if(filebrowser_->ToDirOnline())
        this->ui->ToDirOnlineLED->setColor("green");
    else
        SetActionButtonsDisabled();
}


void
MainWindow::UpdateToDirSpace()
{
    this->ui->ToDirTotalSpaceLabel->setText(filebrowser_->GetToDirTotalSpaceLabel().append(" total"));
    this->ui->ToDirFreeSpaceLabel->setText(filebrowser_->GetToDirFreeSpaceLabel().append(" free"));
    this->ui->ToDirSpaceBar->setValue(filebrowser_->GetToDirRatioUsed());
    this->ui->TransferDial->setMaximum(100 - filebrowser_->GetToDirRatioUsed());
}
