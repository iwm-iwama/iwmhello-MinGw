//------------------------------------------------------------------------------
#define   IWM_VERSION         "iwmhello_20210316"
#define   IWM_COPYRIGHT       "Copyright (C)2021 iwm-iwama"
//------------------------------------------------------------------------------
#include "lib_iwmutil.h"

INT  main();
VOID print_version();
VOID print_help();

// [�����F] + ([�w�i�F] * 16)
//  0 = Black    1 = Navy     2 = Green    3 = Teal
//  4 = Maroon   5 = Purple   6 = Olive    7 = Silver
//  8 = Gray     9 = Blue    10 = Lime    11 = Aqua
// 12 = Red     13 = Fuchsia 14 = Yellow  15 = White
#define   ColorTitle          (15 + ( 9 * 16))
#define   ColorHeaderFooter   ( 7 + ( 0 * 16))
#define   ColorBgText1        (15 + (12 * 16))
#define   ColorExp1           (13 + ( 0 * 16))
#define   ColorExp2           (14 + ( 0 * 16))
#define   ColorExp3           (11 + ( 0 * 16))
#define   ColorText1          (15 + ( 0 * 16))

#define   DATE_FORMAT         "%g%y-%m-%d" // (��)%g�t���Ȃ��ƑS�Đ����\��

//
// ��~����
// -sleep=NUM
//
UINT _Sleep = 0;
//
// ���s�֌W
//
MBS  *$program     = "";
MBS  **$args       = {0};
UINT $argsSize     = 0;
UINT $colorDefault = 0;
UINT $execMS       = 0;

INT
main()
{
	// �R�}���h���^����
	$program      = iCmdline_getCmd();
	$args         = iCmdline_getArgs();
	$argsSize     = iary_size($args);
	$colorDefault = iConsole_getBgcolor(); // ���݂̕����F�^�w�i�F
	$execMS       = iExecSec_init(); // ���s�J�n����

	// -h | -help
	if($argsSize == 0 || imb_cmpp($args[0], "-h") || imb_cmpp($args[0], "-help"))
	{
		print_help();
		imain_end();
	}

	// -v | -version
	if(imb_cmpp($args[0], "-v") || imb_cmpp($args[0], "-version"))
	{
		print_version();
		LN();
		imain_end();
	}

	// [0] Msg
	P("%s", $args[0]);

	// [1..]
	for(INT _i1 = 1; _i1 < $argsSize; _i1++)
	{
		MBS **_as1 = ija_split($args[_i1], "=", "\"\"\'\'", FALSE);
		MBS **_as2 = ija_split(_as1[1], ",", "\"\"\'\'", TRUE);

		// -sleep
		if(imb_cmpp(_as1[0], "-sleep"))
		{
			_Sleep = inum_atoi(_as2[0]);
		}

		ifree(_as2);
		ifree(_as1);
	}

	Sleep(_Sleep);
	NL();

	// ��������
	P("-- %.3fsec\n\n", iExecSec_next($execMS));

	// Debug
	icalloc_mapPrint(); ifree_all(); icalloc_mapPrint();

	// �ŏI����
	imain_end();
}

VOID
print_version()
{
	LN();
	P (" %s", IWM_COPYRIGHT);
	NL();
	P ("   Ver.%s+%s", IWM_VERSION, LIB_IWMUTIL_VERSION);
	NL();
}

VOID
print_help()
{
	iConsole_setTextColor(ColorHeaderFooter);
		print_version();
		LN();
	iConsole_setTextColor(ColorTitle);
		P (" �T���v�� ");
	iConsole_setTextColor($colorDefault);
		NL();
		NL();
	iConsole_setTextColor(ColorBgText1);
		P (" %s [������] [�I�v�V����] ", $program);
	iConsole_setTextColor($colorDefault);
		NL();
		NL();
	iConsole_setTextColor(ColorExp1);
		P2(" (�g�p��)");
	iConsole_setTextColor(ColorText1);
		P ("   %s \"Hello World!\" -sleep=5000", $program);
	iConsole_setTextColor(ColorExp2);
		NL();
		NL();
		P2(" [�I�v�V����]");
	iConsole_setTextColor(ColorExp3);
		P2("   -sleep=NUM");
	iConsole_setTextColor(ColorText1);
		P2("       NUM�}�C�N���b��~");
	iConsole_setTextColor(ColorHeaderFooter);
		NL();
		LN();
	iConsole_setTextColor($colorDefault); // ���̕����F�^�w�i�F
}
