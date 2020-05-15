#include "lib_iwmutil.h"
#include "lib_iwmutil_stack.h"
/////////////////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------------------
	Stack����
---------------------------------------------------------------------------------------*/
/////////////////////////////////////////////////////////////////////////////////////////
//-----------------
// �������^free()
//-----------------
/* (��)
	$struct_iStackA *Stack = iStack_allocA();
	iStack_freeA(Stack);
*/
// v2014-02-01
$struct_iStackA
*iStack_allocA()
{
	$struct_iStackA *Stack = ($struct_iStackA*)icalloc(1, sizeof($struct_iStackA), FALSE);
		icalloc_err(Stack);
	(Stack->size) = 0;
	(Stack->ary) = icalloc_MBS_ary(0);
	return Stack;
}
// v2014-02-01
VOID
iStack_freeA(
	$struct_iStackA *Stack // �i�[�X�^�b�N��
)
{
	if(!Stack)
	{
		return;
	}
	iStack_eraseA(Stack);
	ifree(Stack->ary);
	ifree(Stack);
}
//---------------------
// Stack[n]�̓��e����
//---------------------
/* (��)
	$struct_iStackA *Stack = iStack_allocA();
		iStack_pushA(Stack, "iwama");
			P832(Stack->size, Stack->ary[0]);
		iStack_pushA(Stack, "���");
			P832(Stack->size, Stack->ary[1]);
		iStack_eraseA(Stack2);
			P832(Stack->size, Stack->ary[0]);
			P832(Stack->size, Stack->ary[1]);
	iStack_freeA(Stack);
*/
// v2015-12-23
BOOL
iStack_eraseA(
	$struct_iStackA *Stack // �i�[�X�^�b�N��
)
{
	if(!Stack)
	{
		return FALSE;
	}
	UINT u1 = 0;
	while(u1 < (Stack->size))
	{
		ifree(*((Stack->ary) + u1));
		*((Stack->ary) + u1) = 0;
		++u1;
	}
	(Stack->size) = 0;
	return TRUE;
}
//----------------------
// Stack�̃N���[���쐬
//----------------------
/* (��)
	$struct_iStackA *Stack1 = iStack_allocA();
		iStack_pushA(Stack1, "ABC");
	$struct_iStackA *Stack2 = iStack_cloneA(Stack1);
	P83(Stack1->size);
		iStack_printA(Stack1);
	P83(Stack2->size);
		iStack_printA(Stack2);
	iStack_freeA(Stack1);
	iStack_freeA(Stack2);
*/
// v2014-03-05
$struct_iStackA
*iStack_cloneA(
	$struct_iStackA *Stack
)
{
	$struct_iStackA *rtn = iStack_allocA();
	UINT u1 = 0;
	while(u1 < (Stack->size))
	{
		iStack_pushA(rtn, *((Stack->ary) + u1));
		++u1;
	}
	return rtn;
}
//-------
// size
//-------
/* (��)
	P3(iStack_sizeA(Stack)); // {"a", "b", "c"} => 3
*/
// v2013-02-02
UINT
iStack_sizeA(
	$struct_iStackA *Stack // �i�[�X�^�b�N��
)
{
	return (Stack ? Stack->size : 0);
}
//------------------
// index�l�`�F�b�N
//------------------
/* (��)
	// (Stack->size) = 5 �̂Ƃ�
	iStack_IndexChkA(Stack, -1); //=> 4
	iStack_IndexChkA(Stack,  6); //=> 5
*/
// v2019-08-16
VOID
iStack_IndexChkA(
	$struct_iStackA *Stack, // �i�[�X�^�b�N��
	INT *pos                // index�l
)
{
	INT i1 = (Stack->size);
	if(*pos < 0)
	{
		*pos += i1;
		if(*pos < 0)
		{
			*pos = -1;
		}
	}
	if(*pos > i1)
	{
		*pos = i1;
	}
}
//--------------
// Index�l�擾
//--------------
/* (��)
	P2(iStack_indexA(Stack, 0)); // {"a", "b", "c"} => "a"
*/
// v2019-08-16
MBS
*iStack_indexA(
	$struct_iStackA *Stack, // �i�[�X�^�b�N��
	INT index               // �v�f[n]
)
{
	if(!Stack)
	{
		return 0;
	}
	INT i1 = (Stack->size);
	iStack_IndexChkA(Stack, &index);
	if(index < 0 || index >= i1)
	{
		return 0;
	}
	return ims_clone(*((Stack->ary) + index)); // �R�s�[�n��
}
//-------
// �}��
//-------
/* (��)
	// {"a", "b", "c"} �̂Ƃ�
	iStack_insertA(Stack, 0, "A"); //=> {"A", "a", "b", "c"}
	iStack_insertA(Stack, 4, "D"); //=> {"A", "a", "b", "c", "D"}
*/
// v2019-08-16
BOOL
iStack_insertA(
	$struct_iStackA *Stack, // �i�[�X�^�b�N��
	INT to,                 // �}���ʒu�̌�̗v�f[n]
	MBS *ptr                // �i�[������
)
{
	if(!Stack)
{
		return FALSE;
	}
	INT i1 = (Stack->size);
	iStack_IndexChkA(Stack, &to);
	if(to < 0 || to > i1)
	{
		return FALSE;
	}
	++i1;
	MBS **a1 = (Stack->ary);
		a1 = irealloc_MBS_ary(a1, i1);
	INT i2 = i1;
	while(i2 > to)
	{
		*(a1 + i2) = *(a1 + i2 - 1);
		--i2;
	}
	*(a1 + to) = ims_clone(ptr);
	(Stack->size) = i1;
	(Stack->ary) = a1;
	return TRUE;
}
//-------
// �u��
//-------
/* (��)
	// {"a", "b", "c"} �̂Ƃ�
	iStack_replaceA(Stack, 1, "B"); //=> {"a", "B", "c"}
*/
// v2019-08-16
BOOL
iStack_replaceA(
	$struct_iStackA *Stack, // �i�[�X�^�b�N��
	INT to,                 // �u���ʒu�̗v�f[n]
	MBS *ptr                // �u����̕�����
)
{
	if(!Stack)
	{
		return FALSE;
	}
	INT i1 = (Stack->size);
	iStack_IndexChkA(Stack, &to);
	if(to < 0 || to >= i1)
	{
		return FALSE;
	}
	MBS **a1 = (Stack->ary);
	ifree(*(a1 + to));
	*(a1 + to) = ims_clone(ptr);
	return TRUE;
}
//-------
// �폜
//-------
/* (��)
	// {"a", "b", "c"} �̂Ƃ�
	iStack_removeA(Stack, 1); //=> {"a", "c"}
*/
// v2019-08-16
BOOL
iStack_removeA(
	$struct_iStackA *Stack, // �i�[�X�^�b�N��
	INT to                  // �v�f[n]
)
{
	if(!Stack)
	{
		return FALSE;
	}
	INT i1 = (Stack->size);
	iStack_IndexChkA(Stack, &to);
	if(to < 0 || to >= i1 || i1 < 1)
	{
		return FALSE;
	}
	MBS **a1 = (Stack->ary);
	ifree(*(a1 + to)); // �Y���f�[�^���폜
	--i1;              // �X�^�b�N�T�C�Y�����炷
	INT i2 = 0;
	while(i2 < i1)
	{
		*(a1 + to + i2) = *(a1 + to + i2 + 1);
		++i2;
	}
	a1 = irealloc_MBS_ary(a1, i1); // �X�^�b�N�T�C�Y�ύX
	(Stack->size) = i1;
	(Stack->ary) = a1;
	return TRUE;
}
/* (��)
	MBS *p1 = iStack_remove2A(Stack, 1); // {"a", "b", "c"} => "b"
	P82(p1);
	ifree(p1);
*/
// v2014-02-02
MBS
*iStack_remove2A(
	$struct_iStackA *Stack, // �i�[�X�^�b�N��
	INT to                  // �v�f[n]
)
{
	MBS *rtn = iStack_indexA(Stack, to);
	iStack_removeA(Stack, to);
	return rtn;
}
//--------------
// Stack������
//--------------
/* (��)
	// to   = {"a", "b", "c"}
	// from = ["123", "45"]
	P3(iStack_concatA(to, from)); //=> 5
	iStack_printA(to); //=> {"a", "b", "c", "123", "45"}
*/
// v2019-08-16
UINT
iStack_concatA(
	$struct_iStackA *to,
	$struct_iStackA *from
)
{
	INT i_from = iStack_sizeA(from);
	INT i1 = 0;
	while(i_from > i1)
	{
		iStack_insertA(to, (to->size), iStack_indexA(from, i1));
		++i1;
	}
	return iStack_sizeA(to);
}
//---------
// ���炷
//---------
/* (��)
	iStack_rotateA(Stack,  1); // {"a", "b", "c"} => {"c", "a", "b"}
	iStack_rotateA(Stack, -1); // {"a", "b", "c"} => {"b", "c", "a"}
*/
// v2019-08-16
BOOL
iStack_rotateA(
	$struct_iStackA *Stack, // �i�[�X�^�b�N��
	INT move                // �ړ���
)
{
	if(!move || !Stack || !(Stack->size))
	{
		return FALSE;
	}
	INT ary_size = (Stack->size);
	MBS **ary = (Stack->ary);
	INT i1 = 0, i2 = 0;
	// �ړ���
	while(move < 0)
	{
		move += ary_size;
	}
	while(move >= ary_size)
	{
		move -= ary_size;
	}
	// �ޔ�̈�쐬
	INT *aswap = icalloc(ary_size, sizeof(UINT), FALSE);
	i1 = 0;
	i2 = ary_size - move;
	while(move > i1)
	{
		*(aswap + i1) = (UINT)*(ary + i2 + i1); // (ary_size - move)..(move-1)
		++i1;
	}
	// �ޔ������̈�ɂ��炵�đ��
	i1 = ary_size - 1;
	while(move <= i1)
	{
		*(ary + i1) = *(ary + i1 - move); // (ary_size - 1-move) => (ary_size - 1)
		--i1;
	}
	// �ޔ�̈��擪�ɑ��
	i1 = 0;
	while(move > i1)
	{
		*(ary + i1) = (MBS*)*(aswap + i1); // 0..(move-1)
		++i1;
	}
	ifree(aswap);
	return TRUE;
}
//-------------------
// swap�͈̓`�F�b�N
//-------------------
/* (��)
	// (Stack->size) = 5 �̂Ƃ�
	iStack_swapIndexChkA(Stack, 4, 1); // {4, 1} => {1, 4}
*/
// v2013-02-02
VOID
iStack_swapIndexChkA(
	$struct_iStackA *Stack, // �i�[�X�^�b�N��
	INT *from,              // �v�f[n]
	INT *to                 // �v�f[n]
)
{
	iStack_IndexChkA(Stack, from);
	iStack_IndexChkA(Stack, to);
	if(*from > *to)
	{
		INT i1 = 0;
		i1 = *from;
		*from = *to;
		*from = i1;
	}
}
//-------
// swap
//-------
/* (��)
	iStack_swapA(Stack, 0, 2); // {"a", "b", "c"} => {"c", "b", "a"}
*/
// v2019-08-16
BOOL
iStack_swapA(
	$struct_iStackA *Stack, // �i�[�X�^�b�N��
	INT from,               // �v�f[n]
	INT to                  // �v�f[n]
)
{
	if(!Stack)
	{
		return FALSE;
	}
	INT i1 = (Stack->size);
	iStack_swapIndexChkA(Stack, &from, &to);
	if(from < 0 || from >= i1 || to < 0 || to >= i1)
	{
		return FALSE;
	}
	MBS **a1 = (Stack->ary);
	MBS *tmp = *(a1 + from);
		*(a1 + from) = *(a1 + to);
		*(a1 + to) = tmp;
	return TRUE;
}
//--------------------
// Stack�ɔz���}��
//--------------------
// v2014-03-05
UINT
iStack_pushAryA(
	$struct_iStackA *Stack,
	MBS **ary
)
{
	MBS *p1 = 0;
	UINT u1 = 0;
	while((p1 = *(ary + u1)))
	{
		iStack_pushA(Stack, p1);
		++u1;
	}
	return Stack->size;
}
//----------------------
// �������Stack�ɕϊ�
//----------------------
/* (��)
	$struct_iStackA *Stack = iStack_fromA("iwama, �����, ���", ", ");
	iStack_printA(Stack);
	iStack_freeA(Stack);
*/
// v2014-04-12
$struct_iStackA
*iStack_fromA(
	MBS *ptr,
	MBS *token // ���ʕ�����̂Ƃ��� "" ���w��
)
{
	$struct_iStackA *rtn = iStack_allocA();
	if(!token)
	{
		token = "";
	}
	MBS **ary = ija_token(ptr, token);
		iStack_pushAryA(rtn, ary);
	ifree(ary);
	return rtn;
}
//--------------------
// �z���Stack�ɕϊ�
//--------------------
/* (��)
	MBS **ary = iary_nnew("iwama", "�����", "���", NULL);
	iary_print(ary);
	$struct_iStackA *Stack = iStack_fromAryA(ary);
	ifree(ary);
	iStack_printA(Stack);
	iStack_freeA(Stack);
*/
// v2014-03-05
$struct_iStackA
*iStack_fromAryA(
	MBS **ary
)
{
	$struct_iStackA *rtn = iStack_allocA();
	iStack_pushAryA(rtn, ary);
	return rtn;
}
//------------------------
// Stack���當������擾
//------------------------
/* (��)
	MBS *str = iStack_popStrA(Stack, 0, 2);
*/
// v2020-05-08
MBS
*iStack_popStrA(
	$struct_iStackA *Stack, // �i�[�X�^�b�N��
	INT from,               // �擾�J�n�̗v�f[n]
	INT to                  // �擾�����̗v�f[n]
)
{
	if(!Stack)
	{
		return NULL;
	}
	iStack_swapIndexChkA(Stack, &from, &to);
	INT StackSize = (Stack->size);
		if(from < 0)
		{
			from = 0;
		}
		if(to >= StackSize)
		{
			to = (StackSize - 1);
		}
		if(from >= StackSize || to < 0)
		{
			return NULL;
		}
	MBS *rtn = 0, *p1 = 0;
	MBS **a1 = (Stack->ary);
	INT i1 = 0;
	while(from <= to)
	{
		p1 = ims_ncat_clone(rtn, *(a1 + from));
		ifree(rtn);
			rtn = p1;
		++from;
		++i1;
	}
	return rtn;
}
//----------------------
// Stack����z����擾
//----------------------
/* (��)
	MBS **ary = iStack_popAryA(Stack, 0, 2);
*/
// v2019-08-16
MBS
**iStack_popAryA(
	$struct_iStackA *Stack, // �i�[�X�^�b�N��
	INT from,               // �擾�J�n�̗v�f[n]
	INT to                  // �擾�����̗v�f[n]
)
{
	if(!Stack)
{
		return ima_null();
	}
	iStack_swapIndexChkA(Stack, &from, &to);
	INT i1 = (Stack->size);
	if(from < 0)
	{
		from = 0;
	}
	if(to >= i1)
	{
		to = (i1 - 1);
	}
	if(from >= i1 || to < 0)
	{
		return icalloc_MBS_ary(0);
	}
	MBS **a1 = icalloc_MBS_ary(to - from);
	MBS **a2 = (Stack->ary);
	INT i2 = 0;
	while(from <= to)
	{
		*(a1 + i2) = ims_clone(*(a2 + from));
		++from;
		++i2;
	}
	return a1;
}
//----------------------
// Stack�𕶎���ɕϊ�
//----------------------
/* (��)
	MBS *ptr = iStack_toA(Stack, "::");
*/
// v2014-04-15
MBS
*iStack_toA(
	$struct_iStackA *Stack,
	MBS *token
)
{
	MBS **ary = iStack_toAryA(Stack);
	MBS *rtn = iary_join(ary, token);
	ifree(ary);
	return rtn;
}
//------------
// Stack�ꗗ
//------------
/* (��)
	iStack_printA(Stack); // {"a", "b", "c"} => "a", "b", "c"
*/
// v2020-05-08
VOID
iStack_printA(
	$struct_iStackA *Stack // �i�[�X�^�b�N��
)
{
	NL();
	UINT u1 = 0;
	while(u1 < (Stack->size))
	{
		P("[%u]%s\n", u1 + 1, *((Stack->ary) + u1));
		++u1;
	}
	NL();
}
//-----------------
// �������^free()
//-----------------
/* (��)
	$struct_iStackDBL *Stack = iStack_allocDBL();
	iStack_freeDBL(Stack);
*/
// v2014-12-16
$struct_iStackDBL
*iStack_allocDBL()
{
	$struct_iStackDBL *Stack = ($struct_iStackDBL*)icalloc(1, sizeof($struct_iStackDBL), FALSE);
		icalloc_err(Stack);
	(Stack->size) = 0;
	(Stack->ary) = icallocDBL(0);
	return Stack;
}
// v2014-12-16
VOID
iStack_freeDBL(
	$struct_iStackDBL *Stack // �i�[�X�^�b�N��
)
{
	if(!Stack)
	{
		return;
	}
	ifree(Stack->ary);
	ifree(Stack);
}
//---------------------
// Stack[n]�̓��e����
//---------------------
/* (��)
	$struct_iStackDBL *Stack = iStack_allocDBL();
		iStack_pushDBL(Stack, 100);
			P833(Stack->size, Stack->ary[0]);
		iStack_pushDBL(Stack, 200);
			P833(Stack->size, Stack->ary[1]);
		iStack_eraseDBL(Stack);
			P833(Stack->size, Stack->ary[0]);
			P833(Stack->size, Stack->ary[1]);
	iStack_freeDBL(Stack);
*/
// v2015-12-23
BOOL
iStack_eraseDBL(
	$struct_iStackDBL *Stack // �i�[�X�^�b�N��
)
{
	if(!Stack)
	{
		return FALSE;
	}
	UINT u1 = 0;
	while(u1 < (Stack->size))
	{
		*((Stack->ary) + u1) = 0.0;
		++u1;
	}
	(Stack->size) = 0;
	return TRUE;
}
//----------------------
// Stack�̃N���[���쐬
//----------------------
/* (��)
	$struct_iStackDBL *Stack1 = iStack_allocDBL();
		iStack_pushDBL(Stack1, 12345);
	$struct_iStackDBL *Stack2 = iStack_cloneDBL(Stack1);
	P83(Stack1->size);
		iStack_printDBL(Stack1);
	P83(Stack2->size);
		iStack_printDBL(Stack2);
	iStack_freeDBL(Stack1);
	iStack_freeDBL(Stack2);
*/
// v2014-12-16
$struct_iStackDBL
*iStack_cloneDBL(
	$struct_iStackDBL *Stack
)
{
	$struct_iStackDBL *rtn = iStack_allocDBL();
	UINT u1 = 0;
	while(u1 < (Stack->size))
	{
		iStack_pushDBL(rtn, *((Stack->ary) + u1));
		++u1;
	}
	return rtn;
}
//-------
// size
//-------
/* (��)
	P3(iStack_sizeDBL(Stack)); // {100, 0, 200} => 3
*/
// v2014-12-16
UINT
iStack_sizeDBL(
	$struct_iStackDBL *Stack // �i�[�X�^�b�N��
)
{
	return (Stack ? Stack->size : 0);
}
//------------------
// index�l�`�F�b�N
//------------------
/* (��)
	// (Stack->size) = 5 �̂Ƃ�
	iStack_indexChkDBL(Stack, -1); //=> 4
	iStack_indexChkDBL(Stack,  6); //=> 5
*/
// v2019-08-16
VOID
iStack_indexChkDBL(
	$struct_iStackDBL *Stack, // �i�[�X�^�b�N��
	INT *pos                  // index�l
)
{
	INT i1 = (Stack->size);
	if(*pos < 0)
	{
		*pos += i1;
		if(*pos < 0)
		{
			*pos = -1;
		}
	}
	if(*pos > i1)
	{
		*pos = i1;
	}
}
//--------------
// Index�l�擾
//--------------
/* (��)
	P3(iStack_indexDBL(Stack, 0)); // {0, 1, 2} => 0
*/
// v2019-08-16
DOUBLE
iStack_indexDBL(
	$struct_iStackDBL *Stack, // �i�[�X�^�b�N��
	INT index                 // �v�f[n]
)
{
	if(!Stack)
	{
		return 0;
	}
	INT i1 = (Stack->size);
	iStack_indexChkDBL(Stack, &index);
	if(index < 0 || index >= i1)
	{
		return 0;
	}
	return *((Stack->ary) + index);
}
//-------
// �}��
//-------
/* (��)
	// {0, 1, 2} �̂Ƃ�
	iStack_insertDBL(Stack, 0, 100); //=> {100, 0, 1, 2}
	iStack_insertDBL(Stack, 4, 200); //=> {100, 0, 1, 2, 200}
*/
// v2019-08-16
BOOL
iStack_insertDBL(
	$struct_iStackDBL *Stack, // �i�[�X�^�b�N��
	INT to,                   // �}���ʒu�̌�̗v�f[n]
	DOUBLE num                // �i�[������
)
{
	if(!Stack)
	{
		return FALSE;
	}
	INT i1 = (Stack->size);
	iStack_indexChkDBL(Stack, &to);
	if(to < 0 || to > i1)
	{
		return FALSE;
	}
	++i1;
	DOUBLE *a1 = (Stack->ary);
		a1 = ireallocDBL(a1, i1);
	INT i2 = i1;
	while(i2 > to)
	{
		*(a1 + i2) = *(a1 + i2 - 1);
		--i2;
	}
	*(a1 + to) = num;
	(Stack->size) = i1;
	(Stack->ary) = a1;
	return TRUE;
}
//-------
// �u��
//-------
/* (��)
	// {100, 200, 300} �̂Ƃ�
	iStack_replaceDBL(Stack, 1, 0); //=> {100, 0, 300}
*/
// v2019-08-16
BOOL
iStack_replaceDBL(
	$struct_iStackDBL *Stack, // �i�[�X�^�b�N��
	INT to,                   // �u���ʒu�̗v�f[n]
	DOUBLE num                // �u����̕�����
)
{
	if(!Stack)
	{
		return FALSE;
	}
	INT i1 = (Stack->size);
	iStack_indexChkDBL(Stack, &to);
	if(to < 0 || to >= i1)
	{
		return FALSE;
	}
	*((Stack->ary) + to) = num;
	return TRUE;
}
//-------
// �폜
//-------
/* (��)
	// {100, 200, 300} �̂Ƃ�
	iStack_removeDBL(Stack, 1); //=> {100, 300}
*/
// v2019-08-16
BOOL
iStack_removeDBL(
	$struct_iStackDBL *Stack, // �i�[�X�^�b�N��
	INT to                    // �v�f[n]
)
{
	if(!Stack)
	{
		return FALSE;
	}
	INT i1 = (Stack->size);
	iStack_indexChkDBL(Stack, &to);
	if(to < 0 || to >= i1 || i1 < 1)
	{
		return FALSE;
	}
	DOUBLE *a1 = (Stack->ary);
	--i1; // �X�^�b�N�T�C�Y�����炷
	INT i2 = 0;
	while(i2 < i1)
	{
		*(a1 + to + i2) = *(a1 + to + i2 + 1);
		++i2;
	}
	a1 = ireallocDBL(a1, i1); // �X�^�b�N�T�C�Y�ύX
	(Stack->size) = i1;
	(Stack->ary) = a1;
	return TRUE;
}
/* (��)
	INT64 I1 = iStack_remove2DBL(Stack, 1); // {100, 200, 300} => 200
	P83(I1);
*/
// v2014-12-16
DOUBLE
iStack_remove2DBL(
	$struct_iStackDBL *Stack, // �i�[�X�^�b�N��
	INT to                    // �v�f[n]
)
{
	DOUBLE rtn = iStack_indexDBL(Stack, to);
	iStack_removeDBL(Stack, to);
	return rtn;
}
//--------------
// Stack������
//--------------
/* (��)
	// to   = {0, 1, 2}
	// from = {3, 4}
	P3(iStack_concatDBL(to, from)); //=> 5
	iStack_printDBL(to); //=> {0, 1, 2, 3, 4}
*/
// v2019-08-16
UINT
iStack_concatDBL(
	$struct_iStackDBL *to,
	$struct_iStackDBL *from
)
{
	INT i_from = iStack_sizeDBL(from);
	INT i1 = 0;
	while(i_from > i1)
	{
		iStack_insertDBL(to, (to->size), iStack_indexDBL(from, i1));
		++i1;
	}
	return iStack_sizeDBL(to);
}
//---------
// ���炷
//---------
/* (��)
	iStack_rotateDBL(Stack,  1); // {0, 1, 2} => {2, 0, 1}
	iStack_rotateDBL(Stack, -1); // {0, 1, 2} => {1, 2, 0}
*/
// v2019-08-16
BOOL
iStack_rotateDBL(
	$struct_iStackDBL *Stack, // �i�[�X�^�b�N��
	INT move                  // �ړ���
)
{
	if(!move || !Stack || !(Stack->size))
	{
		return FALSE;
	}
	INT ary_size = (Stack->size);
	DOUBLE *ary = (Stack->ary);
	INT i1 = 0, i2 = 0;
	// �ړ���
	while(move < 0)
	{
		move += ary_size;
	}
	while(move >= ary_size)
	{
		move -= ary_size;
	}
	// �ޔ�̈�쐬
	UINT *aswap = icalloc(ary_size, sizeof(UINT), FALSE);
	i1 = 0;
	i2 = ary_size - move;
	while(move > i1)
	{
		*(aswap + i1) = (UINT)*(ary + i2 + i1); // (ary_size - move)..(move - 1)
		++i1;
	}
	// �ޔ������̈�ɂ��炵�đ��
	i1 = ary_size - 1;
	while(move <= i1)
	{
		*(ary + i1) = *(ary + i1 - move); // (ary_size - 1-move) => (ary_size - 1)
		--i1;
	}
	// �ޔ�̈��擪�ɑ��
	i1 = 0;
	while(move > i1)
	{
		*(ary + i1) = (DOUBLE)*(aswap + i1); // 0..(move-1)
		++i1;
	}
	ifree(aswap);
	return TRUE;
}
//-------------------
// swap�͈̓`�F�b�N
//-------------------
/* (��)
	// (Stack->size) = 5 �̂Ƃ�
	iStack_swapIndex_chkDBL(Stack, 4, 1); // {4, 1} => {1, 4}
*/
// v2014-12-16
VOID
iStack_swapIndex_chkDBL(
	$struct_iStackDBL *Stack, // �i�[�X�^�b�N��
	INT *from,                // �v�f[n]
	INT *to                   // �v�f[n]
)
{
	iStack_indexChkDBL(Stack, from);
	iStack_indexChkDBL(Stack, to);
	if(*from > *to)
	{
		INT i1 = 0;
		i1 = *from;
		*from = *to;
		*from = i1;
	}
}
//-------
// swap
//-------
/* (��)
	iStack_swapDBL(Stack, 0, 2); // {0, 1, 2} => {2, 1, 0}
*/
// v2019-08-16
BOOL
iStack_swapDBL(
	$struct_iStackDBL *Stack, // �i�[�X�^�b�N��
	INT from,                 // �v�f[n]
	INT to                    // �v�f[n]
)
{
	if(!Stack)
	{
		return FALSE;
	}
	INT i1 = (Stack->size);
	iStack_swapIndex_chkDBL(Stack, &from, &to);
	if(from < 0 || from >= i1 || to < 0 || to >= i1)
	{
		return FALSE;
	}
	DOUBLE *a1 = (Stack->ary);
	DOUBLE tmp = *(a1 + from);
		*(a1 + from) = *(a1 + to);
		*(a1 + to) = tmp;
	return TRUE;
}
//------------
// Stack�ꗗ
//------------
/* (��)
	iStack_printDBL(Stack); // {0, 1, 2} => 0, 1, 2
*/
// v2020-05-08
VOID
iStack_printDBL(
	$struct_iStackDBL *Stack // �i�[�X�^�b�N��
)
{
	NL();
	UINT u1 = 0;
	while(u1 < (Stack->size))
	{
		P("[%u%]%.8f\n", u1, *((Stack->ary) + u1));
		++u1;
	}
	NL();
}
//---------------------
// Stack[n]�̓��e����
//---------------------
/* (��)
	UINT u1 = 0;
	MBS *p1 = "C, path, size, M, A";
	MBS **aSWitch = ija_token(p1, ",  "); // ",  "�Ƃ��ċ󔒖���
	MBS	*aCase1[] = {"number", "path", "dir", "name", "ext", "type", "attr", "size", "ctime", "mtime", "atime", NULL};
	MBS	*aCase2[] = {"N"     , "p"   , "d"  , "n"   , "e"  , "t"   , "a"   , "s"   , "C"    , "M"    , "A"    , NULL};
	$struct_iStackDBL *Stack = iStack_allocDBL();
		iStack_switchCaseDBL(Stack, aSWitch, aCase1, aCase2, 0, 7); // [0..5]
		u1 = 0;
		while(u1<Stack->size)
		{
			P84(Stack->ary[u1]);
			++u1;
		}
		iStack_eraseDBL(Stack);
		// aCase�͈̔͂�؂�ւ��Ďg�p
		iStack_switchCaseDBL(Stack, aSWitch, aCase1, aCase2, 8, 10); // [6..10]
		u1 = 0;
		while(u1<Stack->size)
		{
			P84(Stack->ary[u1]);
			++u1;
		}
	iStack_freeDBL(Stack);
	ifree(aSWitch);
*/
// v2015-12-23
BOOL
iStack_switchCaseDBL(
	$struct_iStackDBL *Stack,
	MBS **aSWitch,      // (��) {"size", "N", NULL}
	MBS **aCase1,       // ��r���̔z�� (��) {"number", "dir", "name", "size", NULL}
	MBS **aCase2,       // ���̗��L     (��) {"N"     , "d"  , "n"   , "s"   , NULL}
	UINT aCaseRangeBgn, // aCase�̌����J�n�ʒu[0..]         (��) 0
	UINT aCaseRangeEnd  // aCase�̌����I���ʒu[..(Array-1)] (��) 4
)
{
	UINT u1 = 0, u2 = 0;
	if(aCaseRangeBgn > aCaseRangeEnd)
	{
		u1 = aCaseRangeBgn;
		aCaseRangeBgn = aCaseRangeEnd;
		aCaseRangeEnd = u1;
	}
	UINT arySize = iary_size(aSWitch);
	MBS *p1 = 0;
	u1 = 0;
	while(u1 < arySize && (p1 = *(aSWitch + u1)))
	{
		u2 = aCaseRangeBgn;
		while(u2 <= aCaseRangeEnd)
		{
			if(imb_cmpp(p1, *(aCase1 + u2)) || imb_cmpp(p1, *(aCase2 + u2)))
			{
				iStack_pushDBL(Stack, u2);
				break;
			}
			++u2;
		}
		++u1;
	}
	return (Stack->size ? TRUE : FALSE);
}
//-------------------------------
// �����񂩂琔�l�z��𒊏o����
//-------------------------------
/* (��)
	MBS *str = "0-2,  5 - 10 , 11-, -12,  , 20, 0"; //=> 0, 1, 2, 5, 6, 7, 8, 9, 10, 11, 12, 20, 0
	$struct_iStackDBL *Stack = iStack_AtoDBL(str, 1.0);
	iStack_printDBL(Stack);
	iStack_freeDBL(Stack);
*/
// v2014-12-18
$struct_iStackDBL
*iStack_AtoDBL(
	MBS *str,
	DOUBLE dDiff
)
{
	$struct_iStackDBL *rtn = iStack_allocDBL();
	UINT u1 = 0;
	DOUBLE d1 = 0, d2 = 0, d3 = 0;
	MBS **ary1 = 0, **ary2 = 0;
	MBS *p1 = 0, *p2 = 0;
	ary1 = ija_token(str, ", ");
	u1 = 0;
	while((p1 = *(ary1 + u1)))
	{
		if(*(p2 = ijs_trimL(p1)))
		{
			ary2 = ija_token(p1, "-");
			if(*(ary2 + 0) && *(ary2 + 1))
			{
				d1 = inum_atof(*(ary2 + 0));
				d2 = inum_atof(*(ary2 + 1));
				if(d2 < d1)
				{
					d3 = d1;
					d1 = d2;
					d2 = d3;
				}
			}
			else
			{
				d3 = inum_atof(p1);
				if(d3 < 0)
				{
					d3 = -(d3);
				}
				d1 = d2 = d3;
			}
			ifree(ary2);
			// [n1...n2]=�v�f
			while(d1 <= d2)
			{
				iStack_pushDBL(rtn, d1);
				if(dDiff <= 0)
				{
					break;
				}
				d1 += dDiff;
			}
		}
		ifree(p2);
		++u1;
	}
	ifree(ary1);
	return rtn;
}
//--------------------------------
// �g�p����Drive���X�^�b�N�֏o��
//--------------------------------
/* (��)
	$struct_iStackA *Stack = iStack_drivelistA();
	iStack_printA(Stack); // Stack�ꗗ
	iStack_freeA(Stack);  // free()
*/
// v2014-03-08
$struct_iStackA
*iStack_drivelistA()
{
	CONST INT SJIS_A = 65;
	$struct_iStackA *Stack = iStack_allocA();
	MBS s1[4] = "";
	UINT u1 = 1, u2 = 0, bit = GetLogicalDrives();
	while(u1 < bit)
	{
		if(u1 & bit)
		{
			sprintf(s1, "%c:\\", (SJIS_A+u2)); // "[A-Z]:\"
			// �}�E���g����Ă�����̂����\��
			if(iFchk_typePathA(s1))
			{
				iStack_pushA(Stack, s1);
			}
		}
		u1 <<= 1;
		++u2;
	}
	return Stack;
}
