#pragma once

#include <stdio.h>

#include "core.h"
#include "color.h"

typedef struct __RGBA32_t bitmap_color_t;

struct __bitmap_file_header_t
{
	byte_t	header_field[2];	// Used to identify the BMP and DIB format.
	i32_t	size;				// The actual size of the file in bytes.
	byte_t	reserved1[2];		// Reserved
	byte_t	reserved2[2];		// Reserved
	i32_t	offset;				// The address where the bitmap image data starts.
};

struct __bitmap_information_header_t
{
	i32_t	header_size;
	u32_t	width;
	u32_t	height;
	i16_t	planes;
	i16_t	bpp;						// Bits per pixel
	i32_t	compression;
	i32_t	image_size;
	i32_t	x_ppm;						// X Pixel per meter
	i32_t	y_ppm;						// Y Pixel per meter
	i32_t	colors;
	i32_t	color_count;				// Important color count
	i32_t	red_bitmask;
	i32_t	green_bitmask;
	i32_t	blue_bitmask;
	i32_t	alpha_bitmask;
	i32_t	color_space_type;
	byte_t	color_space_endpoints[36];
	i32_t	red_gamma;
	i32_t	green_gamma;
	i32_t	blue_gamma;
	i32_t	intent;
	i32_t	icc_data;
	i32_t	icc_size;
	byte_t	reserved[4];
};

struct __bitmap_color_table_t
{
	bitmap_color_t* colors;
};

struct __bitmap_pixel_array_t
{
	byte_t* pixels;		// size: ceil(bpp * width / 32) * 4 * height
};

typedef struct __bitmap_file_header_t			bitmap_file_header_t;
typedef struct __bitmap_information_header_t	bitmap_information_header_t;
typedef struct __bitmap_color_table_t			bitmap_color_table_t;
typedef struct __bitmap_pixel_array_t			bitmap_pixel_array_t;

struct __bitmap_format_t
{
	bitmap_file_header_t*			header;
	bitmap_information_header_t*	information;	
	bitmap_color_table_t*			color_table;
	bitmap_pixel_array_t*			pixel_array;

	// TODO : ICC color profile data
};

typedef struct __bitmap_format_t bitmap_t;

LKA_API bitmap_t* load_bitmap_file(char* filepath);
LKA_API void destroy_bitmap_image(bitmap_t* image);

void load_bitmap_file_header		(bitmap_t* image, FILE* file);
void load_bitmap_information_header	(bitmap_t* image, FILE* file);
void load_bitmap_color_table		(bitmap_t* image, FILE* file);
void load_bitmap_image_data			(bitmap_t* image, FILE* file);
