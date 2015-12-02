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
        Mat src_image;      //���͉摜
        string src_fname;   //���͉摜�t�@�C����
    };
    typedef struct OutputInfo
    {
        Mat png_image;       //�o�͉摜�iPNG�j
        Mat msk_image;       //�}�X�N�摜
        string png_fname;    //�o�͉摜�iPNG�j�t�@�C����
        string msk_fname;    //�}�X�N�摜�t�@�C����
    };

private:

    //���͊֌W
    InputInfo iInfo;
    //�o�͊֌W
    OutputInfo oInfo;   
    //GUI�֌W
    GUI_Params gp;

private:
    //���o��
    int Read(string _src_fname); //�摜��ǂݍ���
    void Write(void);   //�o�͉摜��ۑ�����

    //GUI 
    static void onMouse(int event, int x, int y, int flag, void* param);
    int View(void); //GUI���N������

    //�����n
    int ExeGrabCut(void);   //GrabCut�����s����
    void MakePngImage(Mat _src, Mat *_dst, Mat msk);    //PNG�摜�𐶐�����
    
public:
    GrabCutClass(void);     //�R���X�g���N�^
    ~GrabCutClass(void);    //�f�X�g���N�^

    //�����C������
    void Launch(string _src_fname, string _msk_fname = "default");  //�N������
};

#endif