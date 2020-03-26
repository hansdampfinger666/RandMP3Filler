#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <QObject>
#include <QFileInfo>
#include <QDir>
#include <QString>

#include <string>
#include <vector>
#include <array>

#include <fstream>
#include <algorithm>


class ConfigLoader : public QObject
{
    Q_OBJECT

public:
    typedef struct{
        std::string value;
        unsigned int param_index;
    } FoundParam;

    struct ProgramParameters{
        std::string default_from_dir;
        std::string default_to_dir;
        float expected_folder_size;
        float expected_file_size;
    } program_params_ {};

    ConfigLoader();
    ~ConfigLoader();
    void LoadConfig();

    const std::string GetDefaultPathFrom() { return program_params_.default_from_dir; };
    const std::string GetDefaultPathTo() { return program_params_.default_to_dir; }
    float GetExpectedFolderSize() { return program_params_.expected_folder_size; };
    float GetExpectedFileSize() { return program_params_.expected_file_size; };

private:
    const std::string config_file_name_linux_ = "config";
    const std::vector<std::string> program_params_labels_ { "default_from_dir = ", "default_to_dir = ",
                                                            "expected_folder_size = ", "expected_file_size = "};
    std::string config_path_;

    FoundParam ReadValue(const std::string &data,
                         const std::vector<bool> &found_params,
                         const std::vector<std::string> &params);
    void AddValueToSet(const FoundParam &param);
    std::string Trim(std::string &str);
};

#endif // CONFIG_LOADER_H
