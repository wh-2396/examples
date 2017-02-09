#ifndef _DESTEST_H_
#define _DESTEST_H_

#include "BaseAll.h"

class DesTest : public CP11TestBase  
{
public:
	DesTest(char* dll_file_path);
	virtual ~DesTest();
	void Test(void);
private:
	void GenerateKey(void);
	void crypt_Single(void);
	void crypt_Update(void);
	
private:
	CK_OBJECT_HANDLE m_hKey;
};

#endif // _DESTEST_H_
