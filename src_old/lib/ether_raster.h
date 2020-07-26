#ifndef __ETHER_RASTER_H__
#define __ETHER_RASTER_H__

/*
 * Ether uses the notion of a "raster", a rectangular array of pixel values.
 * In most cases, all rendering is done into a raster, which may or may not 
 * correspond to an actual physical screen; in cases where it doesn't, 
 * it's necessary to copy ("blit") the raster to the display.
 * Each raster has a height, a width, a depth (number of bits per pixel) a 
 * window (the rectangular region within the raster into which 
 * rendering is done) and a "rowbytes" value (the number of bytes per 
 * horizontal row of pixels).
 *
 * The reason for setting the number of bytes per row of pixels has to 
 * do with alternate scan-line encoding; if you render the left and right 
 * images into the left and right halves of a 640-pixel wide screen, and 
 * then set the rowbytes value to 320, you wind up with a raster that has
 * the left-eye image on the even scanlines and the right-eye image on the
 * odd scanlines.
 */

struct _Ether_Raster
{
  int width;
  int height;
  int depth;
  int left;
  int top;
  int right;
  int bottom;
  int rowbytes;
  unsigned char *data;
};

EAPI EtherRaster   *ether_raster_create         (int width, int height, int depth);
EAPI void           ether_raster_destroy        (EtherRaster *raster);
EAPI void           ether_raster_window_set     (EtherRaster *raster, int left, int top, int right, int bottom);
EAPI void           ether_raster_window_get     (EtherRaster *raster, int *left, int *top, int *right, int *bottom);
EAPI int            ether_raster_height_get     (EtherRaster *raster);
EAPI int            ether_raster_width_get      (EtherRaster *raster);
EAPI int            ether_raster_depth_get      (EtherRaster *raster);
EAPI int            ether_raster_rowbytes_get   (EtherRaster *raster);
EAPI void           ether_raster_rowbytes_set   (EtherRaster *raster, int n);
EAPI void           ether_raster_read_scanline  (EtherRaster *raster, int n, unsigned char *buff);
EAPI void           ether_raster_write_scanline (EtherRaster *raster, int n, unsigned char *buff);
EAPI unsigned char *ether_raster_data_get       (EtherRaster *raster);
EAPI int            ether_raster_size_get       (EtherRaster *raster);

#endif /* __ETHER_RASTER_H__ */
