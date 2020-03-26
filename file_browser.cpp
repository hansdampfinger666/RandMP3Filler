#include "file_browser.h"


FileBrowser::FileBrowser(QObject *parent) : QObject(parent)
{
    filebrowser_from_ = new QFileDialog();
    filebrowser_from_->setFileMode(QFileDialog::Directory);
    QObject::connect(this->filebrowser_from_, &QFileDialog::accepted,
                     this, &FileBrowser::FromDialogAccepted);

    filebrowser_to_ = new QFileDialog();
    filebrowser_to_->setFileMode(QFileDialog::Directory);
    QObject::connect(this->filebrowser_to_, &QFileDialog::accepted,
                     this, &FileBrowser::ToDialogAccepted);
}


void
FileBrowser::OpenFromDirDialog()
{
    if(!from_dir_.isEmpty())
        filebrowser_from_->setDirectory(from_dir_);
    filebrowser_from_->exec();
}


void
FileBrowser::OpenToDirDialog()
{
    if(!to_dir_.isEmpty())
        filebrowser_to_->setDirectory(to_dir_);
    filebrowser_to_->exec();
}


void
FileBrowser::SetFromDir(const QString &from_dir)
{
    from_dir_ = from_dir;

    if(VolumeIsOnline(from_dir))
        from_dir_online_ = true;
    else
        from_dir_online_ = false;

    emit FromDirChanged();
}


void
FileBrowser::SetToDir(const QString &to_dir)
{
    to_dir_ = to_dir;

    if(VolumeIsOnline(to_dir))
    {
        SetStorageSpaceData(to_dir_);
        to_dir_online_ = true;
    }
    else
        to_dir_online_ = false;

    emit ToDirChanged();
}


bool
FileBrowser::VolumeIsOnline(const QString &dir)
{
    mounts_ = QStorageInfo::mountedVolumes();

    for(auto mount : mounts_)       
        if(QDir(dir).rootPath().indexOf(mount.rootPath()) != - 1)
            return true;
    return false;
}


void
FileBrowser::FromDialogAccepted()
{
    SetFromDir(filebrowser_from_->selectedFiles().at(0));
}


void
FileBrowser::ToDialogAccepted()
{
    SetToDir(filebrowser_to_->selectedFiles().at(0));
}


QString
FileBrowser::GetToDirFreeSpaceLabel()
{
    std::string gb = std::to_string(to_dir_free_space_bytes_ * fac_byte_to_gb);
    return QString::fromStdString(gb.substr(0, gb.find_first_of(".,") + 2) + " GB");
}


QString
FileBrowser::GetToDirTotalSpaceLabel()
{
    std::string gb = std::to_string(to_dir_total_space_bytes_ * fac_byte_to_gb);
    return QString::fromStdString(gb.substr(0, gb.find_first_of(".,") + 2) + " GB");
}


void
FileBrowser::SetStorageSpaceData(const QString &dir)
{
    QStorageInfo storage_info;

    storage_info.setPath(dir);
    to_dir_free_space_bytes_ = storage_info.bytesAvailable();
    to_dir_total_space_bytes_ = storage_info.bytesTotal();
    to_dir_ratio_used_ = (100 / to_dir_total_space_bytes_) *
            (to_dir_total_space_bytes_ - to_dir_free_space_bytes_);
    emit ToDirSpaceDataChanged();
}




