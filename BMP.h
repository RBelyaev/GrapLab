#ifndef GRAPH_HEDER_BMP_H_
#define GRAPH_HEDER_BMP_H_

#include <string>
#include <vector>
#include <fstream>

#pragma pack(push, 1)
struct FileHeader {
	uint16_t file_type{ 0x4D42 };
	uint32_t file_size{ 0 };
	uint16_t reserved_1{ 0 };
	uint16_t reserved_2{ 0 };
	uint32_t offset_data{ 1078 };
};

struct InfoHeader {
	uint32_t header_size{ 40 };
	int32_t width{ 0 };
	int32_t height{ 0 };
	uint16_t planes{ 1 };
	uint16_t bit_count{ 8 };
	uint32_t compression{ 0 };
	uint32_t size_image{ 0 };
	int32_t x_pixels_per_meter{ 0 };
	int32_t y_pixels_per_meter{ 0 };
	uint32_t colors_used{ 256 };
	uint32_t colors_important{ 256 };
};

struct RGB {
	uint8_t red{ 0 };
	uint8_t green{ 0 };
	uint8_t blue{ 0 };
	uint8_t reserved{ 0 };
};
#pragma pack(pop)

class BMP {
public:
	BMP() { CreateColorTable(); };

	void Read(std::string& filename);
	void Write(std::string& filename);
	void Interpret(std::vector<std::vector<uint8_t>>& data);

	FileHeader file_header_;
	InfoHeader info_header_;
	std::vector<RGB> color_table_{ 256 };
	std::vector<uint8_t> data_;

private:
	void CreateColorTable();
};


#endif
