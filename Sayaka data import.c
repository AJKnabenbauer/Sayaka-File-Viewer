#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

// Virtual terminal color sequences
#define CNSL_COLOR_FRG_BLACK	  "\033[30m"
#define CNSL_COLOR_FRG_RED		  "\033[31m"
#define CNSL_COLOR_FRG_GREEN	  "\033[32m"
#define CNSL_COLOR_FRG_YELLOW	  "\033[33m"
#define CNSL_COLOR_FRG_BLUE		  "\033[34m"
#define CNSL_COLOR_FRG_MAGENTA	  "\033[35m"
#define CNSL_COLOR_FRG_CYAN		  "\033[36m"
#define CNSL_COLOR_FRG_WHITE	  "\033[37m"
#define CNSL_COLOR_FRG_EXTENDED	  "\033[38m"
#define CNSL_COLOR_FRG_DEFAULT	  "\033[39m"
#define CNSL_COLOR_FRG_BLACK_B	  "\033[90m"
#define CNSL_COLOR_FRG_RED_B	  "\033[91m"
#define CNSL_COLOR_FRG_GREEN_B	  "\033[92m"
#define CNSL_COLOR_FRG_YELLOW_B	  "\033[93m"
#define CNSL_COLOR_FRG_BLUE_B	  "\033[94m"
#define CNSL_COLOR_FRG_MAGENTA_B  "\033[95m"
#define CNSL_COLOR_FRG_CYAN_B	  "\033[96m"
#define CNSL_COLOR_FRG_WHITE_B	  "\033[97m"

#define MAX_MEM ((1UL<<20) * 5) // 5MB
#define MEM_CHUNK_SIZE ((1UL<<10) * 10) // 100 KB

// Note that .dp1 files are little endian 32 bit ints
typedef union D32 {
    uint8_t u8[4];
    int32_t i32;
} D32;

typedef union {
    D32 data[11];
    struct {
        D32 Unknown0;
        D32 Unknown1;
        D32 Unknown2;
        D32 Unknown3;
        D32 Unknown4;
        D32 Unknown5;
        D32 Unknown6;
        D32 cutDataEnd;
        D32 footerDataEnd;
        D32 lengthFile;
        D32 lineItems;
    };
} headerData;


// helper function that prints a D32 in different formats to the console
// and color codes it accordingly
void printHex(unsigned int row, D32* d, int newlineEn);

// helper function to close our file without having to 
// worry about dereferencing a null pointer
void safeClose(FILE** ppFile);

// helper function to free dynamically allocated memory without 
// having to worry about dereferencing a null pointer
void safeFree(void** ppBuffer);

// helper function to bulk load a binary file into a dynamically allocated buffer
// MAKE SURE TO FREE MEMORY AFTER USE
void* openAndLoadFile(char* cPath, size_t* sBufferSize);

// function responsible for parsing the data and formatting it in the console
void readAndPrintFile(char* cPath);

// function to enable virtual terminal processing
// When you run the .exe for this program on its own, the console window that
// gets created does not process ANSI escape sequences. We can explicitly enable
// the option on the stdOut handle
void enableConsoleColors();

// helper function to get last window error and display it in a message box, before
// calling exit
void WinError();


// program entry point

int main()
{
    char strInput[256] ={ 0 };

    enableConsoleColors();

    while (1)
    {
        printf(
            "\nEnter file path to sayaka .dp1 file. \n"
            "Make sure to include .dp1 extention \n"
            "Max file path length is 256 characters \n\n");

        fgets(strInput, sizeof(strInput), stdin);

        // fgets() leaves a trailing newline character that we have to remove
        strInput[strcspn(strInput, "\n")] = 0;

        printf("Attempting to open: %s \n", strInput);

        readAndPrintFile(strInput);
    }

    return 0;
}




