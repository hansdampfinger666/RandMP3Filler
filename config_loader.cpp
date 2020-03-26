#include "config_loader.h"


ConfigLoader::ConfigLoader()
{
    config_path_ = QDir::currentPath().toStdString() + "/" + config_file_name_linux_;
}


ConfigLoader::~ConfigLoader()
{}


void
ConfigLoader::LoadConfig()
{
    QFileInfo file_info(QString::fromStdString(config_path_));

    std::string data;
    std::vector<bool> found_params;
    found_params.assign(program_params_labels_.size(), false);
    FoundParam found_param;

    if(file_info.exists() && file_info.isFile())
    {
        std::ifstream data_stream(config_path_);

        while(getline(data_stream, data))
        {
            if(data.empty() || data.find_first_of("\"") == 0 || data.find("//") == 0)
                continue;

            data = Trim(data);
            found_param = ReadValue(data, found_params, program_params_labels_);

            AddValueToSet(found_param);
        }
    }
}


ConfigLoader::FoundParam
ConfigLoader::ReadValue(const std::string &data,
                        const std::vector<bool> &found_params,
                        const std::vector<std::string> &params)
{
    unsigned int param_index = 0;
    for(auto param : params)
    {
        if(found_params.at(param_index))
        {
            param_index++;
            continue;
        }
        if(data.substr(0, param.size()) == param)
            return { data.substr(param.size()), param_index };
        param_index++;
    }
    return { "", 0 };
}


void
ConfigLoader::AddValueToSet(const FoundParam &param)
{
    switch(param.param_index)
    {
    case 0:
        program_params_.default_from_dir = param.value;
        break;
    case 1:
        program_params_.default_to_dir = param.value;
        break;
    case 2:
        program_params_.expected_folder_size = std::stof(param.value);
        break;
    case 3:
        program_params_.expected_file_size = std::stof(param.value);
        break;
    }
}


std::string
ConfigLoader::Trim(std::string &str)
{
    return str = str.substr(str.find_first_not_of(" "), str.find_last_not_of(" ") + 1);
}
