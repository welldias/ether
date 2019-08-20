#ifndef __ETHER_WLD_H__
#define __ETHER_WLD_H__

EAPI int ether_wld_read(FILE *in);
EAPI int ether_wld_load(char *filename);
EAPI int ether_wld_process_line(char *buff);

#endif /* __ETHER_CAMERA_H__ */
