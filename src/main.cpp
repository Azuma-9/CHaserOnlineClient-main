#include "../include/CHaser_Main.h"
#include <sstream>
using namespace std;

vector<vector<int>>map(100, vector<int>(100, NOT_FOUND));//周囲情報を格納する二次元配列
vector<vector<int>>mapp(100, vector<int>(100, 0));//スコアを格納する二次元配列
vector<vector<int>>seen(100, vector<int>(100, 0));//既に通ったマスか否かを管理（回数も管理）
vector<int>mp(5, 0); //例外処理をした時のスコアを格納する配列

int y = 50, x = 50;//座標の初期値
int miny = 50, maxy = 50, minx = 50, maxx = 50;

string returnCode;//サーバーからの戻り値（string）を格納
vector<int>returnNum(100, 0);//キャストした戻り値を格納


int dy[] = { -1,0,0,1 };//上　左　右　下　
int dx[] = { 0,-1,1,0 };//上　左　右　下

//戻り値をintにキャスト
vector<int> ParseInt(string t) {
	vector<int>vec;
	stringstream ss(t);
	string s;
	while (getline(ss, s, ',')) {
		try {
			vec.push_back(stoi(s));
		}
		catch (exception e) {//例外が発生した場合の処理
			vec.push_back(-1);
		}
	}

	while (vec.size() < 9)vec.push_back(-1);

	return vec;
}

//周囲情報をmap配列に格納
void MapUpdate(string cmd) {
	int sy = -1, ey = 1;
	int sx = -1, ex = 1;

	int index = 0;

	if (cmd == "pu3lu") { sy = -3; ey = -1; sx = -1; ex = 1; }
	else if (cmd == "pd3ld") { sy = 1; ey = 3; sx = -1; ex = 1; }
	else if (cmd == "pl3ll") { sy = -1; ey = 1; sx = -3; ex = -1; }
	else if (cmd == "pr3lr") { sy = -1; ey = 1; sx = 1; ex = 3; }
	else if (cmd == "pu3su") { sy = -9; ey = -1; sx = 0; ex = 0; }
	else if (cmd == "pd3sd") { sy = 1; ey = 9; sx = 0; ex = 0; }
	else if (cmd == "pl3sl") { sy = 0; ey = 0; sx = -9; ex = -1; }
	else if (cmd == "pr3sr") { sy = 0; ey = 0; sx = 1; ex = 9; }
	else if (cmd == "du") { sy = -3; ey = -1; sx = -1; ex = 1; }
	else if (cmd == "dd") { sy = 1; ey = 3; sx = -1; ex = 1; }
	else if (cmd == "dl") { sy = -1; ey = 1; sx = -3; ex = -1; }
	else if (cmd == "dr") { sy = -1; ey = 1; sx = 1; ex = 3; }
	else if (cmd == "dru") { sy = -3; ey = -1; sx = 1; ex = 3; }
	else if (cmd == "drd") { sy = 1; ey = 3; sx = 1; ex = 3; }
	else if (cmd == "dlu") { sy = -3; ey = -1; sx = -3; ex = -1; }
	else if (cmd == "dld") { sy = 1; ey = 3; sx = 1; ex = 3; }

	for (int i = sy; i <= ey; i++) {
		for (int j = sx; j <= ex; j++) {
			int iy = y + i;
			int jx = x + j;
			map[iy][jx] = returnNum[index];
			index++;
		}
	}
}

//座標を移動
void Coordinate(string cmd) {

	map[y][x] = 8;
	mapp[y][x] = 100;
	if (cmd == "gru" || cmd == "wu" || cmd == "du" || cmd == "pl3ll" || cmd == "pl3sl")y--;
	else if (cmd == "grl" || cmd == "wl" || cmd == "dl" || cmd == "pd3ld" || cmd == "pd3sd")x--;
	else if (cmd == "grr" || cmd == "wr" || cmd == "dr" || cmd == "pu3lu" || cmd == "pu3su")x++;
	else if (cmd == "grd" || cmd == "wd" || cmd == "dd" || cmd == "pr3lr" || cmd == "pr3sr")y++;
	else if (cmd == "pd2w2u")y -= 2;
	else if (cmd == "pu2w2d")y += 2;
	else if (cmd == "pr2w2l")x -= 2;
	else if (cmd == "pl2w2r")x += 2;
	else if (cmd == "w3u")y -= 3;
	else if (cmd == "w3d")y += 3;
	else if (cmd == "w3l")x -= 3;
	else if (cmd == "w3r")x += 3;
	else if (cmd == "dlu") { y--; x--; }
	else if (cmd == "dru") { y--; x++; }
	else if (cmd == "dld") { y++; x--; }
	else if (cmd == "drd") { y++; x++; }
	else if (cmd == "pd2wu")y--;
	else if (cmd == "pu2wd")y++;
	else if (cmd == "pr2wl")x--;
	else if (cmd == "pl2wr")x++;
	else if (cmd == "pru2wld") { y++; x--; }
	else if (cmd == "plu2wrd") { y++; x++; }
	else if (cmd == "prd2wlu") { y--; x--; }
	else if (cmd == "pld2wru") { y--; x++; }
	else if (cmd == "keilu") { y -= 2; x--; }
	else if (cmd == "keiru") { y -= 2; x++; }
	else if (cmd == "keild") { y += 2; x--; }
	else if (cmd == "keird") { y += 2; x++; }
}

