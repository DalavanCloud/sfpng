#include <libpng/png.h>

int main(int argc, char* argv[]) {
  png_structp png = NULL;
  png_infop info = NULL;

  FILE* f = fopen(argv[1], "rb");

  png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                               NULL, NULL, NULL);
  info = png_create_info_struct(png);

  png_init_io(png, f);
  png_read_png(png, info, 0, NULL);

  printf("dimensions: %dx%d\n",
         (int)png_get_image_width(png, info),
         (int)png_get_image_height(png, info));
  printf("bit depth: %d  color type: %d\n",
         png_get_bit_depth(png, info),
         png_get_color_type(png, info));
  int interlaced = png_get_interlace_type(png, info) != PNG_INTERLACE_NONE;
  printf("interlaced: %s\n", interlaced ? "yes" : "no");

  if (interlaced) {
    /* XXX skip interlaced images for now. */
    goto out;
  }

  double gamma;
  if (png_get_gAMA(png, info, &gamma))
    printf("gamma: %.2f\n", gamma);

  png_byte** row_pointers = png_get_rows(png, info);

 out:
  png_destroy_read_struct(&png, &info, NULL);

  return 0;
}