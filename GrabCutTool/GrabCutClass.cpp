#include "stdafx.h"

GrabCutClass::GrabCutClass(void){}
GrabCutClass::~GrabCutClass(void){}

void GrabCutClass::MakePngImage(Mat _src, Mat *_dst, Mat msk)
{
    //_srcが空だったらエラー
    if (_src.empty())
    {
        cout << "GrabCutClass::MakePngImage ERROR : _src is empty." << endl;
        return;
    }
    else
    {
        //本当はココでカラーかどうかの判定もした方がいい
        
        Mat tmp = _src.clone();
        //RGB各チャンネルに分割
        vector<Mat> vMat;
        split(tmp, vMat);  //vMat[0]=B, vMat[1]=G, vMat[2]=R
        //マスクを後ろにくっつける
        vMat.push_back(msk);
        //1枚の画像に戻す(splitの逆)
        merge(vMat, *_dst);

        return;
    }
}

void GrabCutClass::Write(void)
{
    //出力ファイル情報を更新
    string fname = this->iInfo.src_fname;   //入力ファイル名を取得
    int dot_loc = fname.find_last_of(".");    //.の位置を取得
    this->oInfo.png_fname = fname.substr(0, dot_loc) + "_gc.png";   //PNG画像ファイル名
    this->oInfo.msk_fname = fname.substr(0, dot_loc) + "_msk.png";  //マスク画像ファイル名

    //保存
    imwrite(this->oInfo.png_fname, this->oInfo.png_image);
    imwrite(this->oInfo.msk_fname, this->oInfo.msk_image);
    return;
}

void GrabCutClass::onMouse(int event, int x, int y, int flag, void* param)
{
    //パラメータを取得
    GUI_Params *gp = (GUI_Params *)param;
    
    // マウスイベントを取得
    switch (event) {
    case cv::EVENT_MOUSEMOVE:   //マウス移動中の処理
        if (gp->is_drawing)
        {
            gp->rect.width = x - gp->rect.x;
            gp->rect.height = y - gp->rect.y;
        }
        break;
    case cv::EVENT_LBUTTONDOWN: //マウス左クリック(押)の処理
        gp->is_drawing = true;
        gp->rect_is_setted = false;
        gp->rect = Rect(x, y, 0, 0);
        break;
    case cv::EVENT_LBUTTONUP:   //マウス左クリック(離)の処理
        gp->is_drawing = false;
        gp->rect_is_setted = true;
        break;
    }
}

int GrabCutClass::Read(string _src_fname)
{
    //入力画像の読み込み
    this->iInfo.src_image = imread(_src_fname);
    if (this->iInfo.src_image.empty())  //失敗したらreturn
    {
        cout << "GrabCutClass::MakePngImage ERROR : Cannot open file " << _src_fname << endl;
        return -1;
    }

    //入力画像ファイル名を格納
    this->iInfo.src_fname = _src_fname;

    //ビュアーを起動
    this->View();

    return 0;
}

int GrabCutClass::View(void)
{    
    //いろいろ説明
    cout << "======================================" << endl;
    cout << " [Step1] 抽出したい領域を四角で囲む" << endl;
    cout << " [Step2] Escで終了＆保存" << endl;
    cout << "======================================" << endl;

    //ウィンドウ生成
    string winName = this->iInfo.src_fname;
    namedWindow(winName);
    // マウスイベントに対するコールバック関数を登録
    setMouseCallback(winName, this->onMouse, &this->gp);

    //表示ループ
    while (1) {
        this->iInfo.src_image.copyTo(this->gp.view_image);    //入力画像をコピー
        if (this->gp.is_drawing || this->gp.rect_is_setted)
        {
            this->gp.DrawRect();    //必要に応じて四角を描画
        }
        imshow(winName, this->gp.view_image);   //表示
        // 'Esc'が押されたら終了
        if (waitKey(10) == 27)
        {
            this->ExeGrabCut();
            break;
        }
    }
    return 0;
}

int GrabCutClass::ExeGrabCut(void)
{
    //正しい矩形が選択されているか確認
    Rect r = this->gp.rect;
    if (r.x < 0 || r.y < 0 || r.width <= 0 || r.height <= 0)
    {
        cout << "GrabCutClass::ExeGrabCut ERROR : invalid rect size." << endl;
        return -1;
    }

    //GrabCut実行
    Mat gcRes, gcBgModel, gcFgModel;
    grabCut(this->iInfo.src_image, gcRes, r, gcBgModel, gcFgModel, 1, GC_INIT_WITH_RECT);

    //実行結果のマスクを作成
    compare(gcRes, GC_PR_FGD, this->oInfo.msk_image, CV_CMP_EQ);

    //出力用の画像を生成
    this->MakePngImage(this->iInfo.src_image, &this->oInfo.png_image, this->oInfo.msk_image);

    //書き出し
    this->Write();

    return 0;
}

void GrabCutClass::Launch(string _src_fname, string _msk_fname)
{
    //画像読み込み
    this->Read(_src_fname);

}