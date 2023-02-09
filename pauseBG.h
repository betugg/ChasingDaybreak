
//{{BLOCK(pauseBG)

//======================================================================
//
//	pauseBG, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 173 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 5536 + 2048 = 8096
//
//	Time-stamp: 2022-04-21, 21:16:00
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PAUSEBG_H
#define GRIT_PAUSEBG_H

#define pauseBGTilesLen 5536
extern const unsigned short pauseBGTiles[2768];

#define pauseBGMapLen 2048
extern const unsigned short pauseBGMap[1024];

#define pauseBGPalLen 512
extern const unsigned short pauseBGPal[256];

#endif // GRIT_PAUSEBG_H

//}}BLOCK(pauseBG)
