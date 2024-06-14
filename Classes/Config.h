//
// Created by ccr on 2024/6/12.
//

#ifndef RIPANDTEAR_CONFIG_H
#define RIPANDTEAR_CONFIG_H


using namespace std;

static const int DOOM_TAG = 110;  // DOOM tag
static const int MAP_TAG = 111;     // map tag
static int DOOM_WIDTH = 50;		// player img width
static int DOOM_HEIGHT = 100;	    // player img height
static int DOOM_SPEED = 30;  // player jump speed

static string MAP_TMX_FILE_PATH = "map/doomBattle.png";
static string MAP_BG_LAYER_NAME = "back";  // map background layer name



static string GAME_OVER_TIPS = "Game Over!";  // game failed tips words
static string GAME_SUCCESS = "Success!";      // game success tips words

//	fnt file path
static string TIPS_FNT_PATH = "fonts/futura-48.fnt";




#endif //RIPANDTEAR_CONFIG_H

