//----------------------------------------------
#define IWM_VERSION   "iwmhello_20191121"
#define IWM_COPYRIGHT "(C)2008-2019 iwm-iwama"
//----------------------------------------------
#include "lib_iwmutil.h"

//-------
// �֐�
//-------
VOID version();
VOID help();

//-----------
// ���L�ϐ�
//-----------
UINT $execMS   = 0;
MBS  *$program = NULL;
MBS  **$args   = {NULL};
UINT $argsSize = 0;
MBS  *$stdout  = NULL;
INT  $iRepeat  = 0;

INT
main()
{
	// ���s�J�n����
	$execMS = iExecSec_init();

	// �R�}���h���^����
	$program = iCmdline_getCmd();
	$args = iCmdline_getArgs();
	$argsSize = $IWM_uAryUsed;

	// "-help"
	if($argsSize == 0)
	{
		help();
		imain_end();
	}

	MBS **as1 = {NULL};
	MBS **as2 = {NULL};

	for(INT _i1 = 0; _i1 < $argsSize; _i1++)
	{
		if(_i1 == 0)
		{
			MBS *s1 = $args[0];
		
			// "-help", "-h"
			if(imb_cmpp(s1, "-help") || imb_cmpp(s1, "-h"))
			{
				help();
				imain_end();
			}

			// "-version", "-v"
			if(imb_cmpp(s1, "-version") || imb_cmpp(s1, "-v"))
			{
				version();
				LN();
				imain_end();
			}

			// Stdout
			$stdout = s1;
		}
		else
		{
			// (��) -sleep=5000,sec
			// (��) "=" => {"-sleep", "5000,sec"}
			as1 = ija_split($args[_i1], "=", "", FALSE);
			INT _i11 = $IWM_uAryUsed;

			MBS *sLabel = as1[0];

			for(INT _i2 = 1; _i2 < _i11; _i2++)
			{
				// (��) "," => {"5000", "sec"}
				as2 = ija_split(as1[_i2], ",", "\"\"\'\'", TRUE);

				// "-sleep"
				if(imb_cmpp(sLabel, "-sleep"))
				{
					$iRepeat = inum_atoi(as2[0]);
				}

				ifree(as2);
			}
			ifree(as1);
		}
	}

	P("%s", $stdout);
	Sleep($iRepeat);
	NL();

	// ��������
	P("-- %.3fsec\n\n", iExecSec_next($execMS));

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
	LN();
}

VOID
help()
{
	version();
	P2("���g�p�@��");
	P ("  %s [������] [�I�v�V����]\n", $program);
	NL();
	P2("���I�v�V������");
	P2("  -help, -h");
	P2("      �w���v");
	P2("  -version, -v");
	P2("      �o�[�W�������");
	NL();
	P2("  -sleep=[NUM]");
	P2("      [NUM]�}�C�N���b��~");
	NL();
	P2("���g�p�၄");
	P ("  %s \"Hello World!\" -sleep=5000\n", $program);
	LN();
}
