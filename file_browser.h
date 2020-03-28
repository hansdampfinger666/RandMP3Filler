#ifndef FILE_BROWSER_H
#define FILE_BROWSER_H

#include <QObject>
#include <QFileDialog>
#include <QStorageInfo>
#include <QString>

#include <iostream>


class FileBrowser : public QObject
{
    Q_OBJECT
public:
    explicit FileBrowser(QObject *parent = nullptr);
    void OpenFromDirDialog();
    void OpenToDirDialog();

    void SetFromDir(const QString &from_dir);
    void SetToDir(const QString &to_dir);

    QString GetFromDirLabel() { return from_dir_; };
    QString GetToDirLabel(){ return to_dir_; };
    QString GetToDirTotalSpaceLabel();
    QString GetToDirFreeSpaceLabel();
    int GetToDirRatioUsed() { return to_dir_ratio_used_; };
    QString GetFromDir() { return from_dir_; };
    float GetToDirFreeSpace() { return to_dir_free_space_bytes_; }
    float GetToDirTotalSpace() { return to_dir_total_space_bytes_; };
    bool FromDirOnline() { return from_dir_online_; };
    bool ToDirOnline() { return to_dir_online_; };

private slots:


signals:
    void FromDirChanged();
    void ToDirChanged();
    void ToDirSpaceDataChanged();

private:
    const float fac_byte_to_mb = 0.000001;
    const float fac_byte_to_gib_ = 0.0000000009313226;
    const float fac_byte_to_gb = 0.000000001;

    QString from_dir_;
    QString to_dir_;
    float to_dir_free_space_bytes_ = 0;
    float to_dir_total_space_bytes_ = 0;
    int to_dir_ratio_used_ = 0;
    bool from_dir_online_ = false;
    bool to_dir_online_ = false;

    QFileDialog *filebrowser_from_;
    QFileDialog *filebrowser_to_;
    QList<QStorageInfo> mounts_;

    bool VolumeIsOnline(const QString &dir);
    void FromDialogAccepted();
    void ToDialogAccepted();
    bool VolumesOnline(const QString from_dir, const QString to_dir);
    bool SetStorageSpaceData(const QString &dir);
};

#endif // FILE_BROWSER_H
