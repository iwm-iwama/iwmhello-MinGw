:: Ini ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	@echo off
	cd %~dp0
	%~d0
	cls
	::
	:: �t�@�C�����̓\�[�X�Ɠ���
	::
	set fn=%~n0
	set exec=%fn%.exe
	set op_link=-Os -lgdi32 -luser32 -lshlwapi
	set src=%fn%.c
	set lib=lib_iwmutil.a

:: Make ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	echo --- Compile -S ------------------------------------
	for %%s in (%src%) do (
		gcc.exe %%s -S %op_link%
		echo %%~ns.s
	)
	echo.

	echo --- Make ------------------------------------------
	for %%s in (%src%) do (
		gcc.exe %%s -c -Wall %op_link%
	)
	gcc.exe *.o %lib% -o %exec% %op_link%
	echo %exec%

	:: �㏈��
	strip -s %exec%
	rm *.o

	:: ���s
	if not exist "%exec%" goto end

	:: ����
	echo.
	pause

:: Test ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cls
	set tm1=%time%
	echo [%tm1%]

	set s="Hello, World!"

	%exec%
	%exec% %s% -sleep=1000

	echo [%tm1%]
	echo [%time%]

:: Quit ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:end
	echo.
	pause
	exit
