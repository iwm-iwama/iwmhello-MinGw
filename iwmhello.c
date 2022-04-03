//------------------------------------------------------------------------------
#define  IWM_VERSION         "iwmhello_20220320"
#define  IWM_COPYRIGHT       "Copyright (C)2022 iwm-iwama"
//------------------------------------------------------------------------------
#include "lib_iwmutil.h"

INT  main();
VOID print_version();
VOID print_help();

// ���Z�b�g
#define  PRGB00()            P0("\033[0m")
// ���x��
#define  PRGB01()            P0("\033[38;2;255;255;0m")    // ��
#define  PRGB02()            P0("\033[38;2;255;255;255m")  // ��
// ���͗�^��
#define  PRGB11()            P0("\033[38;2;255;255;100m")  // ��
#define  PRGB12()            P0("\033[38;2;255;220;150m")  // ��
#define  PRGB13()            P0("\033[38;2;100;100;255m")  // ��
// �I�v�V����
#define  PRGB21()            P0("\033[38;2;80;255;255m")   // ��
#define  PRGB22()            P0("\033[38;2;255;100;255m")  // �g��
// �{��
#define  PRGB91()            P0("\033[38;2;255;255;255m")  // ��
#define  PRGB92()            P0("\033[38;2;200;200;200m")  // ��

#define  DATE_FORMAT         "%g%y-%m-%d" // (��)%g�t���Ȃ��ƑS�Đ����\��

INT
main()
{
	// lib_iwmutil ������
	iExecSec_init();       //=> $ExecSecBgn
	iCLI_getCommandLine(); //=> $CMD, $ARGC, $ARGV, $ARGS
	iConsole_EscOn();

	// -h | -help
	if(! $ARGC || iCLI_getOptMatch(0, "-h", "-help"))
	{
		print_help();
		imain_end();
	}

	// -v | -version
	if(iCLI_getOptMatch(0, "-v", "-version"))
	{
		print_version();
		imain_end();
	}

	MBS *p1 = 0;

	for(INT _i1 = 0; _i1 < $ARGC; _i1++)
	{
		// -sleep
		if((p1 = iCLI_getOptValue(_i1, "-sleep=", NULL)))
		{
			Sleep(inum_atoi(p1));
		}
		// print
		else
		{
			P2($ARGS[_i1]);
		}
	}

	// ��������
	/// P("-- %.3fsec\n\n", iExecSec_next());

	// Debug
	/// icalloc_mapPrint(); ifree_all(); icalloc_mapPrint();

	// �ŏI����
	imain_end();
}

VOID
print_version()
{
	PRGB92();
	LN();
	P (" %s\n", IWM_COPYRIGHT);
	P ("   Ver.%s+%s\n", IWM_VERSION, LIB_IWMUTIL_VERSION);
	LN();
	PRGB00();
}

VOID
print_help()
{
	print_version();
	PRGB01();
	P2("\033[48;2;50;50;200m �T���v�� \033[49m");
	NL();
	PRGB02();
	P ("\033[48;2;200;50;50m %s [STR] [Option] \033[49m\n\n", $CMD);
	PRGB11();
	P2(" (�g�p��)");
	PRGB91();
	P ("   %s \033[38;2;255;150;150m\"Hello\" \033[38;2;150;150;255m-sleep=2000 \033[38;2;255;150;150m\"World!\" \033[38;2;150;150;255m-sleep=500\n\n", $CMD);
	PRGB02();
	P2("\033[48;2;200;50;50m [Option] \033[49m");
	PRGB21();
	P2("   -sleep=NUM");
	PRGB91();
	P2("       NUM�}�C�N���b��~");
	NL();
	PRGB92();
	LN();
	PRGB00();
}
