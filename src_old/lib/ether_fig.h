#ifndef __ETHER_FIG_H__
#define __ETHER_FIG_H__

EAPI void          ether_fig_part_array_set (EtherObject **ptr, int maxparts);
EAPI void          ether_fig_scale_set      (float x, float y, float z);
EAPI EtherObject  *ether_fig_read           (FILE *in, EtherObject *parent, char *rootname);
EAPI EtherObject  *ether_fig_load           (char *filename);
    
#endif /* __ETHER_FIG_H__ */