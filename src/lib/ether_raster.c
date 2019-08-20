#include <Ether.h>

EtherRaster  *
ether_raster_create(int width, int height, int depth)
{
  EtherRaster *raster = malloc(sizeof(EtherRaster));
  if(raster == NULL)
    return NULL;
  raster->width    = width;
  raster->height   = height;
  raster->depth    = depth;
  raster->top      = 0;
  raster->left     = 0;
  raster->right    = width-1;
  raster->bottom   = height-1;
  raster->rowbytes = width; // * ((depth+7)/8);
  raster->data     = malloc(raster->height * raster->rowbytes);
  if(raster->data) {
    memset(raster->data, 0, raster->height * raster->rowbytes);
    return raster;
  }
  free(raster);
  return NULL;
}

void
ether_raster_destroy(EtherRaster *raster)
{
	if(raster == NULL)
		return;
	if(raster->data)
		free(raster->data);
	free(raster);
}

void
ether_raster_window_set(EtherRaster *raster, int left, int top, int right, int bottom)
{
	if(raster == NULL)
		return;
	raster->left   = left;
	raster->top    = top;
	raster->right  = right;
	raster->bottom = bottom;
}

void
ether_raster_window_get(EtherRaster *raster, int *left, int *top, int *right, int *bottom)
{
	if(raster == NULL)
		return;
	if(left)
		*left = raster->left;
	if(top)
		*top = raster->top;
	if(right)
		*right = raster->right;
	if(bottom)
		*bottom = raster->bottom;
}

int
ether_raster_height_get(EtherRaster *raster)
{
	if(raster)
		return raster->height;
	else
		return 0;
}

int
ether_raster_width_get(EtherRaster *raster)
{
	if(raster)
		return raster->width;
	else
		return 0;
}

int
ether_raster_depth_get(EtherRaster *raster)
{
	if(raster)
		return raster->depth;
	else
		return 0;
}

int
ether_raster_rowbytes_get(EtherRaster *raster)
{
	if(raster)
		return raster->rowbytes;
	else
		return 0;
}

void
ether_raster_rowbytes_set(EtherRaster *raster, int n)
{
	if(raster)
		raster->rowbytes = n;
}

void
ether_raster_read_scanline(EtherRaster *raster, int n, unsigned char *buff)
{
	if(raster)
		memcpy(buff, &raster->data[n*raster->rowbytes], raster->rowbytes);
}

void
ether_raster_write_scanline(EtherRaster *raster, int n, unsigned char *buff)
{
	if(raster)
		memcpy(&raster->data[n*raster->rowbytes], buff, raster->rowbytes);
}

unsigned char *
ether_raster_data_get(EtherRaster *raster)
{
	/*--> Algo está acontecendo com este teste */
	/* sempre está retornando NULL. Talvez um mal gerenciamento de memoria. */
	if(raster)
	  return raster->data;
	return NULL;
}

int
ether_raster_size_get(EtherRaster *raster)
{
	if(raster)
		return raster->height * raster->rowbytes;
	return 0;
}
