#include"lpcv.h"
#include<string>
#include<expected>
#include"lpcv/imagereader.h"
#include"lpcv/image.h"
#include<png.h>



std::expected<lpcv::Image, lpcv::Status> loadPNG(std::string fileName) {
    
    FILE* fp = fopen(fileName.c_str(), "rb");
    if (!fp) return std::unexpected(lpcv::ERROR_OPEN_FILE);

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) return std::unexpected(lpcv::ERROR_NOT_PNG);

    png_infop info = png_create_info_struct(png);
    if (!info) return std::unexpected(lpcv::ERROR_PNG_INFO_CREATION);

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
    uint8_t colourDepth = png_get_bit_depth(png, info);
    uint8_t colourSpace; 
    switch (png_get_color_type(png, info)) {
        case 0:
            colourSpace = lpcv::G;
            break;
        case 2:
            colourSpace = lpcv::RGB;
            break;
        case 4:
            colourSpace = lpcv::GA;
            break;
        case 6:
            colourSpace = lpcv::RGBA;
            break;
        default:
            return std::unexpected(lpcv::ERROR_UNSUPPORTED_COLOUR_SPACE);
    }


    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);

    lpcv::Image image(colourSpace,colourDepth,width,height);

    for (uint32_t y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));  
        png_read_row(png, row_pointers[y], NULL);
        image.appendData((char*)row_pointers[y], width*lpcv::getChannelCount(colourSpace)*(colourDepth/8));
    }

    
    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);

    return image;
}