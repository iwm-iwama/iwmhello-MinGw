//------------------------------------------------------------------------------
#define  IWM_VERSION         "iwmhello_20220902"
#define  IWM_COPYRIGHT       "Copyright (C)2022 iwm-iwama"
//------------------------------------------------------------------------------
#include "lib_iwmutil2.h"

INT  main();
VOID print_version();
VOID print_help();

// リセット
#define  PRGB00()            P0("\x1b[0m")
// ラベル
#define  PRGB01()            P0("\x1b[38;2;255;255;0m")    // 黄
#define  PRGB02()            P0("\x1b[38;2;255;255;255m")  // 白
// 入力例／注
#define  PRGB11()            P0("\x1b[38;2;255;255;100m")  // 黄
#define  PRGB12()            P0("\x1b[38;2;255;220;150m")  // 橙
#define  PRGB13()            P0("\x1b[38;2;100;100;255m")  // 青
// オプション
#define  PRGB21()            P0("\x1b[38;2;80;255;255m")   // 水
#define  PRGB22()            P0("\x1b[38;2;255;100;255m")  // 紅紫
// 本文
#define  PRGB91()            P0("\x1b[38;2;255;255;255m")  // 白
#define  PRGB92()            P0("\x1b[38;2;200;200;200m")  // 銀

INT
main()
{
	// lib_iwmutil 初期化
	iExecSec_init();            //=> $ExecSecBgn
	iCLI_getCommandLine(65001); //=> $CMD, $ARGC, $ARGV, $ARGS
	iConsole_EscOn();

	// -h | -help
	if(! $ARGC || iCLI_getOptMatch(0, L"-h", L"-help"))
	{
		print_help();
		imain_end();
	}

	// -v | -version
	if(iCLI_getOptMatch(0, L"-v", L"-version"))
	{
		print_version();
		imain_end();
	}

	WCS *wp1 = NULL;
	U8N *up1 = NULL;

	for(INT _i1 = 0; _i1 < $ARGC; _i1++)
	{
		// -sleep
		if((wp1 = iCLI_getOptValue(_i1, L"-sleep=", NULL)))
		{
			Sleep(inum_wtoi(wp1));
		}
		// print
		else
		{
			up1 = W2U($ARGS[_i1]);
				P2(up1);
			ifree(up1);
		}
	}

	// 処理時間
	/// P("-- %.3fsec\n\n", iExecSec_next());

	// Debug
	/// calloc_mapPrint(); ifree_all(); icalloc_mapPrint();

	// 最終処理
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
	U8N *_cmd = W2U($CMD);

	print_version();
	PRGB01();
	P2("\x1b[48;2;50;50;200m サンプル \x1b[49m");
	NL();
	PRGB02();
	P ("\x1b[48;2;200;50;50m %s [STR] [Option] \x1b[49m\n\n", _cmd);
	PRGB11();
	P2(" (使用例)");
	PRGB91();
	P ("   %s \x1b[38;2;255;150;150m\"Hello\" \x1b[38;2;150;150;255m-sleep=2000 \x1b[38;2;255;150;150m\"World!\" \x1b[38;2;150;150;255m-sleep=500\n\n", _cmd);
	PRGB02();
	P2("\x1b[48;2;200;50;50m [Option] \x1b[49m");
	PRGB21();
	P2("   -sleep=NUM");
	PRGB91();
	P2("       NUMマイクロ秒停止");
	NL();
	PRGB92();
	LN();
	PRGB00();

	ifree(_cmd);
}