/// <summary>
/// 
/// </summary>
/// <param name="value"></param>
/// <param name="my">目的地のy座標</param>
/// <param name="mx">目的地のx座標</param>
/// <param name="ci">マスのy座標</param>
/// <param name="cj">マスのx座標</param>
void color(int value, int my, int mx, int ci, int cj) {
	if (ci == y && cj == x)printf("\x1b[36m");//明るい青緑文字
	else if (my == ci && mx == cj)printf("\x1b[44m");//背景色を青
	else if (value == CURB) printf("\x1b[41m");//背景色を赤
	else if (value == WHITE) printf("\x1b[31m");//赤文字
	else if (value == DANGER) printf("\x1b[41m");//背景色を赤
	else if (value == NOT_FOUND) printf("\x1b[0m");//リセット
	else if (value == GEODE) printf("\x1b[31m");//赤文字
	else if (value == TARGET) printf("\x1b[32m");//緑文字
	else if (value == HEART) printf("\x1b[32m");//緑文字
	else if (value == TRILOBITES) printf("\x1b[33m");//黄色文字
	else if (value == FOSSIL_LEFTUP) printf("\x1b[33m");//黄色文字
	else if (value == FOSSIL_RIGHTUP) printf("\x1b[33m");//黄色文字
	else if (value == FOSSIL_LEFTDOWN) printf("\x1b[33m");//黄色文字
	else if (value == FOSSIL_RIGHTDOWN) printf("\x1b[33m");//黄色文字
	else if (value >= 70 && value <= 79) printf("\x1b[33m");//黄色文字
	else if (value < 0)printf("\x1b[31m");//赤文字
	else if (value >= 0)printf("\x1b[32m");//緑文字
	if (ci == y && cj == x) {

	}
	else if (seen[ci][cj] >= 1)printf("\x1b[48;5;153m");//背景色を水色
}

//スコア表示
void ShowMapping() {
	for (int i = y - 3; i <= y + 3; i++) {
		for (int j = x - 3; j <= x + 3; j++) {
			if (i == y && j == x)printf("   Ｍ,");
			else {
				printf("%5d,", mapp[i][j]);
			}
		}
		printf("\n");
	}

	/*printf("%d,%d,%d\n", returnNum[0], returnNum[1], returnNum[2]);
	printf("%d,%d,%d\n", returnNum[3], returnNum[4], returnNum[5]);
	printf("%d,%d,%d\n", returnNum[6], returnNum[7], returnNum[8]);
	printf("\n");*/
}

void ShowMapping2() {
	for (int i = y - 3; i <= y + 3; i++) {
		for (int j = x - 3; j <= x + 3; j++) {
			printf("%5d,", map[i][j]);
		}
		printf("\n");
	}
}

//マップ表示
void MapDisplay(int my, int mx) {

	printf("\n----------------------------------------------------\n");
	for (int i = miny - 3; i <= maxy + 3; i++) {
		for (int j = minx - 3; j <= maxx + 3; j++) {
			if (i < 0 || i>99 || j < 0 || j>99)continue;
			color(map[i][j], my, mx, i, j);//
			if (i == y && j == x)
				printf("Ｍ");

			else if (map[i][j] == 1000 && (i != y || j != x))
				printf("Ｃ");

			else if (map[i][j] == 2000 && (i != y || j != x))
				printf("Ｈ");

			else if (map[i][j] == 3000 && (i != y || j != x))
				printf("ａ");

			else if (map[i][j] == 4000 && (i != y || j != x))
				printf("ｓ");

			else if (map[i][j] == 5000 && (i != y || j != x))
				printf("ｅ");

			else if (map[i][j] == 6000 && (i != y || j != x))
				printf("ｒ");

			else if (map[i][j] == 7000 && (i != y || j != x))
				printf("Ｏ");

			else if (map[i][j] == 8000 && (i != y || j != x))
				printf("ｎ");

			else if (map[i][j] == CURB)
				printf("－");

			else if (map[i][j] == LAND)
				printf("□");

			else if (map[i][j] == TARGET)
				printf("★");

			else if (map[i][j] == BLOCK)
				printf("■");

			else if (map[i][j] == HEART)
				printf("▼");

			else if (map[i][j] == NOT_FOUND)
				printf("？");

			else if (map[i][j] == DANGER)
				printf("！");

			else if (map[i][j] >= 70 && map[i][j] <= 79)
				printf("〇");

			else if (map[i][j] == STAR_UP)
				printf("㊤");

			else if (map[i][j] == STAR_RIGHT)
				printf("㊨");

			else if (map[i][j] == STAR_DOWN)
				printf("㊦");

			else if (map[i][j] == STAR_LEFT)
				printf("㊧");

			else if (map[i][j] == STAR_UP_5)
				printf("∵");

			else if (map[i][j] == STAR_RIGHT_5)
				printf("∵");

			else if (map[i][j] == STAR_DOWN_5)
				printf("∵");

			else if (map[i][j] == STAR_LEFT_5)
				printf("∵");

			else if (map[i][j] == GEODE || map[i][j] == 11)
				printf("＃");

			else if (map[i][j] == FOSSIL_LEFTUP)
				printf("┌ ");

			else if (map[i][j] == FOSSIL_RIGHTUP)
				printf(" ┐");

			else if (map[i][j] == FOSSIL_LEFTDOWN)
				printf("└ ");

			else if (map[i][j] == FOSSIL_RIGHTDOWN)
				printf(" ┘");

			else if (map[i][j] == TRILOBITES)
				printf("￥");

			else if (map[i][j] == WHITE || map[i][j] == 10)
				printf("×");

			else if (map[i][j] == IN_OUT)
				printf("※");

			else if (map[i][j] == BEDROCK_UP_6 || map[i][j] == BEDROCK_DOWN_6 || map[i][j] == BEDROCK_RIGHT_6 || map[i][j] == BEDROCK_LEFT_6)
				printf("◆");

			else if (map[i][j] == BEDROCK_UP_15 || map[i][j] == BEDROCK_DOWN_15 || map[i][j] == BEDROCK_RIGHT_15 || map[i][j] == BEDROCK_LEFT_15)
				printf("◆");

			else if (map[i][j] == BLACKSTAR_LEFTUP)
				printf("↑");

			else if (map[i][j] == BLACKSTAR_RIGHTUP)
				printf("→");

			else if (map[i][j] == BLACKSTAR_LEFTDOWN)
				printf("←");

			else if (map[i][j] == BLACKSTAR_RIGHTDOWN)
				printf("↓");


			else printf("%2d", map[i][j]);


			printf("\x1b[0m");

		}
		puts("");
	}
}

