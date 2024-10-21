#include <windows.h>
#include <stdio.h>

//#define PORT "COM2"
#define BUFSIZE 3

OVERLAPPED ov;
char buf[BUFSIZE];
HANDLE hCom;

int main(void)

{
    DCB dcb;
    //HANDLE hCom;
    BOOL fSuccess;
    char buf[32];
    char* pcCommPort = "COM1";
    sprintf(buf, "\\\\.\\%s", pcCommPort);

    //int buf[BUFSIZE];
    DWORD dwWritten;
    int writefile_return;

    /********************************************************************************
      비동기 입출력은 WINDOWS 2000/NT 만 지원이 된다.
      즉,98SE ,ME 이하의 버전에서는 동작하지 않는다.
      CREATEfILE() 에서 플래그는 6번째 인수에 FILE_FLAG_OVERLAPPED 로  사용하면 된다.
      단, OVERLAPPED 구조체를 선언하고 사용해야 한다.왜냐하면 비동기 입출력 을 하는 동안
      계속 유효해야 하므로 전역으로 선언하고 사용을 해야 한다.
    *********************************************************************************
    */
    hCom = CreateFile("\\\\.\\COM16",
        GENERIC_READ | GENERIC_WRITE,
        0,    // must be opened with exclusive-access
        NULL, // no security attributes
        OPEN_EXISTING, // must use OPEN_EXISTING
        FILE_FLAG_OVERLAPPED,  //0,     not overlapped I/O
        NULL  // hTemplate must be NULL for comm devices
    );

    printf("createfile init\n");

    if (hCom == INVALID_HANDLE_VALUE)
    {
        // Handle the error.
        printf("CreateFile failed with error %d.\n", GetLastError());
        return (1);
    }

    // Build on the current configuration, and skip setting the size
    // of the input and output buffers with SetupComm.

    fSuccess = GetCommState(hCom, &dcb);

    printf("getcommstate test\n");

    if (!fSuccess)
    {
        // Handle the error.
        printf("GetCommState failed with error %d.\n", GetLastError());
        return (2);
    }
    else
    {
        ov.Offset = 0;
        ov.OffsetHigh = 0;
        ov.hEvent = NULL;

        printf("message input: ");
        fgets(buf, BUFSIZE, stdin);
        fputs(buf, stdout);

        /*************************************************************************
          비동기 입출력을 위해 writefile()의 마지막 인수에 NULL 을 전달 하지 않고
          OVERLAPPED구조체를 사용한다.이 구조체의 주소를 전달하면 된다.
          OVERLAPPED 구조체는 전역으로 선언했다.
        ************************************************************************
        */
        writefile_return = WriteFile(hCom, buf, BUFSIZE, &dwWritten, &ov);
        if (writefile_return == 0) //if writefile failed....
        {
            printf("writefile error!\n");
            return 0;
        }
        else
        {
            printf("writefile success!\n");
        }
    }

    CloseHandle(hCom);
    printf("Serial port %s successfully reconfigured.\n", pcCommPort);
    return (0);
}