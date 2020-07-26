#ifndef __ETHER_PALETTE_H__
#define __ETHER_PALETTE_H__

/*
 * Ether makes use of a "palette", a collection of (up to) 256 colors.
 * Each of those colors has three 8-bit components -- one for red, one for 
 * green and one for blue.
 * The Ether_Color values that Ether uses are (in a paletted implementation) 
 * used to index the palette to select an actual color. In order to do 
 * shading of facets and vertices, Ether divides the palette into a number 
 * of "hues", with a number of shades for each hue. By default, the first 
 * 16 entries of the 256-color palette are simple non-shaded colors (for use 
 * in menus and overlaid text); the remaining 240 colors are treated as 15 
 * hues with 16 shades each.
 *
 * However, this is not etched in stone. Ether supports the use of a "hue map",
 * which relates a hue index to a start color in the palette and a count of the 
 * number of shades.  For example, by using the hue map, you could choose to 
 * have 64 shades of flesh tone (instead of 16) in order to represent human 
 * beings more accurately.
 *
 * A hue is represented by a Ether_Hue type, and a palette by a Ether_Palette type.
 * Palettes have a flag that indicates that they've been changed; this is important,
 * since it forces the system to update the physical palette stored in the video hardware.
 */

typedef struct _Ether_Hue         EtherHue;
typedef struct _Ether_Palette     EtherPalette;

struct _Ether_Hue
{
    unsigned char start;                   /**< starting index into palette */
    unsigned char maxshade;                /**< maximum number of shades */
};

struct _Ether_Palette
{
    unsigned char data[256][3];
    EtherHue huemap[256];
    int changed : 1;
};

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