void WinError()
{
    TCHAR* errorBuf = NULL;
    DWORD error = GetLastError();

    DWORD len = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |			//Allocate enough memory for the entire message
        FORMAT_MESSAGE_FROM_SYSTEM |				//Search system message table resources
        FORMAT_MESSAGE_IGNORE_INSERTS,				//Ignore insert sequences
        NULL,										//Location of the message definition (ignored if NULL)
        error,										//Error Message ID
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	//Message language ID
        (LPTSTR)&errorBuf,							//Buffer to recieve message string
        0,											//Min number of TCHARs to allocate buffer for
        NULL										//va_list* of insert arguments
    );

    DWORD lenBuffer = len + (sizeof(TCHAR) * 128);
    TCHAR* strBuffer = malloc(lenBuffer);

    if (strBuffer != NULL)
    {
        swprintf_s(strBuffer, lenBuffer / sizeof(TCHAR), L"Windows Error [%d]: %s \n", error, errorBuf);

        MessageBox(
            NULL,			    //Handle to partent window
            strBuffer,           //MessageBox Text
            NULL,			    //MessageBox Title (NULL = "Error")
            MB_OK |			    //OK button
            MB_ICONERROR |	    //Stop sign icon
            MB_SETFOREGROUND    //Force forground
        );
    }

    LocalFree(errorBuf); //FormatMessage uses LOCALALLOC so we have to use LOCALFREE
    free(strBuffer);
    exit(error);
}






void printHex(unsigned int row, D32* d, int newlineEn)
{
    // File position number
    printf(CNSL_COLOR_FRG_RED "%06u", row);
    printf(CNSL_COLOR_FRG_DEFAULT " | ");

    // Hex
    printf(CNSL_COLOR_FRG_YELLOW "%02x %02x %02x %02x",
        d->u8[0], d->u8[1], d->u8[2], d->u8[3]);
    printf(CNSL_COLOR_FRG_DEFAULT " -> ");

    // Uint8
    printf(CNSL_COLOR_FRG_CYAN "%03u %03u %03u %03u",
        d->u8[0], d->u8[1], d->u8[2], d->u8[3]);
    printf(CNSL_COLOR_FRG_DEFAULT " -> ");

    // Uint16
    printf(CNSL_COLOR_FRG_MAGENTA "%05u %05u",
        ((uint32_t)d->u8[0] | (((uint32_t)d->u8[1]) << 8)),
        ((uint32_t)d->u8[2] | (((uint32_t)d->u8[3]) << 8)));
    printf(CNSL_COLOR_FRG_DEFAULT " -> ");

    // Int32
    printf(CNSL_COLOR_FRG_GREEN "%d", d->i32);

    if (newlineEn != 0) printf(CNSL_COLOR_FRG_DEFAULT "\n");
}




void safeClose(FILE** ppFile)
{
    if (ppFile != NULL) {
        if (*ppFile != NULL) {
            fclose(*ppFile);
            *ppFile = NULL;
        }
    }
}




void safeFree(void** ppBuffer)
{
    if (ppBuffer != NULL) {
        if (*ppBuffer != NULL) {
            free(*ppBuffer);
            *ppBuffer = NULL;
        }
    }
}




void* openAndLoadFile(char* cPath, size_t* sBufferSize)
{
    uint8_t* pBuffer = NULL;
    uint8_t* pTemp = NULL;
    FILE* pFile = NULL;
    errno_t err = 0;
    size_t sizeAllocated = 1; // add additional byte for safety padding
    size_t bytesRead = 0;

    err = fopen_s(&pFile, cPath, "rb");

    if (err || !pFile)
    {
        printf("Unable to open file!\n");
        safeClose(&pFile);
        *sBufferSize = 0;
        return NULL;
    }

    *sBufferSize = 0;

    // keep increasing our memory pool and loading in chunks until 
    // we hit our limit, or reach EOF
    do {

        sizeAllocated += MEM_CHUNK_SIZE;

        pTemp = realloc(pBuffer, sizeAllocated);

        if (pTemp == NULL) {
            printf("Error allocating memory!\n");
            safeClose(&pFile);
            safeFree(&pBuffer);
            *sBufferSize = 0;
            return NULL;
        }

        pBuffer = pTemp;

        bytesRead = fread(pBuffer + *sBufferSize, 1, MEM_CHUNK_SIZE, pFile);
        *sBufferSize += bytesRead;

    } while (bytesRead == MEM_CHUNK_SIZE &&
        *sBufferSize < (MAX_MEM - MEM_CHUNK_SIZE));


    // once we have everything allocated we can shrink the memory down as needed 
    if (*sBufferSize < sizeAllocated)
    {
        pTemp = realloc(pBuffer, *sBufferSize);

        if (pTemp == NULL) {
            printf("Error allocating memory!\n");
            safeClose(&pFile);
            safeFree(&pBuffer);
            *sBufferSize = 0;
            return NULL;
        }

        pBuffer = pTemp;
    }

    printf("Allocated: %llu bytes of memory and read in file!\n", *sBufferSize);

    return pBuffer;
}




