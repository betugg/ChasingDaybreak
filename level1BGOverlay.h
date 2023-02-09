
//{{BLOCK(level1BGOverlay)

//======================================================================
//
//	level1BGOverlay, 1024x256@4, 
//	+ palette 256 entries, not compressed
//	+ 56 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 128x32 
//	Total size: 512 + 1792 + 8192 = 10496
//
//	Time-stamp: 2022-04-21, 17:44:41
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LEVEL1BGOVERLAY_H
#define GRIT_LEVEL1BGOVERLAY_H

#define level1BGOverlayTilesLen 1792
extern const unsigned short level1BGOverlayTiles[896];

#define level1BGOverlayMapLen 8192
extern const unsigned short level1BGOverlayMap[4096];

#define level1BGOverlayPalLen 512
extern const unsigned short level1BGOverlayPal[256];

#endif // GRIT_LEVEL1BGOVERLAY_H

//}}BLOCK(level1BGOverlay)
