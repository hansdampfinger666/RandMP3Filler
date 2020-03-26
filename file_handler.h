#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <QObject>
#include <QDir>

#include <cstdlib>
#include <ctime>

#include <iostream>
#include <benchmarker.h>
#include <config_loader.h>


class FileHandler : public QObject
{
    Q_OBJECT
public:
    FileHandler(ConfigLoader *configloader);
    void MakeTransferList(const QString &dir, const float bytes_to_fill);
    void CopyFilesToDevice();

public slots:

signals:

private:
    const float fac_byte_to_mb = 0.000001;
    const float fac_byte_to_gib_ = 0.0000000009313226;
    const float fac_byte_to_gb = 0.000000001;
    const float fac_mb_to_byte = 1000000;

    ConfigLoader *configloader_;

    typedef struct{
        std::vector<int> path_index;
        std::vector<QString> target_folder;
        std::vector<QString> target_subfolder;
    } TransferFolders;
    TransferFolders transfer_folders_ {};

    typedef struct{
        std::vector<int> path_index;
        std::vector<QString> source_path_file;
        std::vector<QString> target_file_name;
    } TransferFiles;
    TransferFiles transfer_files_ {};

    float filled_bytes_ = 0;

    void ReserveSizeTransferStructs(const float &bytes);
    QList<QString> GetSubFolderList(const QString &dir, const QStringList &subfolder_name_filter);
    bool IsDuplicate(const QString &folder, const QString &subfolder);
    QFileInfoList GetSubFolderInfoList(const QString &dir, const QStringList &subfolder_name_filter);
    float GetFolderSize(const QString &dir);
    QFileInfoList GetFolderFilesInfoList(const QString &dir, const QStringList &file_name_filter);
    void AddFolderFilesToTransferLists(const QString &dir, const QStringList &file_name_filter,
                                       const QString &folder, const QString &subfolder);

};

#endif // FILE_HANDLER_H
