#include"lpcv.h"
#include<string>
#include<expected>

#include<png.h>

lpcv::Image loadPNG(std::string fileName) {
    
    FILE* fp = fopen(fileName.c_str(), "rb");
    if (!fp) throw std::invalid_argument("Error opening file");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) throw std::invalid_argument("file not png");

    png_infop info = png_create_info_struct(png);
    if (!info) throw std::invalid_argument("Error getting png info");

    png_init_io(png, fp);

    png_read_info(png, info);


    if (png_get_color_type(png, info) == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    if (png_get_color_type(png, info) == PNG_COLOR_TYPE_GRAY && png_get_bit_depth(png, info) < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);
    png_read_update_info(png, info);

    uint32_t width = png_get_image_width(png, info);
    uint32_t height = png_get_image_height(png, info);
    uint8_t bitDepth = png_get_bit_depth(png, info);
    uint64_t bytesPerRow = png_get_rowbytes(png, info);

    lpcv::ColourType colourSpace; 
    switch (png_get_color_type(png, info)) {
        case 0:
            switch (bitDepth) {
            case 8: colourSpace = lpcv::G8; break;
            case 16: colourSpace = lpcv::G16; break;
            }
            break;
        case 2:
            switch (bitDepth) {
            case 8: colourSpace = lpcv::RGB8; break;
            case 16: colourSpace = lpcv::RGB16; break;
            default: throw std::invalid_argument("unsupported colour type");;
            }
            break;
        case 4:
            switch (bitDepth) {
            case 8: colourSpace = lpcv::GA8; break;
            case 16: colourSpace = lpcv::GA16; break;
            default: throw std::invalid_argument("unsupported colour type");;
            }
        case 6:
            switch (bitDepth) {
            case 8: colourSpace = lpcv::RGBA8; break;
            case 16: colourSpace = lpcv::RGBA16; break;
            default: throw std::invalid_argument("unsupported colour type");;
            }
            break;
        default: throw std::invalid_argument("unsupported colour type");;
    }


    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);

   std::vector<unsigned char> buffer;
    for (uint32_t y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));  
        png_read_row(png, row_pointers[y], NULL);
        buffer.insert(buffer.end(), (unsigned char*)row_pointers[y], (unsigned char*)row_pointers[y]+bytesPerRow);
    }

    lpcv::Image image(buffer, height, width, colourSpace, fileName);

   
    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);


    return image;
}