//上下左右に移動する場合どのコマンドを使うか決める
string DecideCommand(int dy, int dx) {
	int cnt = 0;//未探索のマス（情報が不明）をカウントするための変数
	int cnt2 = 0;//未探索のマス（情報が不明）をカウントするための変数
	int cnt3 = 0;//未探索のマス（情報が不明）をカウントするための変数

	if (dy == y - 1 && dx == x) {//上に行く場合

		//3マス先まで探索して未探索マスの数でwu,du
		for (int i = -4; i <= -2; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[y + i][x + j] == NOT_FOUND)cnt++;
			}
		}
		//4つ以上未探索だったら
		if (cnt >= 6)return "du";
		else {
			for (int i = -2; i <= 0; i++) {
				for (int j = -3; j <= -1; j++) {
					if (map[y + i][x + j] == NOT_FOUND)cnt2++;
				}
			}
			//4つ以上未探索だったら
			if (cnt2 >= 4)return "pl3ll";
			else {
				for (int i = -1; i <= -1; i++) {
					for (int j = -9; j <= -1; j++) {
						if (map[y + i][x + j] == NOT_FOUND)cnt3++;
					}
				}
				//4つ以上未探索だったら
				if (cnt3 >= 4)return "pl3sl";
				else return "wu";
			}
		}
	}
	else if (dy == y + 1 && dx == x) {//下に行く場合

		for (int i = 2; i <= 4; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[y + i][x + j] == NOT_FOUND)cnt++;
			}
		}

		if (cnt >= 6)return "dd";
		else {
			for (int i = 0; i <= 2; i++) {
				for (int j = 1; j <= 3; j++) {
					if (map[y + i][x + j] == NOT_FOUND)cnt2++;
				}
			}
			if (cnt2 >= 4)return "pr3lr";
			else {
				for (int i = 1; i <= 1; i++) {
					for (int j = 1; i <= 9; i++) {
						if (map[y + i][x + j] == NOT_FOUND)cnt3++;
					}
				}
				if (cnt3 >= 4)return "pr3sr";
				else return "wd";
			}
		}
	}
	else if (dy == y && dx == x - 1) {//左に行く場合

		for (int i = -1; i <= 1; i++) {
			for (int j = -4; j <= -2; j++) {
				if (map[y + i][x + j] == NOT_FOUND)cnt++;
			}
		}

		if (cnt >= 6)return "dl";
		else {
			for (int i = 1; i <= 3; i++) {
				for (int j = -2; j <= 0; j++) {
					if (map[y + i][x + j] == NOT_FOUND)cnt2++;
				}
			}
			if (cnt2 >= 4)return "pd3ld";
			else {
				for (int i = 1; i <= 9; i++) {
					for (int j = -1; j <= -1; j++) {
						if (map[y + i][x + j] == NOT_FOUND)cnt3++;
					}
				}
				if (cnt3 >= 4)return "pd3sd";
				else return "wl";
			}
		}
	}
	else if (dy == y && dx == x + 1) {//右に行く場合
		for (int i = -1; i <= 1; i++) {
			for (int j = 2; j <= 4; j++) {
				if (map[y + i][x + j] == NOT_FOUND)cnt++;
			}
		}

		if (cnt >= 6)return "dr";
		else {
			for (int i = -3; i <= -1; i++) {
				for (int j = 0; j <= 2; j++) {
					if (map[y + i][x + j] == NOT_FOUND)cnt2++;
				}
			}
			if (cnt2 >= 4)return "pu3lu";
			else {
				for (int i = -9; i <= -1; i++) {
					for (int j = 1; j <= 1; j++) {
						if (map[y + i][x + j] == NOT_FOUND)cnt3++;
					}
				}
				if (cnt3 >= 4)return "pu3su";
				else return "wr";
			}
		}
	}
}

//ワープ用関数
void Warp(int warpnum) {
	if (warpnum == STAR_RIGHT)
		x += 10;

	else if (warpnum == STAR_LEFT)
		x -= 10;

	else if (warpnum == STAR_UP)
		y -= 10;

	else if (warpnum == STAR_DOWN)
		y += 10;

	else if (warpnum == STAR_RIGHT_5)
		x += 5;

	else if (warpnum == STAR_LEFT_5)
		x -= 5;

	else if (warpnum == STAR_UP_5)
		y -= 5;

	else if (warpnum == STAR_DOWN_5)
		y += 5;

}

//ブラックスター用関数
void BlackStar(int warpnum) {
	switch (warpnum)
	{
	case BLACKSTAR_LEFTUP:
		x += 9;
		break;
	case BLACKSTAR_RIGHTUP:
		x -= 9;
		y += 10;
		break;
	case BLACKSTAR_LEFTDOWN:
		x += 10;
		y -= 1;
		break;
	case BLACKSTAR_RIGHTDOWN:
		x -= 10;
		y -= 9;
		break;
	default:
		break;
	}
}

