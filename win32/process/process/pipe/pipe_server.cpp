#include <stdio.h>
#include <windows.h>

#define PIPE_NAME "\\\\.\\Pipe\\wh"

HANDLE g_hPipe = INVALID_HANDLE_VALUE;

int main()
{
	char buffer[1024];
	DWORD ReadNum;

	printf("test server.\n");
	g_hPipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, \
		PIPE_TYPE_BYTE|PIPE_READMODE_BYTE , 1, 0, 0, 1000, NULL);
	if(g_hPipe == INVALID_HANDLE_VALUE)
	{
		printf("Create name pipe failed!\n");
		goto out;
	}

	printf("Wait for connect...\n");
	if(ConnectNamedPipe(g_hPipe, NULL) == FALSE)
	{
		printf("Connect failed!\n");
		goto out;
	}
	printf("Connected.\n");

	while(1)
	{
		if(ReadFile(g_hPipe, buffer, sizeof(buffer), &ReadNum, NULL) == FALSE)
		{
			printf("Write failed!\n");
			break;
		}
		buffer[ReadNum] = 0;
		printf(buffer);
	}

out:
	printf("Close pipe.\n");
	CloseHandle(g_hPipe);
	system("pause");
	return 0;
}