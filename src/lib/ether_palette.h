#ifndef __ETHER_PALETTE_H__
#define __ETHER_PALETTE_H__

#include <stdio.h>

#include "ether_defines.h"

/*
 * The following functions will initialize a palette, read a palette and huemap
 * data from a file, get and set individual entries in the palette, get a pointer
 * to a palette's huemap, and read and check the "changed" status of the palette:
 */

EAPI void          ether_palette_init                    (EtherPalette *pal);
EAPI int           ether_palette_read                    (EtherPalette *pal, FILE *in);
EAPI EtherColor    ether_palette_entry_get               (EtherPalette *pal, int n);
EAPI void          ether_palette_entry_set               (EtherPalette *pal, int n, EtherColor color);
EAPI int           ether_palette_has_changed             (EtherPalette *pal);
EAPI void          ether_palette_changed_set             (EtherPalette *pal, int flag);
EAPI EtherHue     *ether_palette_huemap_get              (EtherPalette *pal);
EAPI EtherPalette *ether_palette_default_palette_256_get (void);
EAPI EtherPalette *ether_palette_default_palette_16_get  (void);
EAPI EtherPalette *ether_palette_anaglyph_palette_get    (void);
EAPI EtherPalette *ether_palette_chromadepth_palette_get (void);

    
#endif /* __ETHER_PALETTE_H__ */