void readAndPrintFile(char* cPath)
{
    size_t sBufferSize = 0;
    D32* pBuffer = openAndLoadFile(cPath, &sBufferSize);

    if (pBuffer && sBufferSize > sizeof(headerData))
    {
        size_t sBufferSize_D32 = sBufferSize / sizeof(D32);
        size_t sCurrentD32 = 0;
        size_t sCutNum = 0;
        size_t sFileLength = 0;
        size_t sCutDataLength = 0;
        size_t i = 0;

        headerData* pHeader = (headerData*)pBuffer;

        D32* pD32 = (D32*)pBuffer;

        sFileLength = ((headerData*)pBuffer)->lengthFile.i32 / sizeof(D32);
        sCutDataLength = ((headerData*)pBuffer)->cutDataEnd.i32 / sizeof(D32);

        // there is still unique data stored after the called out file end position
        sFileLength += 14;

        printf(CNSL_COLOR_FRG_YELLOW_B
            "\n*****************************************************************\n"
            "File Header:"
            "\n*****************************************************************\n\n"
            CNSL_COLOR_FRG_DEFAULT);

        printHex(sCurrentD32, &pHeader->Unknown0, 1);
        sCurrentD32++;
        printHex(sCurrentD32, &pHeader->Unknown1, 1);
        sCurrentD32++;
        printHex(sCurrentD32, &pHeader->Unknown2, 1);
        sCurrentD32++;
        printHex(sCurrentD32, &pHeader->Unknown3, 1);
        sCurrentD32++;
        printHex(sCurrentD32, &pHeader->Unknown4, 1);
        sCurrentD32++;
        printHex(sCurrentD32, &pHeader->Unknown5, 1);
        sCurrentD32++;
        printHex(sCurrentD32, &pHeader->Unknown6, 1);
        sCurrentD32++;

        printHex(sCurrentD32, &pHeader->cutDataEnd, 0);
        printf(" / ending byte position of cut data\n");
        sCurrentD32++;

        printHex(sCurrentD32, &pHeader->footerDataEnd, 0);
        printf(" / ending byte position of footer data\n");
        sCurrentD32++;

        printHex(sCurrentD32, &pHeader->lengthFile, 0);
        printf(" / ending byte position of all file data\n");
        sCurrentD32++;

        printHex(sCurrentD32, &pHeader->cutDataEnd, 0);
        printf(" / total number of cut items (2 per cut for beggining and end)\n");
        sCurrentD32++;

        printf(CNSL_COLOR_FRG_YELLOW_B
            "\n*****************************************************************\n"
            "Cut Data:"
            "\n*****************************************************************\n\n"
            CNSL_COLOR_FRG_DEFAULT);


        while (sCurrentD32 < sCutDataLength)
        {
            if (i > 0)
            {
                if (i % 18 == 0) printf("\n");
                if (i % 9 == 0) printf("\n");
            }

            if (i % 9 == 0) {
                sCutNum += 1;
                printf("%06lld -> ", sCutNum);
            }

            printf("%ld ", pD32[sCurrentD32].i32);

            i++;
            sCurrentD32 += 1;
        }

        printf(CNSL_COLOR_FRG_YELLOW_B
            "\n\n*****************************************************************\n"
            "File Footer:"
            "\n*****************************************************************\n\n"
            CNSL_COLOR_FRG_DEFAULT);

        while (sCurrentD32 < sFileLength) {
            printHex(sCurrentD32, &pD32[sCurrentD32], 1);
            sCurrentD32++;
        }
    }

    else
    {
        printf("\nError reading sayaka file: %s\n", cPath);
    }

    safeFree(&pBuffer);

}




void enableConsoleColors()
{
    HANDLE m_hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    //HANDLE m_hStdin = INVALID_HANDLE_VALUE;
    //HANDLE m_hStderr = INVALID_HANDLE_VALUE;

    //m_hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    //m_hStdin = GetStdHandle(STD_INPUT_HANDLE);
    //m_hStderr = GetStdHandle(STD_ERROR_HANDLE);

    DWORD dwMode = 0;

    if (!GetConsoleMode(m_hStdout, &dwMode))
    {
        WinError();
    }

    BOOL good = SetConsoleMode(m_hStdout, (dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING));

    if (!good)
    {
        WinError();
    }
}