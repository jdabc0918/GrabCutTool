// main.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

int main(int argc, char **argv)
{
    GrabCutClass gcc;   //インスタンス生成
    string fname;  //入力画像ファイル名
#if 1
    if(argc == 2)
    {
        fname = argv[1];
    }
    else
    {
        cout << "画像ファイルを.exeにドラッグ＆ドロップすべし。" << endl;
        return 0;
    }
#endif    
    gcc.Launch(fname);

	return 0;
}
