// main.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

int main(int argc, char **argv)
{
    GrabCutClass gcc;   //�C���X�^���X����
    string fname;  //���͉摜�t�@�C����
#if 1
    if(argc == 2)
    {
        fname = argv[1];
    }
    else
    {
        cout << "�摜�t�@�C����.exe�Ƀh���b�O���h���b�v���ׂ��B" << endl;
        return 0;
    }
#endif    
    gcc.Launch(fname);

	return 0;
}
