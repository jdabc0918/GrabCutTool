#include "stdafx.h"

#ifndef __GRABCUTCLASS_H__
#define __GRABCUTCLASS_H__

//
class GrabCutClass
{
private:

    typedef struct GUI_Params
    {
        Mat view_image;
        bool is_drawing;
        bool rect_is_setted;
        Rect rect;

        GUI_Params()
        {
            is_drawing = false;
            rect_is_setted = false;
            rect = Rect(0, 0, 0, 0);
        }
        ~GUI_Params(){}

        void DrawRect()
        {
            Rect r = this->rect;
            if (r.width < 0)
            {
                r.x += r.width;
                r.width *= -1;
            }
            if (r.height < 0)
            {
                r.y += r.height;
                r.height *= -1;
            }
            rectangle(this->view_image, r, Scalar(0, 255, 0), 1);
        }
    };
    typedef struct InputInfo
    {
        Mat src_image;      //入力画像
        string src_fname;   //入力画像ファイル名
    };
    typedef struct OutputInfo
    {
        Mat png_image;       //出力画像（PNG）
        Mat msk_image;       //マスク画像
        string png_fname;    //出力画像（PNG）ファイル名
        string msk_fname;    //マスク画像ファイル名
    };

private:

    //入力関係
    InputInfo iInfo;
    //出力関係
    OutputInfo oInfo;   
    //GUI関係
    GUI_Params gp;

private:
    //入出力
    int Read(string _src_fname); //画像を読み込む
    void Write(void);   //出力画像を保存する

    //GUI 
    static void onMouse(int event, int x, int y, int flag, void* param);
    int View(void); //GUIを起動する

    //処理系
    int ExeGrabCut(void);   //GrabCutを実行する
    void MakePngImage(Mat _src, Mat *_dst, Mat msk);    //PNG画像を生成する
    
public:
    GrabCutClass(void);     //コンストラクタ
    ~GrabCutClass(void);    //デストラクタ

    //★メイン処理
    void Launch(string _src_fname, string _msk_fname = "default");  //起動する
};

#endif