
int CPPdll_add(int add1,int add2)
{
	return add1 + add2;
}

//��������غ����Ļ�����ʹ���������ַ�ʽ
_declspec(dllexport)int CPPdll_sub(int sub1,int sub2)
{
	return sub1 - sub2;
}

_declspec(dllexport)int CPPdll_sub(int sub1,int sub2, int sub3)
{
	return sub1 - sub2 - sub3;
}