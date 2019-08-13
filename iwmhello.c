//----------------------------------------------
#define IWM_VERSION   "iwmhello_20190711"
#define IWM_COPYRIGHT "(C)2008-2019 iwm-iwama"
//----------------------------------------------
/*
	include
*/
#include "lib_iwmutil.h"

/*
	�֐�
*/
VOID version();
VOID help();

/*
	���L�ϐ�
*/
MBS  *_program = 0;
UINT _exec_cjd = 0;

INT
main()
{
	// ���s����
	_exec_cjd = iExecSec_init();

	// �R�}���h���^�����z��
	_program = iCmdline_getCmd();
	MBS **args = iCmdline_getArgs();

	MBS **ap1 = {0};

	// -help, -h
	ap1 = iargs_option(args, "-help", "-h");
		if($IWM_bSuccess || !**args)
		{
			help();
			imain_end();
		}
	ifree(ap1);

	// -version, -v
	ap1 = iargs_option(args, "-version", "-v");
		if($IWM_bSuccess)
		{
			version();
			LN();
			imain_end();
		}
	ifree(ap1);

	// �{����
	P8();
		P2(_program);
		NL();
	P8();
		iary_print(args);
		NL();
	P8();
		ap1 = ija_token(*args, ",  ");
			iary_print(ap1);
		ifree(ap1);
		NL();

	// ��������
	LN();
	P("-- %.3fsec\n\n", iExecSec_next(_exec_cjd));

	// Debug
	icalloc_mapPrint(); ifree_all(); icalloc_mapPrint();

	// �ŏI����
	imain_end();
}

VOID
version()
{
	LN();
	P ("  %s\n", IWM_COPYRIGHT);
	P ("    Ver.%s+%s\n", IWM_VERSION, LIB_IWMUTIL_VERSION);
}

VOID
help()
{
	version();
	LN();
	P2("���g�p�@��");
	P ("  %s [������] [�I�v�V����]\n", _program);
	NL();
	P2("���I�v�V������");
	P2("  -help, -h");
	P2("      �w���v");
	P2("  -version, -v");
	P2("      �o�[�W�������");
	NL();
	P2("���g�p�၄");
	P ("  %s \"Hello World!\"\n", _program);
	LN();
}
