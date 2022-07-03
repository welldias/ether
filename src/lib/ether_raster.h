#ifndef __ETHER_RASTER_H__
#define __ETHER_RASTER_H__

#include "ether_defines.h"

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