//岩盤用関数
void BedRock(int warpnum) {
	if (warpnum == BEDROCK_LEFT_6) {
		x += 6;
	}

	else if (warpnum == BEDROCK_RIGHT_6) {
		x -= 6;
	}

	else if (warpnum == BEDROCK_DOWN_6) {
		y -= 6;
	}

	else if (warpnum == BEDROCK_UP_6) {
		y += 6;
	}

	else if (warpnum == BEDROCK_LEFT_15) {
		x += 15;
	}

	else if (warpnum == BEDROCK_RIGHT_15) {
		x -= 15;
	}

	else if (warpnum == BEDROCK_DOWN_15) {
		y -= 15;
	}

	else if (warpnum == BEDROCK_UP_15) {
		y += 15;
	}
}

//ワープ先の周囲情報を確認
bool To_Warp(int warpnum, int Y, int X) {
	int ans = 0;
	if (warpnum == STAR_RIGHT || warpnum == STAR_RIGHT_5) {
		if (warpnum == STAR_RIGHT)X += 10;
		else X += 5;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
	else if (warpnum == STAR_LEFT || warpnum == STAR_LEFT_5) {
		if (warpnum == STAR_LEFT)X -= 10;
		else X -= 5;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
	else if (warpnum == STAR_UP || warpnum == STAR_UP_5) {
		if (warpnum == STAR_UP)Y -= 10;
		else Y -= 5;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
	else if (warpnum == STAR_DOWN || warpnum == STAR_DOWN_5) {
		if (warpnum == STAR_DOWN)Y += 10;
		else Y += 5;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
}

//ブラックスター先の周囲情報を確認
bool To_BlackStar(int warpnum, int Y, int X) {
	int ans = 0;
	if (warpnum == BLACKSTAR_LEFTUP) {
		X += 9;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
	else if (warpnum == BLACKSTAR_RIGHTUP) {
		X -= 9;
		Y += 10;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
	else if (warpnum == BLACKSTAR_LEFTDOWN) {
		X += 10;
		Y -= 1;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
	else if (warpnum == BLACKSTAR_RIGHTDOWN) {
		X -= 10;
		Y -= 9;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
}

//岩盤先の周囲情報を確認
bool To_BedRock(int warpnum, int Y, int X) {
	int ans = 0;
	if (warpnum == BEDROCK_LEFT_6) {
		X += 6;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
	else if (warpnum == BEDROCK_RIGHT_6) {
		X -= 6;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
	else if (warpnum == BEDROCK_DOWN_6) {
		Y -= 6;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
	else if (warpnum == BEDROCK_UP_6) {
		Y += 6;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
	else if (warpnum == BEDROCK_LEFT_15) {
		X += 15;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
	else if (warpnum == BEDROCK_RIGHT_15) {
		X -= 15;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
	else if (warpnum == BEDROCK_DOWN_15) {
		Y -= 15;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
	else if (warpnum == BEDROCK_UP_15) {
		Y += 15;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (map[Y + i][X + j] == -2)ans++;
			}
		}

		if (ans >= 4)return true;
		else return false;
	}
}

//移動可能なマスに行くためのペア
vector<pair<int, int>>dist = {//0-19
	{-1,0},{0,-1},{0,1},{1,0},
	{-1,-1}, {-1,1}, {1,-1},
	{1,1}, {-2,0}, {0,-2}, {0,2}, {2,0}, {-2,-1},
	{-2,1}, {2,-1}, {2,1},
	{-3,0}, {0,-3}, {0,3},
	{3,0}
};

//左上
vector<pair<int, int>>directions_1 = {
	{-3,-3},
	{-3,-2},
	{-3,-1},
	{-2,-3},
	{-2,-2},
	{-1,-3},
	{-1,-2}
};

//右上
vector<pair<int, int>>directions_2 = {
	{-3,1},
	{-3,2},
	{-3,3},
	{-2,2},
	{-2,3},
	{-1,2},
	{-1,3}
};

//左下
vector<pair<int, int>>directions_3 = {
	{1,-3},
	{1,-2},
	{2,-3},
	{2,-2},
	{3,-3},
	{3,-2},
	{3,-1}
};

//右下
vector<pair<int, int>>directions_4 = {
	{1,2},
	{1,3},
	{2,2},
	{2,3},
	{3,1},
	{3,2},
	{3,3}
};

int main() {
	//一番最初にgrコマンドを送信
	returnCode = client.GetReadyCheck("gr");
	//int型に変換
	returnNum = ParseInt(returnCode);
	//コマンドに応じてy,x座標を移動させる
	Coordinate("gr");
	//コマンドによって得た周囲情報をmap配列に格納する
	MapUpdate("gr");
	//既に通ったマスの回数を増やす
	seen[y][x]++;
	int turn = 0;//ターン数をカウントする変数
	string cmd = "";//行動コマンド
	string cmd2 = "";//grコマンド
	while (1) {
		turn++;
		double dir;//距離
		double score = -10000;//最大スコアの格納
		pair<int, int>p;//最大スコアマスのy,x座標
		bool put = false;//putするか
		bool warp = false;//warpするか
		int z_check = 0;
		bool z_c = false;
		bool reigai = false;
		int warpnum = 0;
		int dirY = 0, dirX = 0;
		int zy = 0, zx = 0;
		vector<bool>z(4, false);

		//周囲に敵クライアントがいるかチェック
		for (int i = 0; i < 9; i++) {
			if (i == 4 || i == 0 || i == 2 || i == 6 || i == 8)continue;
			if (1000 <= returnNum[i])put = true;
		}

		//mapのアイテムに応じてスコアを付ける
		for (int i = y - 3; i <= y + 3; i++) {
			for (int j = x - 3; j <= x + 3; j++) {
				if (i == y && j == x)continue;
				if (map[i][j] == NOT_FOUND) mapp[i][j] = -1000;
				else if (map[i][j] == 70) mapp[i][j] = 400;
				else if (map[i][j] == 71) mapp[i][j] = 401;
				else if (map[i][j] == 72) mapp[i][j] = 402;
				else if (map[i][j] == 73) mapp[i][j] = 403;
				else if (map[i][j] == 74) mapp[i][j] = 404;
				else if (map[i][j] == 75) mapp[i][j] = 405;
				else if (map[i][j] == 76) mapp[i][j] = 406;
				else if (map[i][j] == 77) mapp[i][j] = 407;
				else if (map[i][j] == 78) mapp[i][j] = 408;
				else if (map[i][j] == 79) mapp[i][j] = 409;
				else if (map[i][j] == 0) mapp[i][j] = -1000;
				else if (map[i][j] == 1) mapp[i][j] = 50;
				else if (map[i][j] == 2) mapp[i][j] = -1000;
				else if (map[i][j] == 3) mapp[i][j] = 10;
				else if (map[i][j] == 4) mapp[i][j] = -1000;
				else if (map[i][j] == 5) mapp[i][j] = 200;
				else if (map[i][j] == 6) mapp[i][j] = 175;
				else if (map[i][j] == 7) mapp[i][j] = 150;
				else if (map[i][j] == 8) mapp[i][j] = 100;
				else if (map[i][j] == 9) mapp[i][j] = 100;
				else if (20 <= map[i][j] && map[i][j] <= 23) mapp[i][j] = 230;
				else if (30 <= map[i][j] && map[i][j] <= 33) mapp[i][j] = 230;
				else if (60 <= map[i][j] && map[i][j] <= 63) mapp[i][j] = 230;
				else if (40 <= map[i][j] && map[i][j] <= 53) mapp[i][j] = 90;
				else if (map[i][j] == 12) mapp[i][j] = -2000;
				else if (map[i][j] == -1)mapp[i][j] = -2000;
			}
		}

		//最後にgrで移動した方向と違うマスのスコアを引く
		for (int i = y - 3; i <= y + 3; i++) {
			for (int j = x - 3; j <= x + 3; j++) {
				if (i == y && j == x)continue;

				if (cmd2 == "gru") {//最後に上に移動した
					if (i == y - 1 && j == x) {}
					else mapp[i][j] -= 10;
				}
				else if (cmd2 == "grl") {
					if (i == y && j == x - 1) {}
					else mapp[i][j] -= 10;
				}
				else if (cmd2 == "grr") {
					if (i == y && j == x + 1) {}
					else mapp[i][j] -= 10;
				}
				else if (cmd2 == "grd") {
					if (i == y + 1 && j == x) {}
					else mapp[i][j] -= 10;
				}
			}
		}

		//現在の座標からの距離分スコアを引く
		for (int i = 0; i < 20; i++) {
			int iy = y + dist[i].first;
			int jx = x + dist[i].second;
			//ジオーメタルの場合少し減らす
			if (70 <= map[iy][jx] && map[iy][jx] <= 79) {
				if (abs(y - iy) == 3 || abs(x - jx) == 3)mapp[iy][jx] -= 30;//w3
				if (abs(y - iy) == 2 && abs(x - jx) == 1)mapp[iy][jx] -= 30;//kei
				if (abs(y - iy) == 2 && abs(x - jx) == 0)mapp[iy][jx] -= 20;//p2w2
				if (abs(y - iy) == 0 && abs(x - jx) == 2)mapp[iy][jx] -= 20;//p2w2
				if (abs(y - iy) == 1 && abs(x - jx) == 1)mapp[iy][jx] -= 10;//
			}
			else {
				if (abs(y - iy) == 3 || abs(x - jx) == 3)mapp[iy][jx] -= 30;//w3
				if (abs(y - iy) == 2 && abs(x - jx) == 1)mapp[iy][jx] -= 60;//kei
				if (abs(y - iy) == 2 && abs(x - jx) == 0)mapp[iy][jx] -= 20;//p2w2
				if (abs(y - iy) == 0 && abs(x - jx) == 2)mapp[iy][jx] -= 20;//p2w2
				if (abs(y - iy) == 1 && abs(x - jx) == 1)mapp[iy][jx] -= 10;
			}

			//ワープ先の周囲情報を確認
			if (20 <= map[iy][jx] && map[iy][jx] <= 23 || 30 <= map[iy][jx] && map[iy][jx] <= 33 || 40 <= map[iy][jx] && map[iy][jx] <= 53) {
				if (!To_Warp(map[iy][jx], iy, jx)) {
					mapp[iy][jx] -= 100;
				}
			}

			//ブラックスター先の周囲情報を確認
			if (60 <= map[iy][jx] && map[iy][jx] <= 63) {
				if (!To_BlackStar(map[iy][jx], iy, jx)) {
					mapp[iy][jx] -= 100;
				}
			}

			//岩盤先の周囲情報を確認
			if (40 <= map[iy][jx] && map[iy][jx] <= 53) {
				if (!To_BedRock(map[iy][jx], iy, jx)) {
					mapp[iy][jx] -= 100;
				}
			}

			//既に通ったマスの場合スコアを引く
			if (seen[iy][jx] >= 1) mapp[iy][jx] = mapp[iy][jx] - (seen[iy][jx] * 10);
		}

		//ワープマスを既に通ってる場合
		for (int i = 0; i < 20; i++) {
			int jy = y + dist[i].first;
			int ix = x + dist[i].second;
			if (20 <= map[jy][ix] && map[jy][ix] <= 23 && seen[jy][ix] >= 1)mapp[jy][ix] -= 30;
			else if (30 <= map[jy][ix] && map[jy][ix] <= 33 && seen[jy][ix] >= 1)mapp[jy][ix] -= 30;
			else if (60 <= map[jy][ix] && map[jy][ix] <= 63 && seen[jy][ix] >= 1)mapp[jy][ix] -= 30;
		}

		//一番スコアが高いマスに移動する
		for (int i = 0; i < 20; i++) {
			if (score < mapp[y + dist[i].first][x + dist[i].second]) {
				score = mapp[y + dist[i].first][x + dist[i].second];
				p.first = y + dist[i].first;
				p.second = x + dist[i].second;
				if (20 <= map[y + dist[i].first][x + dist[i].second] && map[y + dist[i].first][x + dist[i].second] <= 23 || 30 <= map[y + dist[i].first][x + dist[i].second] && map[y + dist[i].first][x + dist[i].second] <= 33) {
					warp = true;
					warpnum = map[y + dist[i].first][x + dist[i].second];
				}
				else if (60 <= map[y + dist[i].first][x + dist[i].second] && map[y + dist[i].first][x + dist[i].second] <= 63) {
					warp = true;
					warpnum = map[y + dist[i].first][x + dist[i].second];
				}
				else if (40 <= map[y + dist[i].first][x + dist[i].second] && map[y + dist[i].first][x + dist[i].second] <= 53) {
					warp = true;
					warpnum = map[y + dist[i].first][x + dist[i].second];
				}
				//ワープマスじゃない場合はfalseに戻す
				else {
					warp = false;
				}
			}
		}

		cout << 1 << endl;

		//putする場合
		if (put) {
			if (returnNum[1] >= 1000)cmd = "pu2w2d";
			else if (returnNum[3] >= 1000)cmd = "pl2w2r";
			else if (returnNum[5] >= 1000)cmd = "pr2w2l";
			else if (returnNum[7] >= 1000)cmd = "pd2w2u";
		}
		else {
			//現在の座標と最もスコアが高いマスの距離
			dirY = abs(p.first - y);
			dirX = abs(p.second - x);
			if (dirY == 3 && dirX == 0) {//w3u || w3d
				if (p.first == y - 3)cmd = "w3u";
				else cmd = "w3d";
			}
			else if (dirY == 0 && dirX == 3) {//w3l || w3r
				if (p.second == x - 3)cmd = "w3l";
				else cmd = "w3r";
			}
			else if (dirY == 2 && dirX == 0) {//pd2w2u || pu2w2d
				if (p.first == y - 2)cmd = "pd2w2u";
				else cmd = "pu2w2d";
			}
			else if (dirY == 0 && dirX == 2) {//pr2w2l || pl2w2r
				if (p.second == x - 2)cmd = "pr2w2l";
				else cmd = "pl2w2r";
			}
			else if (p.first == y - 2 && p.second == x - 1) {//keilu
				cmd = "keilu";
			}
			else if (p.first == y - 2 && p.second == x + 1) {//keiru
				cmd = "keiru";
			}
			else if (p.first == y + 2 && p.second == x - 1) {//keild
				cmd = "keild";
			}
			else if (p.first == y + 2 && p.second == x + 1) {//keird
				cmd = "keird";
			}
			else if (p.first == y - 1 && p.second == x - 1) {//dlu
				cmd = "dlu";
			}
			else if (p.first == y - 1 && p.second == x + 1) {//dru
				cmd = "dru";
			}
			else if (p.first == y + 1 && p.second == x - 1) {//dld
				cmd = "dld";
			}
			else if (p.first == y + 1 && p.second == x + 1) {//drd
				cmd = "drd";
			}
			else if (dirY == 1 && dirX == 0) {//上 || 下に行ける場合
				cmd = DecideCommand(p.first, p.second);
			}
			else if (dirY == 0 && dirX == 1) {//左 || 右に行ける場合
				cmd = DecideCommand(p.first, p.second);
			}

			//行動可能範囲内のすべての得点が335以下だった場合
			for (int i = 0; i < 20; i++) {
				int iy = y + dist[i].first;
				int jx = x + dist[i].second;
				if (mapp[iy][jx] <= 335)z_check++;
			}

			if (z_check >= 20) {
				//現在の位置から一番近い335以上のマスに近づくように移動(マイナスは避けて) || 
				// 未探索エリアが多いマスの近くに移動

				for (int i = 0; i < 7; i++) {
					int iy = y + directions_1[i].first;
					int jx = x + directions_1[i].second;
					if (mapp[iy][jx] > 335) {
						z[0] = true;
						break;
					}
				}
				for (int i = 0; i < 7; i++) {
					int iy = y + directions_2[i].first;
					int jx = x + directions_2[i].second;
					if (mapp[iy][jx] > 335) {
						z[1] = true;
						break;
					}
				}
				for (int i = 0; i < 7; i++) {
					int iy = y + directions_3[i].first;
					int jx = x + directions_3[i].second;
					if (mapp[iy][jx] > 335) {
						z[2] = true;
						break;
					}
				}
				for (int i = 0; i < 7; i++) {
					int iy = y + directions_4[i].first;
					int jx = x + directions_4[i].second;
					if (mapp[iy][jx] > 335) {
						z[3] = true;
						break;
					}
				}

				if (z[0]) {
					mp[0] = mapp[y - 2][x - 1];
					mp[1] = mapp[y - 1][x - 1];
					mp[2] = mapp[y][x - 1];
					mp[3] = mapp[y][x - 2];
					mp[4] = mapp[y][x - 3];
					sort(mp.rbegin(), mp.rend());
					for (int i = 0; i < 5; i++) {
						if (mp[i] == mapp[y - 2][x - 1] && mp[i] > 0) {
							cmd = "keilu";
							p.first = y - 2;
							p.second = x - 1;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y - 1][x - 1] && mp[i] > 0) {
							cmd = "dlu";
							p.first = y - 1;
							p.second = x - 1;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y][x - 1] && mp[i] > 0) {
							cmd = "wl";
							p.first = y;
							p.second = x - 1;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y][x - 2] && mp[i] > 0) {
							cmd = "pr2w2l";
							p.first = y;
							p.second = x - 2;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y][x - 3] && mp[i] > 0) {
							cmd = "w3l";
							p.first = y;
							p.second = x - 3;
							reigai = true;
							break;
						}
					}
				}

				if (z[1]) {
					mp[0] = mapp[y - 2][x + 1];
					mp[1] = mapp[y - 1][x + 1];
					mp[2] = mapp[y][x + 1];
					mp[3] = mapp[y][x + 2];
					mp[4] = mapp[y][x + 3];
					sort(mp.rbegin(), mp.rend());
					for (int i = 0; i < 5; i++) {
						if (mp[i] == mapp[y - 2][x + 1] && mp[i] > 0) {
							cmd = "keiru";
							p.first = y - 2;
							p.second = x + 1;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y - 1][x + 1] && mp[i] > 0) {
							cmd = "dru";
							p.first = y - 1;
							p.second = x + 1;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y][x + 1] && mp[i] > 0) {
							cmd = "wr";
							p.first = y;
							p.second = x + 1;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y][x + 2] && mp[i] > 0) {
							cmd = "pl2w2r";
							p.first = y;
							p.second = x + 2;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y][x + 3] && mp[i] > 0) {
							cmd = "w3r";
							p.first = y;
							p.second = x + 3;
							reigai = true;
							break;
						}
					}
				}

				if (z[2]) {
					mp[0] = mapp[y][x - 3];
					mp[1] = mapp[y][x - 2];
					mp[2] = mapp[y][x - 1];
					mp[3] = mapp[y + 1][x - 1];
					mp[4] = mapp[y + 2][x - 1];
					sort(mp.rbegin(), mp.rend());
					for (int i = 0; i < 5; i++) {
						if (mp[i] == mapp[y][x - 3] && mp[i] > 0) {
							cmd = "w3l";
							p.first = y;
							p.second = x - 3;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y][x - 2] && mp[i] > 0) {
							cmd = "pr2w2l";
							p.first = y;
							p.second = x - 2;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y][x - 1] && mp[i] > 0) {
							cmd = "wl";
							p.first = y;
							p.second = x - 1;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y + 1][x - 1] && mp[i] > 0) {
							cmd = "dld";
							p.first = y + 1;
							p.second = x - 1;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y + 2][x - 1] && mp[i] > 0) {
							cmd = "keild";
							p.first = y + 2;
							p.second = x - 1;
							reigai = true;
							break;
						}
					}
				}
				if (z[3]) {
					mp[0] = mapp[y][x + 1];
					mp[1] = mapp[y][x + 2];
					mp[2] = mapp[y][x + 3];
					mp[3] = mapp[y + 1][x + 1];
					mp[4] = mapp[y + 2][x + 1];
					sort(mp.rbegin(), mp.rend());
					for (int i = 0; i < 5; i++) {
						if (mp[i] == mapp[y][x + 1] && mp[i] > 0) {
							cmd = "wr";
							p.first = y;
							p.second = x + 1;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y][x + 2] && mp[i] > 0) {
							cmd = "pl2w2r";
							p.first = y;
							p.second = x + 2;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y][x + 3] && mp[i] > 0) {
							cmd = "w3r";
							p.first = y;
							p.second = x + 3;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y + 1][x + 1] && mp[i] > 0) {
							cmd = "drd";
							p.first = y + 1;
							p.second = x + 1;
							reigai = true;
							break;
						}
						if (mp[i] == mapp[y + 2][x + 1] && mp[i] > 0) {
							cmd = "keird";
							p.first = y + 2;
							p.second = x + 1;
							reigai = true;
							break;
						}
					}
				}
			}
		}
		//map表示
		MapDisplay(p.first, p.second);
		cout << endl;
		ShowMapping();
		cout << endl;
		//ShowMapping2();


		cout << turn << "ターン目\n";
		cout << "最大スコア座標 : " << p.first << " " << p.second << endl;
		cout << "コマンド ; " << cmd << endl;

		returnCode = client.FullCommandCheck(cmd);
		returnNum = ParseInt(returnCode);

		//座標変換
		Coordinate(cmd);
		//mapの情報を更新
		MapUpdate(cmd);

		seen[y][x]++;

		//Warp,BlackStar,BedRockの場合さらに座標変換
		if (warp) {
			Warp(warpnum);
			BlackStar(warpnum);
			BedRock(warpnum);
			seen[y][x]++;
		}

		if (miny > y)miny = y;
		if (maxy < y)maxy = y;
		if (minx > x)minx = x;
		if (maxx < x)maxx = x;

		score = 0;
		warp = false;


		for (int i = 0; i < 4; i++) {
			if (seen[y + dy[i]][x + dx[i]] >= 1)mapp[y + dy[i]][x + dx[i]] = mapp[y + dy[i]][x + dx[i]] - (seen[y + dy[i]][x + dx[i]] * 10);
			if (map[y + dy[i]][x + dx[i]] == 70)mapp[y + dy[i]][x + dx[i]] = 500;
			else if (map[y + dy[i]][x + dx[i]] == 71)mapp[y + dy[i]][x + dx[i]] = 501;
			else if (map[y + dy[i]][x + dx[i]] == 72)mapp[y + dy[i]][x + dx[i]] = 502;
			else if (map[y + dy[i]][x + dx[i]] == 73)mapp[y + dy[i]][x + dx[i]] = 503;
			else if (map[y + dy[i]][x + dx[i]] == 74)mapp[y + dy[i]][x + dx[i]] = 504;
			else if (map[y + dy[i]][x + dx[i]] == 75)mapp[y + dy[i]][x + dx[i]] = 505;
			else if (map[y + dy[i]][x + dx[i]] == 76)mapp[y + dy[i]][x + dx[i]] = 506;
			else if (map[y + dy[i]][x + dx[i]] == 77)mapp[y + dy[i]][x + dx[i]] = 507;
			else if (map[y + dy[i]][x + dx[i]] == 78)mapp[y + dy[i]][x + dx[i]] = 508;
			else if (map[y + dy[i]][x + dx[i]] == 79)mapp[y + dy[i]][x + dx[i]] = 509;
			else if (map[y + dy[i]][x + dx[i]] == 0)mapp[y + dy[i]][x + dx[i]] = -1000;
			else if (map[y + dy[i]][x + dx[i]] == 1)mapp[y + dy[i]][x + dx[i]] = 10;
			else if (map[y + dy[i]][x + dx[i]] == 2)mapp[y + dy[i]][x + dx[i]] = 10;
			else if (map[y + dy[i]][x + dx[i]] == 3)mapp[y + dy[i]][x + dx[i]] = 10;
			else if (map[y + dy[i]][x + dx[i]] == 4)mapp[y + dy[i]][x + dx[i]] = -1000;
			else if (map[y + dy[i]][x + dx[i]] == 5)mapp[y + dy[i]][x + dx[i]] = 200;
			else if (map[y + dy[i]][x + dx[i]] == 6)mapp[y + dy[i]][x + dx[i]] = 175;
			else if (map[y + dy[i]][x + dx[i]] == 7)mapp[y + dy[i]][x + dx[i]] = 150;
			else if (map[y + dy[i]][x + dx[i]] == 8)mapp[y + dy[i]][x + dx[i]] = 100;
			else if (20 <= map[y + dy[i]][x + dx[i]] && map[y + dy[i]][x + dx[i]] <= 23)mapp[y + dy[i]][x + dx[i]] = -10;
			else if (30 <= map[y + dy[i]][x + dx[i]] && map[y + dy[i]][x + dx[i]] <= 33)mapp[y + dy[i]][x + dx[i]] = -10;
			else if (map[y + dy[i]][x + dx[i]] == 9)mapp[y + dy[i]][x + dx[i]] = 100;
			else if (60 <= map[y + dy[i]][x + dx[i]] && map[y + dy[i]][x + dx[i]] <= 63)mapp[y + dy[i]][x + dx[i]] = -10;
			else if (40 <= map[y + dy[i]][x + dx[i]] && map[y + dy[i]][x + dx[i]] <= 53)mapp[y + dy[i]][x + dx[i]] = -10;
			else if (map[y + dy[i]][x + dx[i]] == 12)mapp[y + dy[i]][x + dx[i]] = -300;
			else if (map[y + dy[i]][x + dx[i]] == -1)mapp[y + dy[i]][x + dx[i]] = -300;
		}

		for (int i = 0; i < 4; i++) {
			int iy = y + dy[i];
			int jx = x + dx[i];
			if (cmd == "wu" || cmd == "du" || cmd == "pl3ll") {
				if (iy == y - 1 && jx == x) {}
				else mapp[iy][jx] -= 10;
			}
			else if (cmd == "wl" || cmd == "dl" || cmd == "pd3ld") {
				if (iy == y && jx == x - 1) {}
				else mapp[iy][jx] -= 10;
			}
			else if (cmd == "wr" || cmd == "dr" || cmd == "pu3lu") {
				if (iy == y && jx == x + 1) {}
				else mapp[iy][jx] -= 10;
			}
			else if (cmd == "wd" || cmd == "dd" || cmd == "pr3lr") {
				if (iy == y + 1 && jx == x) {}
				else mapp[iy][jx] -= 10;
			}
		}

		for (int i = 0; i < 4; i++) {
			if (score < mapp[y + dy[i]][x + dx[i]]) {
				score = mapp[y + dy[i]][x + dx[i]];
				p.first = dy[i];
				p.second = dx[i];
				if (20 <= map[y + dy[i]][x + dx[i]] && map[y + dy[i]][x + dx[i]] <= 23 || 30 <= map[y + dy[i]][x + dx[i]] && map[y + dy[i]][x + dx[i]] <= 33) {
					warp = true;
					warpnum = map[y + dy[i]][x + dx[i]];
				}
				else if (40 <= map[y + dy[i]][x + dx[i]] && map[y + dy[i]][x + dx[i]] <= 53) {
					warp = true;
					warpnum = map[y + dy[i]][x + dx[i]];
				}
				else if (60 <= map[y + dy[i]][x + dx[i]] && map[y + dy[i]][x + dx[i]] <= 63) {
					warp = true;
					warpnum = map[y + dy[i]][x + dx[i]];
				}
				//ワープマスじゃない場合はfalseに戻す
				else {
					warp = false;
				}
			}
		}

		if (score == 0)cmd2 = "gr";
		else if (p.first == -1 && p.second == 0)cmd2 = "gru";
		else if (p.first == 0 && p.second == -1)cmd2 = "grl";
		else if (p.first == 0 && p.second == 1)cmd2 = "grr";
		else if (p.first == 1 && p.second == 0)cmd2 = "grd";

		if (reigai)cmd2 = "gr";

		MapDisplay(y + p.first, x + p.second);

		cout << "最大スコア座標 : " << y + p.first << " " << x + p.second << endl;
		cout << "getready\n";
		cout << cmd2 << endl;

		//getreadyコマンド
		returnCode = client.GetReadyCheck(cmd2);
		returnNum = ParseInt(returnCode);

		//座標変換
		Coordinate(cmd2);
		//mapの情報を更新
		MapUpdate(cmd2);

		seen[y][x]++;

		//Warp,BlackStar,BedRockの場合さらに座標変換
		if (warp) {
			Warp(warpnum);
			BlackStar(warpnum);
			BedRock(warpnum);
			seen[y][x]++;
		}

		if (miny > y)miny = y;
		if (maxy < y)maxy = y;
		if (minx > x)minx = x;
		if (maxx < x)maxx = x;


	}
}