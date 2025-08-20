#ifndef CHASER_DEFINES_H
#define CHASER_DEFINES_H

#define PROXY_HOST "www7019ug.sakura.ne.jp"
#define PROXY_PORT "80"
#define TARGET_HOST "www7019ug.sakura.ne.jp"
#define TARGET_PORT "80"
#define USERNAME "zenpro2025037"
#define PASSWORD "5kbRZFSH"
#define ROOM_NUMBER "6426"//6416-6435 (6191)
#define ATTEMPT_LIMIT 10 //通信エラー発生時に再接続を試みる回数の上限

#endif

#pragma region マクロとか
#define DANGER			-5			//危険域
#define NOT_FOUND		-2			//未探索
#define CURB			-1			//場外
#define IN_OUT			-3			//場外の可能性がある
#define LAND			0			//土地
#define TARGET			1
#define BLOCK			2
#define HEART			3
#define GEODE           4


#define FOSSIL_LEFTUP		5		//化石
#define FOSSIL_RIGHTUP		6
#define FOSSIL_LEFTDOWN		7
#define FOSSIL_RIGHTDOWN	8

#define TRILOBITES	9				//三葉虫

#define WHITE	12					//ホワイト

#define STAR_RIGHT		20			//10マスワープ
#define STAR_LEFT		21
#define STAR_UP			22
#define STAR_DOWN		23

#define STAR_RIGHT_5	30			//5マスワープ
#define STAR_LEFT_5		31
#define STAR_UP_5		32
#define STAR_DOWN_5		33

#define BEDROCK_LEFT_6		40		//6マス岩盤
#define BEDROCK_RIGHT_6		41
#define BEDROCK_DOWN_6		42
#define BEDROCK_UP_6		43

#define BEDROCK_LEFT_15		50		//15マス岩盤
#define BEDROCK_RIGHT_15	51
#define BEDROCK_DOWN_15		52
#define BEDROCK_UP_15		53

#define BLACKSTAR_LEFTUP    60		//ブラックスター
#define BLACKSTAR_RIGHTUP   61
#define BLACKSTAR_LEFTDOWN  62
#define BLACKSTAR_RIGHTDOWN 63


//反転させたもの
#define REVERSAL_LAND				100
#define REVERSAL_TARGET				101
#define REVERSAL_BLOCK				102
#define REVERSAL_HEART				103
#define REVERSAL_NOTFOUND			-102

#define REVERSAL_FOSSIL_LEFTUP		105
#define REVERSAL_FOSSIL_RIGHTUP		106
#define REVERSAL_FOSSIL_LEFTDOWN	107
#define REVERSAL_FOSSIL_RIGHTDOWN	108

#define REVERSAL_TRILOBITES			109

#define REVERSAL_WHITE				112

#define REVERSAL_STAR_RIGHT			120
#define REVERSAL_STAR_LEFT			121
#define REVERSAL_STAR_UP			122
#define REVERSAL_STAR_DOWN			123

#define REVERSAL_STAR_RIGHT_5		130
#define REVERSAL_STAR_LEFT_5		131
#define REVERSAL_STAR_UP_5			132
#define REVERSAL_STAR_DOWN_5		133

#define REVERSAL_BEDROCK_LEFT_6		140
#define REVERSAL_BEDROCK_RIGHT_6	141
#define REVERSAL_BEDROCK_DOWN_6		142
#define REVERSAL_BEDROCK_UP_6		143

#define REVERSAL_BEDROCK_LEFT_15	150
#define REVERSAL_BEDROCK_RIGHT_15	151
#define REVERSAL_BEDROCK_DOWN_15	152
#define REVERSAL_BEDROCK_UP_15		153

#define Look			"Look"
#define Search			"Search"
#define Kei				"Kei"
#define Walk			"Walk"

#define Up				"Up"
#define Right			"Right"
#define Down			"Down"
#define Left			"Left"
#define Center			"Center"

#define Heart			"Heart"
#define Target			"Target"
#define	Block			"Block"
#define Curb			"Curb"

#define LEFT_UP(map , Y , X)		map[Y - 1][X - 1]
#define UP(map , Y , X)				map[Y - 1][X	]
#define RIGHT_UP(map , Y , X)		map[Y - 1][X + 1]
#define LEFT(map , Y , X)			map[Y	 ][X - 1]
#define CENTER(map , Y , X)			map[Y	 ][X	]
#define RIGHT(map , Y , X)			map[Y	 ][X + 1]
#define LEFT_DOWN(map , Y , X)		map[Y + 1][X - 1]
#define DOWN(map , Y , X)			map[Y + 1][X	]
#define RIGHT_DOWN(map , Y , X)		map[Y + 1][X + 1]
#define LEFT_UP_UP(map , Y , X)		map[Y - 2][X - 1]
#define RIGHT_UP_UP(map , Y , X)	map[Y - 2][X + 1]
#define LEFT_DOWN_DOWN(map , Y , X)	map[Y + 2][X - 1]
#define RIGHT_DOWN_DOWN(map , Y , X)map[Y + 2][X + 1]

#define UP3(map , Y , X)			map[Y - 3][X	]
#define LEFT3(map , Y , X)			map[Y	 ][X - 3]
#define RIGHT3(map , Y , X)			map[Y	 ][X + 3]
#define DOWN3(map , Y , X)			map[Y + 3][X	]

#define SIZE			100


#define INF				-1000

#define INDEX			8	//斜めを抜いた状態
#define E_INDEX			12	//lookで見なければならない範囲

#define Enemy(val)			((val == 1000 || val == 2000 || val == 3000 || val == 4000 || val == 5000 || val == 6000 || val == 7000 || val == 8000) && val != me)//val != me && val % 1000 == 0 && val != 0
#define WARP(val)			(val == STAR_RIGHT || val == STAR_LEFT || val == STAR_UP || val == STAR_DOWN || val == STAR_RIGHT_5 || val == STAR_LEFT_5 || val == STAR_UP_5 || val == STAR_DOWN_5)
#define NOT_WARP(val)		(val != STAR_RIGHT && val != STAR_LEFT && val != STAR_UP && val != STAR_DOWN && val != STAR_RIGHT_5 && val != STAR_LEFT_5 && val != STAR_UP_5 && val != STAR_DOWN_5)
#define BEDROCK(val)		(val == BEDROCK_LEFT_6 || val == BEDROCK_RIGHT_6 || val == BEDROCK_DOWN_6 || val == BEDROCK_UP_6 || val == BEDROCK_LEFT_15 || val == BEDROCK_RIGHT_15 || val == BEDROCK_DOWN_15 || val == BEDROCK_UP_15)
#define BLACKSTAR(val)		(val == BLACKSTAR_LEFTUP || val == BLACKSTAR_RIGHTUP || val == BLACKSTAR_LEFTDOWN || val == BLACKSTAR_RIGHTDOWN)

#pragma endregion
