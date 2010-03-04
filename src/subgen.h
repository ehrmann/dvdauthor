/*
 * Copyright (C) 2002, 2003 Jan Panteltje <panteltje@yahoo.com>
 * With many changes by Scott Smith (trckjunky@users.sourceforge.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 * USA
 */
#include "subglobals.h"

typedef struct
{
    unsigned char r, g, b, t; // t=255 means opaque, t=0 means transparent
} palt;

typedef struct { /* representation of a raster image read from a file on disk */
    char *fname; /* name of file to read image from */
    unsigned char *img; /* one byte per pixel, index into pal */
    palt pal[256]; /* image colour table */
    int numpal; /* nr used entries in pal */
    int width,height; /* dimensions of image (will be even) */
} pict;

typedef struct {
    int x0,y0,x1,y1;
} rectangle;

typedef struct { /* representation of a menu button */
    char *name;
    int autoaction;
    rectangle r; /* bounds of button */
    char *up,*down,*left,*right; /* pointers to other buttons in corresponding directions, if any */
    int grp; /* which group button belongs to */
} button;

typedef struct { /* representation of a subpicture and associated buttons */
    unsigned int x0, y0; /* top-left coords of pixels actually present */
    unsigned int xd, yd;
    int spts, sd; // start pts, subtitle duration
    int forced;
    int numbuttons; /* nr entries in buttons */
    int numpal; /* nr entries used in masterpal */
    int autooutline,outlinewidth,autoorder;
    pict img; /* button image in "normal" state */
    pict hlt; /* button image in "highlighted" state (user has moved to button with remote) */
    pict sel; /* button image in "selected" state (user has hit OK key with button highlighted) */
    unsigned char *fimg; /* combined menu subpicture image built here */
    palt pal[4]; /* palette for fimg */
    palt masterpal[16];
    palt transparentc;
    int numgroups; /* how many button groups */
    int groupmap[3][4]; /* colour table for each button group, -1 for unused entries in each group */
    button *buttons; /* array of buttons */
    subtitle *sub_title;
} stinfo;

#define SUB_BUFFER_MAX      53220
#define SUB_BUFFER_HEADROOM     1024

extern unsigned char *sub;
extern int debug;
extern int have_textsub;
extern int have_transparent;
extern int transparent_color;

extern stinfo **spus;
extern int numspus;

extern int skip;

int calcY(const palt *p);
int calcCr(const palt *p);
int calcCb(const palt *p);

int findmasterpal(stinfo *s, const palt *p);
  /* returns the index in s->masterpal corresponding to colour p, allocating a
    new palette entry if not there already. */

// subgen-parse-xml

int spumux_parse(const char *fname);

// subgen-encode

int dvd_encode(stinfo *s);
int svcd_encode(stinfo *s);
int cvd_encode(stinfo *s);

// subgen-image

int process_subtitle(stinfo *s);
void image_init();
void image_shutdown();
