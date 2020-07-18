#ifndef __ETHER_PLG_H__
#define __ETHER_PLG_H__

EAPI void          ether_plg_scale_set    (float x, float y, float z);
EAPI void          ether_plg_offset_set   (float x, float y, float z);
EAPI EtherShape   *ether_plg_read         (FILE *in);
EAPI int           ether_plg_write        (EtherShape *shape, FILE *out);
EAPI EtherObject  *ether_plg_load_object  (char *filename);
EAPI int           ether_plg_save_object  (EtherObject *object, char *filename);

    
#endif /* __ETHER_PLG_H__ */

