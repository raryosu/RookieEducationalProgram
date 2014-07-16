#include "DxLib.h"
#include <math.h>

//画面の横幅縦幅を指定
#define ScreenWidth 800
#define ScreenHeight 600
#define Gravity 1.098
/*
#defineとは
"マクロ"といい、不変の値を設定する。
例えば、画面の大きさや重力加速度など。
*/

// WinMain 関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//ログ出力をしない(LOGファイルは邪魔)
	SetOutApplicationLogValidFlag(FALSE);
	// タイトルを変更
	SetMainWindowText("Test");
	// ウインドウモードに変更
	ChangeWindowMode(TRUE);
	//ウインドウの大きさ設定
	SetGraphMode(ScreenWidth, ScreenHeight, 32);
	//ウインドウの大きさを自由に変更出来るかどうかのフラグ
	SetWindowSizeChangeEnableFlag(TRUE);
	// 裏画面を使用
	SetDrawScreen(DX_SCREEN_BACK);
	// ＤＸライブラリの初期化
	if(DxLib_Init() == -1 ) return -1;
	// 画面の初期化
	ClearDrawScreen();
	// 透過色を変更(ピンク)
	SetTransColor(255, 0, 255);
	
	int White, Black;
	bool StartFlag, GameOverFlag;
	int BallGraph, Point_X, Point_Y, Size_X, Size_Y;
	float Move_X, Move_Y, Acc_X, Acc_Y, e, u;
/*	
	White				:白色の値
	BallGraph			:ボールの画像
	Point_X, Point_Y	:座標
	Size_X, Size_Y		:画像のサイズ
	Move_X, Move_Y		:速度
	Acc_X, Acc_Y		:加速度
	e					:反発係数
	u					:摩擦係数
*/
	
	// 色の値を取得
	White = GetColor(255, 255, 255);
	Black = GetColor(0, 0, 0);
	
	//フラグの初期化
	StartFlag = FALSE;
	GameOverFlag = FALSE;

	//画像読み込み
	BallGraph = LoadGraph("Ball.png");
	//画像のサイズを得る
	GetGraphSize(BallGraph, &Size_X, &Size_Y);

	//それぞれのパラメータの初期値を設定
	Point_X = 0;
	Point_Y = 0;
	Move_X = 0.0;
	Move_Y = 0.0;
	Acc_X = 0.0;
	Acc_Y = 0.0;
	e = 0.75;
	u = 0.9;
/*
	初速度、加速度を変えることで、移動の様子が変わってくる。
	反発係数や摩擦係数を1.0以上にすると･･･?
	色々試してみても、いいかもね。
*/
	
	// ゲームループ開始　エスケープキーが押されたら終了する
	while(ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面に四角を描き,前に描いてあった画面を消す
		DrawBox(0, 0, ScreenWidth, ScreenHeight, White, TRUE);
/*
	ゲームは無限ループの紙芝居をしているもの。
	一回ループするごとにキャラクターなどが動いていく。
	ゲームオーバーのフラグが立ったりすると、ループを抜け、プログラムが終了する。
*/		
		//スペースキーを押すとスタート
		if(CheckHitKey(KEY_INPUT_SPACE) == 1)	StartFlag = TRUE;
		else if(StartFlag == FALSE && GameOverFlag == FALSE)
			DrawString(Point_X + Size_X + 10, 10, "Please press the SPACE key.", Black);

		//スタートフラグが立つと処理開始
		if(StartFlag){
			//次の速度＝現在の速度×加速度
			Move_X += Acc_X;
			Move_Y += Acc_Y + Gravity;

			//ボールが転がっている時、摩擦抵抗を受ける
			if(fabs(Move_Y) < 0.4){
				Move_X *= u;
			}

			//次の座標＝現在の座標＋移動距離
			Point_X += (int)Move_X;
			Point_Y += (int)Move_Y;

			//画面外に出てしまった時の処理
			if(Point_X < 0 || ScreenWidth - Size_X < Point_X){
				if(Point_X < 0)	Point_X = 0;
				else	Point_X = ScreenWidth - Size_X;

				//反発係数を掛ける
				Move_X = -Move_X * e;

				//ベクトルを逆にする
				Acc_X = -Acc_X * e;
			}

			if(Point_Y < 0 || ScreenHeight - Size_Y < Point_Y){
				if(Point_X < 0)	Point_Y = 0;
				else	Point_Y = ScreenHeight - Size_Y;

				Move_Y = -Move_Y * e;
			}

			//ボールの速度(絶対値)がある程度小さくなったら、0にする
			if(fabs(Move_X) < 1.0)	Move_X = 0.0;
			if(fabs(Move_Y) < 0.4)	Move_Y = 0.0;
			
			//ボールの速度(絶対値)がある程度小さくなったら、0にする
			if(fabs(Acc_X) < 1.0)	Acc_X = 0.0;
			if(fabs(Acc_Y) < 0.4)	Acc_Y = 0.0;

			//速度が0かつ、地面を転がっていると終了
			if(Move_X == 0.0 && Move_Y == 0.0 && Point_Y == ScreenHeight - Size_Y){
				GameOverFlag = TRUE;
				StartFlag = FALSE;
			}
		}

		//ボールの画像を描画
		DrawGraph(Point_X, Point_Y, BallGraph, TRUE);

		// 一定時間待つ
		//これが無いと、処理が早く終わりすぎてしまう。
		WaitTimer(20);

		//ゲームオーバーフラグが立ったら3秒後に終了する。
		if(GameOverFlag){
			DrawString(10, 10, "Ball has stopped. And exit.", Black);
			WaitTimer(3000);
			break;
		}

		// メッセージ処理
		// エラーが起きたらループを抜ける
		if(ProcessMessage() == -1 )	break;
	}

	// ＤＸライブラリの使用終了
	DxLib_End();

	// ソフトの終了
	return 0;
}

