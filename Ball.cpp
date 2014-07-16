#include "DxLib.h"
#include <math.h>

//画面の横幅縦幅を指定
#define ScreenWidth 800
#define ScreenHeight 600

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
	
	int White;
	int BallGraph, Point_X, Point_Y, Size_X, Size_Y;
	float Move_X, Move_Y, Acc_X, Acc_Y, g, e;
/*	
	BallGraph			:ボールの画像
	Point_X, Point_Y	:座標
	Size_X, Size_Y		:画像のサイズ
	Move_X, Move_Y		:速度
	Acc_X, Acc_Y		:加速度
	g					:重力加速度
	e					:反発係数
*/
	
	// 色の値を取得
	White = GetColor(255, 255, 255);
	
	//画像読み込み
	BallGraph = LoadGraph("Ball.png");
	//画像のサイズを得る
	GetGraphSize(BallGraph, &Size_X, &Size_Y);
	//それぞれのパラメータの初期値を設定
	Point_X = 0;
	Point_Y = 0;
	Move_X = 5.0;
	Move_Y = 1.0;
	Acc_X = 1.0;
	Acc_Y = 1.0;
	g = 1.098;
	e = -0.9;
	
	// ゲームループ開始　エスケープキーが押されたら終了する
	while(ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 )
	{
		// 画面に四角を描き,前に描いてあった画面を消す
		DrawBox(0, 0, ScreenWidth, ScreenHeight, White, TRUE);
		
		//次の移動距離＝現在の移動距離×加速度
		Move_X = Move_X * Acc_X;
		if(Move_Y > 0)	Move_Y = Move_Y * (Acc_Y * g);
		else	Move_Y = Move_Y * Acc_Y + Move_Y * g;

		//次の座標＝現在の座標＋移動距離
		Point_X = Point_X + (int)Move_X;
		Point_Y = Point_Y + (int)Move_Y;

		//画面外に出てしまった時の処理
		if(Point_X < 0 || ScreenWidth - Size_X < Point_X){
			if(Point_X < 0)	Point_X = -Point_X;
			else	Point_X = Point_X - ScreenWidth + Size_X;

			//飛び出てしまった移動量に反発係数を掛ける。
//			Move_X = Move_X * e;
			Acc_X = Acc_X * e;
//			Point_X = Point_X + (int)Move_X;
		}

		if(Point_Y < 0 || ScreenHeight - Size_Y < Point_Y){
			if(Point_X < 0)	Point_Y = -Point_Y;
			else	Point_Y = Point_Y - ScreenHeight;

//			Move_Y = Move_Y * e;
			Acc_Y = Acc_Y * e;
//			Point_Y = Point_Y + (int)Move_Y;
		}

		//ボールの画像を描画
		DrawGraph(Point_X, Point_Y, BallGraph, TRUE);

		// 一定時間待つ
		WaitTimer(20);

		//ボールの移動量が0になったら終了
//		if((int)Move_X == 0 && (int)Move_Y == 0)	break;

		// メッセージ処理
		if(ProcessMessage() == -1 )	break;	// エラーが起きたらループを抜ける
	}

	// ＤＸライブラリの使用終了
	DxLib_End();

	// ソフトの終了
	return 0;
}

