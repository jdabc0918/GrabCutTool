#include "stdafx.h"

GrabCutClass::GrabCutClass(void){}
GrabCutClass::~GrabCutClass(void){}

void GrabCutClass::MakePngImage(Mat _src, Mat *_dst, Mat msk)
{
    //_src���󂾂�����G���[
    if (_src.empty())
    {
        cout << "GrabCutClass::MakePngImage ERROR : _src is empty." << endl;
        return;
    }
    else
    {
        //�{���̓R�R�ŃJ���[���ǂ����̔����������������
        
        Mat tmp = _src.clone();
        //RGB�e�`�����l���ɕ���
        vector<Mat> vMat;
        split(tmp, vMat);  //vMat[0]=B, vMat[1]=G, vMat[2]=R
        //�}�X�N�����ɂ�������
        vMat.push_back(msk);
        //1���̉摜�ɖ߂�(split�̋t)
        merge(vMat, *_dst);

        return;
    }
}

void GrabCutClass::Write(void)
{
    //�o�̓t�@�C�������X�V
    string fname = this->iInfo.src_fname;   //���̓t�@�C�������擾
    int dot_loc = fname.find_last_of(".");    //.�̈ʒu���擾
    this->oInfo.png_fname = fname.substr(0, dot_loc) + "_gc.png";   //PNG�摜�t�@�C����
    this->oInfo.msk_fname = fname.substr(0, dot_loc) + "_msk.png";  //�}�X�N�摜�t�@�C����

    //�ۑ�
    imwrite(this->oInfo.png_fname, this->oInfo.png_image);
    imwrite(this->oInfo.msk_fname, this->oInfo.msk_image);
    return;
}

void GrabCutClass::onMouse(int event, int x, int y, int flag, void* param)
{
    //�p�����[�^���擾
    GUI_Params *gp = (GUI_Params *)param;
    
    // �}�E�X�C�x���g���擾
    switch (event) {
    case cv::EVENT_MOUSEMOVE:   //�}�E�X�ړ����̏���
        if (gp->is_drawing)
        {
            gp->rect.width = x - gp->rect.x;
            gp->rect.height = y - gp->rect.y;
        }
        break;
    case cv::EVENT_LBUTTONDOWN: //�}�E�X���N���b�N(��)�̏���
        gp->is_drawing = true;
        gp->rect_is_setted = false;
        gp->rect = Rect(x, y, 0, 0);
        break;
    case cv::EVENT_LBUTTONUP:   //�}�E�X���N���b�N(��)�̏���
        gp->is_drawing = false;
        gp->rect_is_setted = true;
        break;
    }
}

int GrabCutClass::Read(string _src_fname)
{
    //���͉摜�̓ǂݍ���
    this->iInfo.src_image = imread(_src_fname);
    if (this->iInfo.src_image.empty())  //���s������return
    {
        cout << "GrabCutClass::MakePngImage ERROR : Cannot open file " << _src_fname << endl;
        return -1;
    }

    //���͉摜�t�@�C�������i�[
    this->iInfo.src_fname = _src_fname;

    //�r���A�[���N��
    this->View();

    return 0;
}

int GrabCutClass::View(void)
{    
    //���낢�����
    cout << "======================================" << endl;
    cout << " [Step1] ���o�������̈���l�p�ň͂�" << endl;
    cout << " [Step2] Esc�ŏI�����ۑ�" << endl;
    cout << "======================================" << endl;

    //�E�B���h�E����
    string winName = this->iInfo.src_fname;
    namedWindow(winName);
    // �}�E�X�C�x���g�ɑ΂���R�[���o�b�N�֐���o�^
    setMouseCallback(winName, this->onMouse, &this->gp);

    //�\�����[�v
    while (1) {
        this->iInfo.src_image.copyTo(this->gp.view_image);    //���͉摜���R�s�[
        if (this->gp.is_drawing || this->gp.rect_is_setted)
        {
            this->gp.DrawRect();    //�K�v�ɉ����Ďl�p��`��
        }
        imshow(winName, this->gp.view_image);   //�\��
        // 'Esc'�������ꂽ��I��
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
    //��������`���I������Ă��邩�m�F
    Rect r = this->gp.rect;
    if (r.x < 0 || r.y < 0 || r.width <= 0 || r.height <= 0)
    {
        cout << "GrabCutClass::ExeGrabCut ERROR : invalid rect size." << endl;
        return -1;
    }

    //GrabCut���s
    Mat gcRes, gcBgModel, gcFgModel;
    grabCut(this->iInfo.src_image, gcRes, r, gcBgModel, gcFgModel, 1, GC_INIT_WITH_RECT);

    //���s���ʂ̃}�X�N���쐬
    compare(gcRes, GC_PR_FGD, this->oInfo.msk_image, CV_CMP_EQ);

    //�o�͗p�̉摜�𐶐�
    this->MakePngImage(this->iInfo.src_image, &this->oInfo.png_image, this->oInfo.msk_image);

    //�����o��
    this->Write();

    return 0;
}

void GrabCutClass::Launch(string _src_fname, string _msk_fname)
{
    //�摜�ǂݍ���
    this->Read(_src_fname);

}