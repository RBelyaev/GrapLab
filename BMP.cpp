#include "BMP.h"


void BMP::Read(std::string& filename) {
    std::ifstream image(filename, std::ios::binary);

    if (!image.is_open()) 
        throw std::runtime_error("File wasn't opened");
    
    image.read(reinterpret_cast<char*>(&file_header_), sizeof(FileHeader));
    image.read(reinterpret_cast<char*>(&info_header_), sizeof(InfoHeader));

    for (RGB& color : color_table_) 
        image.read(reinterpret_cast<char*>(&color), sizeof(RGB));

    data_.resize(info_header_.size_image);
    image.read(reinterpret_cast<char*>(data_.data()), data_.size());

    image.close();
}

void BMP::Write(std::string& filename) 
{
    std::ofstream image(filename, std::ios::binary);

    if (!image.is_open()) 
        throw std::runtime_error("File wasn't opened");

    image.write(reinterpret_cast<char*>(&file_header_), sizeof(FileHeader));
    image.write(reinterpret_cast<char*>(&info_header_), sizeof(InfoHeader));

    for (RGB& color : color_table_) {
        image.write(reinterpret_cast<char*>(&color), sizeof(RGB));
    }

    image.write(reinterpret_cast<char*>(data_.data()), data_.size());

    image.close();
}

void BMP::CreateColorTable() 
{
    for (int i = 0; i < 256; ++i) 
    {
        color_table_[i].red = i;
        color_table_[i].green = i;
        color_table_[i].blue = i;
    }
}

void BMP::Interpret(std::vector<std::vector<uint8_t>>& map) 
{
    data_.reserve(map.size() * map[0].size());

    for (auto& row : map)
        data_.insert(data_.end(), row.begin(), row.end());

    file_header_.file_size = sizeof(FileHeader) + sizeof(InfoHeader) +
        color_table_.size() * sizeof(RGB) + map.size();

    info_header_.height = map.size();
    info_header_.width = map[0].size();
    info_header_.size_image = data_.size();
}