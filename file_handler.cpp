#include "file_handler.h"

FileHandler::FileHandler(ConfigLoader *configloader)
{
    configloader_ = configloader;

//    QFile::copy("/run/media/al/FreeNAS/Musik/Musik/Digested Flesh/Macerate The Soul/01 Copulation Of The Dead.mp3",
//                "/home/al/Music/Digested Flesh/Macerate The Soul/01 Copulation Of The Dead.mp3");

//    QFile::copy("/run/media/al/FreeNAS/Musik/Musik/Digested Flesh/The Answer To Infection",
//                "/home/al/Music/Digested Flesh/The Answer To Infection");

//    QDir().mkdir("/home/al/Music/Digested Flesh");
}

//float subfolder_size = QFile(subfolder_info.at(0).absoluteFilePath()).size() * fac_byte_to_mb;
//folder iteration needed to get size of folder (https://stackoverflow.com/questions/47854288/can-not-get-directory-size-in-qt-c)

// copy works with smb share mounted to file system https://wiki.samba.org/index.php/Mounting_samba_shares_from_a_unix_client
//    QFile::copy("/run/media/al/FreeNAS/MAC Enterprise.txt",
//                "/home/al/Documents/test");


void
FileHandler::MakeTransferList(const QString &dir, const float bytes_to_fill)
{
    Benchmarker::start_timer("With duplicate check");
    std::cout << "GB to fill: " << bytes_to_fill * fac_byte_to_gb << std::endl;

    ReserveSizeTransferStructs(bytes_to_fill);

    QStringList folder_list_ = GetSubFolderList(dir, QStringList() << "*");

    std::srand(std::time(nullptr));
    int i = 0;

    while(filled_bytes_ < (bytes_to_fill - 150 * fac_mb_to_byte))
    {
        int folder_id = std::rand() % (folder_list_.count() - 1);
        int subfolder_id = 0;
        float subfolder_size = 0;

        QFileInfoList folder_info = GetSubFolderInfoList(dir,
                                                         QStringList() << folder_list_.at(folder_id));

        if(folder_info.isEmpty())
            continue;

        QStringList subfolder_list = GetSubFolderList(folder_info.at(0).absoluteFilePath(),
                                                      QStringList() << "*");

        if(subfolder_list.isEmpty())
            continue;
        if(subfolder_list.count() == 1)
            subfolder_id = 0;
        else if(subfolder_list.count() > 1)
            subfolder_id = std::rand() % (subfolder_list.count() - 1);
        if(IsDuplicate(folder_list_.at(folder_id), subfolder_list.at(subfolder_id)))
            continue;

        QFileInfoList subfolder_info = GetSubFolderInfoList(folder_info.at(0).absoluteFilePath(),
                                                            QStringList() << subfolder_list.at(subfolder_id));

        if(subfolder_info.isEmpty())
            continue;

        subfolder_size = GetFolderSize(subfolder_info.at(0).absoluteFilePath());

        if(subfolder_size == 0)
            subfolder_size = 150 * fac_mb_to_byte;

        if(((filled_bytes_ + subfolder_size) > bytes_to_fill))
            continue;

        filled_bytes_ += subfolder_size;

        AddFolderFilesToTransferLists(subfolder_info.at(0).absoluteFilePath(),
                                      QStringList() << "*",
                                      folder_list_.at(folder_id),
                                      subfolder_list.at(subfolder_id));
        i++;
    }

    std::cout << "Requested to fill up " << bytes_to_fill * fac_byte_to_gb << " GB of data." << std::endl
              << "Filled " << filled_bytes_ * fac_byte_to_gb << " GB worth of data" << std::endl
              << "albums included: " << i << std::endl;

    Benchmarker::end_timer(__PRETTY_FUNCTION__);
}


void
FileHandler::ReserveSizeTransferStructs(const float &bytes)
{
    int spaces_folders = 0;
    int spaces_files = 0;

    if(configloader_->GetExpectedFolderSize() != 0)
        spaces_folders = bytes / configloader_->GetExpectedFolderSize();
    else
        spaces_folders = bytes / 70000000;

    if(configloader_->GetExpectedFileSize() != 0)
        spaces_files = bytes / configloader_->GetExpectedFileSize();
    else
        spaces_files = bytes / 6000000;

    transfer_folders_.path_index.reserve(spaces_folders);
    transfer_folders_.target_folder.reserve(spaces_folders);
    transfer_folders_.target_subfolder.reserve(spaces_folders);

    transfer_files_.path_index.reserve(spaces_files);
    transfer_files_.source_path_file.reserve(spaces_files);
    transfer_files_.target_file_name.reserve(spaces_files);
}


QList<QString>
FileHandler::GetSubFolderList(const QString &dir,
                              const QStringList &subfolder_name_filter)
{    
    if(subfolder_name_filter.at(0) == "Impure [GER]")
        std::cout << subfolder_name_filter.at(0).toStdString() << std::endl;

    return QDir(dir).entryList(subfolder_name_filter,
                               QDir::Dirs | QDir::NoDotAndDotDot);
}


QFileInfoList
FileHandler::GetSubFolderInfoList(const QString &dir,
                                  const QStringList &subfolder_name_filter)
{
    return QDir(dir).entryInfoList(subfolder_name_filter,
                                   QDir::Dirs | QDir::NoDotAndDotDot);
}


float
FileHandler::GetFolderSize(const QString &dir)
{
    float size = 0;
    QFileInfoList files = GetFolderFilesInfoList(dir, QStringList() << "*");
    QFileInfoList subfolders = GetSubFolderInfoList(dir, QStringList() << "*");

    for(auto file : files)
        size += file.size();

    if(!subfolders.isEmpty())
        for(auto subfolder : subfolders)
            size += GetFolderSize(subfolder.absoluteFilePath());

    return size;
}


QFileInfoList
FileHandler::GetFolderFilesInfoList(const QString &dir,
                                    const QStringList &file_name_filter)
{
    return QDir(dir).entryInfoList(file_name_filter, QDir::Files);
}


bool
FileHandler::IsDuplicate(const QString &folder, const QString &subfolder)
{
    for(auto transfer_folder : transfer_folders_.target_folder)
        if(transfer_folder == folder)
            for(auto transfer_subfolder : transfer_folders_.target_subfolder)
                if(transfer_subfolder == subfolder)
                    return true;
    return false;
}


void
FileHandler::AddFolderFilesToTransferLists(const QString &dir,
                                           const QStringList &file_name_filter,
                                           const QString &folder,
                                           const QString &subfolder)
{
    int path_index;
    QFileInfoList source_files = GetFolderFilesInfoList(dir, file_name_filter);

    if(transfer_folders_.path_index.empty())
        path_index = 0;
    else
        path_index = transfer_folders_.path_index.back() + 1;

    transfer_folders_.path_index.push_back(path_index);
    transfer_folders_.target_folder.push_back(folder);
    transfer_folders_.target_subfolder.push_back(subfolder);

    for(auto source_file : source_files)
    {
        transfer_files_.path_index.push_back(path_index);
        transfer_files_.source_path_file.push_back(source_file.absoluteFilePath());
        transfer_files_.target_file_name.push_back(source_file.fileName());
    }
}


