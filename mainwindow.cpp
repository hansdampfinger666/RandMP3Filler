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

//    clickablelabel_ = new ClickableLabel(this);
//    clickablelabel_->setGeometry(10,10,100,35);
//    clickablelabel_->topLevelWidget();
//    clickablelabel_->show();

//    QWidget *testwidget_ = new QWidget(this);
//    testwidget_->setGeometry(this->geometry());


//    testwidget_ = new QWidget();
//    testwidget_->setGeometry(this->geometry());
//    testwidget_->show();

//    QRect og_size = this->geometry();
//    QRect test_size = testwidget_->geometry();

//    std::cout << "x y height width" << std::endl;
//    std::cout << "og sizes: " << og_size.x() << "   " << og_size.y() << "   "  << og_size.height() << "   "  << og_size.width() << "   "  << std::endl;
//    std::cout << "shadow sizes " << test_size.x() << "   "  << test_size.y() << "   "  << test_size.height() << "   "  << test_size.width() << "   "  << std::endl;

//    clickablelabel_ = new ClickableLabel(this);
//    clickablelabel_->setGeometry(this->ui->ToDirTransferSizeText->geometry());
//    clickablelabel_->setText("Test");

//    QRect label_size = this->ui->ToDirTransferSizeText->geometry();
//    QRect shadow_size = clickablelabel_->geometry();

//    std::cout << "x y height width" << std::endl;
//    std::cout << "Text label sizes " << label_size.y() << "   " << label_size.x() << "   " << label_size.height() << "   " << label_size.width() << "   " << std::endl;
//    std::cout << "shadow sizes " << shadow_size.y() << "   " << shadow_size.x() << "   " << shadow_size.height() << "   " << shadow_size.width() << "   " << std::endl;




//    widget->topLevelWidget();
//    widget->show();
//    QPushButton *testbutton = new QPushButton(widget);
//    QPushButton *testbutton = new QPushButton(testwidget_);
//    testbutton->setGeometry(100,100,100,100);


//    QObject::connect(this->clickablelabel_, &ClickableLabel::DoubleClicked,
//                     this, &MainWindow::OnToDirTransferSizeTextDoubleClick);


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

    QObject::connect(this->ui->ToDirTransferDial, &QDial::valueChanged,
                     this, &MainWindow::OnToDirTransferDialChanged);
    QObject::connect(this->ui->ToDirTransferSizeText, &QLabel::underMouse,
                     this, &MainWindow::OnToDirTransferSizeTextDoubleClick);
//    QObject::connect(this->ui->ToDirTransferTextBox, &QLineEdit::returnPressed,
//                     this, &MainWindow::OnToDirTransferSizeTextBoxChanged);

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
                                   (float)this->ui->ToDirTransferDial->value() / 100);

    QGuiApplication::restoreOverrideCursor();
}


void
MainWindow::OnSendFilesButtonClicked()
{

}


void
MainWindow::OnToDirTransferDialChanged()
{
    this->ui->ToDirTransferBar->setValue(this->ui->ToDirTransferDial->value());
}


void
MainWindow::OnToDirTransferSizeTextDoubleClick()
{


}



//void
//MainWindow::OnToDirTransferSizeTextBoxChanged()
//{
//    this->ui->ToDirTransferTextBox->blockSignals(true);

//    std::string text = this->ui->ToDirTransferTextBox->text().toStdString();

//    if(!text.empty())
//    {
//        for(auto character : text)
//        {
//            size_t index = text.find_first_not_of("1234567890");
//            if(index != std::string::npos)
//                text.replace(index, 1, "");
//        }
//        if(!text.empty())
//            if(std::stof(text) * fac_gb_to_byte > filebrowser_->GetToDirFreeSpace())
//            {
//                std::string max_gb = std::to_string(filebrowser_->GetToDirFreeSpace() * fac_byte_to_gb);
//                text = max_gb.substr(0, max_gb.find_first_of("."));
//            }

//        this->ui->ToDirTransferTextBox->setText(QString::fromStdString(text));
//    }
//    this->ui->ToDirTransferTextBox->blockSignals(false);
//}


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

    if(filebrowser_->ToDirOnline())
    {
        this->ui->ToDirSpaceBar->setValue(filebrowser_->GetToDirRatioUsed());
        this->ui->ToDirTransferDial->setMaximum(100 - filebrowser_->GetToDirRatioUsed());
    }
    else
    {
        this->ui->ToDirSpaceBar->setValue(0);
        this->ui->ToDirTransferDial->setMaximum(0);
    }
}
