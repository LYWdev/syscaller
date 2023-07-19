#include "syscallTest_v5.h"

//고쳐야하는 부분-1
#define TRACING_ON "/data/shared/tracing_on"                                   // tracing_on path
#define TRACING_TRACE "/data/shared/trace"                                     // trace path
#define TRACING_MARKER "/data/shared/trace_marker"                             // trace_marker path
#define TRACING_RESULT_FILE_PATH "/program/OpenTracingTest.txt" // trace log output path
#define FUNCTION_RESULT_FILE_PATH "/program/Open.txt"       // container log output path
#define KATA_FD_PATH "/tmp/test.txt"                                           // 카타컨테이너를 위한 fsync용 파일 경로
#define CONTAINER_TYPE 0                                                       // 0: 도커, 1: gVisor, 2: kata
#define PARAMETER_NUMBER 3                                                     // parameter 개수를 넣어주시오
#define SYSCALL_NAME "Open"                                                    // syscall이름을 적어주세요.
#define ARG1_TYPE ARG_PATHNAME                                                 // argument 타입을 argtype에서 복사 붙이기로 붙여주세요.
#define ARG2_TYPE ARG_LIST                                                     // OP와 LIST는 FLAG
#define ARG3_TYPE ARG_MODE_T
#define ARG4_TYPE ARG_UNDEFINED
#define ARG5_TYPE ARG_UNDEFINED
#define ARG6_TYPE ARG_UNDEFINED
#define FLAG_COUNT 1            //테스트할 시스템콜의 parameter중 flag인 parameter의 개수
#define FLAG_COUNT1 0
#define FLAG_COUNT2 0
#define FLAG_COUNT3 0
#define SYS_NUMBER 2
#define FAIL_COUNT 10     //몇번 실패하면 다음으로 넘어가는지
//고쳐야하는 부분-1 끝

//고쳐야하는 부분-2
char *parameterName[] = {
    "const char *filename"};
int parameterNameCount;

char syscallName[100];

// 테스트하는 애의 플래그

unsigned long o_flags_base[] = {
     O_CREAT, O_RDONLY, O_WRONLY, O_RDWR};   //,
char *o_flags_base_char[] = {
    "O_CREAT","O_RDONLY", "O_WRONLY", "O_RDWR"}; //"O_CREAT"
int flagBaseSize;
int o_flags_base_index;
unsigned long *o_flags_base_set;
unsigned int o_flags_base_set_size;
// 선택 플래그
unsigned long o_flags[] = {
    O_APPEND, O_ASYNC, O_CLOEXEC, O_DIRECT, O_DIRECTORY, O_DSYNC, O_EXCL, O_NOATIME, O_NOCTTY, O_NOFOLLOW, O_NONBLOCK, O_NDELAY, O_PATH, O_SYNC, O_TMPFILE, O_TMPFILE, O_TRUNC};
char *o_flags_char[] = {
    "O_APPEND", "O_ASYNC", "O_CLOEXEC", "O_DIRECT", "O_DIRECTORY", "O_DSYNC", "O_EXCL", "O_NOATIME", "O_NOCTTY", "O_NOFOLLOW", "O_NONBLOCK", "O_NDELAY", "O_PATH", "O_SYNC", "O_TMPFILE", "O_TMPFILE", "O_TRUNC"};


// 선행조건1 플래그
unsigned long o_flags_base1[] = {
    };
char *o_flags_base1_char[] = {
    }; //"O_CREAT"



unsigned long o_flags1[] = {
    };
char *o_flags1_char[] = {
    };


// 선행조건2 플래그
unsigned long o_flags_base2[] = {
    };
char *o_flags_base2_char[] = {
    }; //"O_CREAT"


// 선택 플래그
unsigned long o_flags2[] = {
    };
char *o_flags2_char[] = {
    };


// 선행조건3 플래그
unsigned long o_flags_base3[] = {
    };
char *o_flags_base3_char[] = {
   }; //"O_CREAT"


// 선택 플래그
unsigned long o_flags3[] = {
    };
char *o_flags3_char[] = {
    };

//고쳐야하는 부분-2 끝

int prepareTest(syscallInfo *rec, int *next)
{
    /* 작성 부분 rec의 a1, a2, a3, a4, a5, a6를 전부 채워야함
     * 필요 없는 부분은 안채워도 됨
     * 블로킹 되는 함수의 경우  signal_alarm()을 사용해서 특정 시간 후에 블로킹을 풀어줄수 있도록 함
     * flag가 있으면 flag부터 할 수 있도록
     */
    //고쳐야하는 부분-3
    /*********************여기서부터 작성*************************/
    int nextLogic = 0;
    resetArgumentRec(rec);
    nextLogic = getFlag(rec, 2, 0, *next);
    if(nextLogic)
    {
        return 1;
    }

    char *fname = (char *)malloc(sizeof(char) * (strlen("/tmp/open_test.txt") + 1));
    rec->a1 = fname;
    strcpy(fname,"/tmp/open_test.txt");
    rec->a3 = 0666;
    /************************이까지 작성**********************/
    //고쳐야하는 부분-3 끝
    makeLogBuffer(rec, 1);
    return 0;
}

int finishTest(syscallInfo *rec, int *next)
{
    char cmd[10000] = {0};
    makeLogBuffer(rec, 0);
    FILE *fp = NULL;
        fp = fopen(FUNCTION_RESULT_FILE_PATH, "a");
        if (fp == NULL)
        {
#ifdef __DEBUG
            printf("finishTest file open failed...\n");
            return -1;
#endif
        }
        fprintf(stdout, "%s", rec->logBuffer);
        fprintf(fp, "%s", rec->logBuffer);
        fclose(fp);
    if (rec->testSuccess)
    {
        rec->failCount = 0;
        *next = 1;
    }
    else
    {
        rec->failCount++;
        *next = 0;
    }
    if(rec->kindOfContainer == 2)
    {
        sprintf(cmd, "sshpass -p 1234 scp /program/%s.txt kataworker@192.168.122.109:/home/kataworker/syscallresult", SYSCALL_NAME);
        system(cmd);
    }
    //고쳐야하는 부분-4
    /*********** 자원 반환이나 추가적으로 해야할 것 있으면 아래에 작성할 것*********************/

    if (rec->testSuccess)
    {
        free((char *)rec->a1);
        close(rec->retval);
    }

    /***********************이까지 작성할 것**********************************/
    //고쳐야하는 부분-4 끝
    resetRecFlagBufferUsed(rec);
    resetRecFlagBuffer(rec);
    if(rec->failCount >= FAIL_COUNT)
    {
        rec->failCount = 0;
        *next = 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    syscallInfo rec = {};
    int next = 0;
    int end = 0;
    initTest(&rec);
    while (!end)
    {
        end = prepareTest(&rec, &next); // 준비 과정에서 더이상 할게 없으면 end로 1을 반환 하면 테스트 종료됨
        if (end == 1)
        {
            break;
        }
        if (rec.kindOfContainer != 2)
        {
            tracingOn(1);
        }
        doTest(&rec);
        if (rec.kindOfContainer != 2)
        {
            tracingOn(0);
            trace_file_make(&rec);
        }
        finishTest(&rec, &next); // 성공할 때는 next에 1을 반환 아니면 0을 반환
    }

    freeHeap(); // 자원반환 코드
    return 0;
}

void signal_handler(int signo)
{
#ifdef __DEBUG
    printf("sigalrm happened!\n");
#endif
}

void signal_alarm(int time)         //블로킹 되면 깨워줘야할 때 사용
{
#ifdef __DEBUG
    printf("sigalrm register!\n");
#endif
    alarm(time);
}

void makePowerSet(unsigned long *flag_arr, unsigned long *flag_power_arr, int size)
{
    int idx = 0;
    // 멱집합은 2^n개의 부분집합으로 이루어지므로 1 << n 개만큼을 구해야함
    for (int i = 0; i < (1 << size); i++)
    {
        for (int j = 0; j < size; j++)
        {
            if ((i & (1 << j)) >= 1)
            {
                // 1을 j만큼 << shift하면, 그 공간은 0으로 채워짐.
                // 따라서, 각 멱집합에 해당하는 비트 형태의 i와 1 << j를 AND 연산했을 때,
                // 해당 원소가 켜져있으면(1), 그 값은 >= 1이 되고,
                // 현재 검사하고 있는 멱집합의 원소로 포함되는 것
                // printf("%d ", flag_arr[j]);
                flag_power_arr[idx] |= flag_arr[j];
            }
        }
        idx++;
    }
}

void initTest(syscallInfo *rec)
{
    signal(SIGALRM, signal_handler); // 시그널 함수 등록해놓기
    rec->kindOfContainer = CONTAINER_TYPE;
    parameterNameCount = PARAMETER_NUMBER;
    strcpy(syscallName, SYSCALL_NAME);
    rec->nr = SYS_NUMBER;
    rec->a1Type = ARG1_TYPE;
    rec->a2Type = ARG2_TYPE;
    rec->a3Type = ARG3_TYPE;
    rec->a4Type = ARG4_TYPE;
    rec->a5Type = ARG5_TYPE;
    rec->a6Type = ARG6_TYPE;
    rec->flagCount = FLAG_COUNT;
    rec->flagCount1 = FLAG_COUNT1;
    rec->flagCount2 = FLAG_COUNT2;
    rec->flagCount3 = FLAG_COUNT3;
    // 테스트 하는 시스템 콜
    flagBaseSize = sizeof(o_flags_base) / sizeof(unsigned long);

    if (flagBaseSize != 0)
    {
        o_flags_base_set_size = 1 << flagBaseSize;
        o_flags_base_set = malloc(sizeof(unsigned long) * o_flags_base_set_size);
        makePowerSet(o_flags_base, o_flags_base_set, flagBaseSize);
    }

    flagSize = sizeof(o_flags) / sizeof(unsigned long);
    
    if (flagSize != 0)
    {
        o_flags_set_size = 1 << flagSize;
        o_flags_set = malloc(sizeof(unsigned long) * o_flags_set_size);
        makePowerSet(o_flags, o_flags_set, flagSize);
    }

    //출력
    // 선행조건1
    flagBaseSize1 = sizeof(o_flags_base1) / sizeof(unsigned long);

    if (flagBaseSize1 != 0)
    {
        o_flags_base_set_size1 = 1 << flagBaseSize1;
        o_flags_base_set1 = malloc(sizeof(unsigned long) * o_flags_base_set_size1);
        makePowerSet(o_flags_base1, o_flags_base_set1, flagBaseSize1);
    }

    flagSize1 = sizeof(o_flags1) / sizeof(unsigned long);

    if (flagSize1 != 0)
    {
        o_flags_set_size1 = 1 << flagSize1;
        o_flags_set1 = malloc(sizeof(unsigned long) * o_flags_set_size1);
        makePowerSet(o_flags1, o_flags_set1, flagSize1);
    }

    // 선행조건2
    flagBaseSize2 = sizeof(o_flags_base2) / sizeof(unsigned long);

    if (flagBaseSize2 != 0)
    {
        o_flags_base_set_size2 = 1 << flagBaseSize2;
        o_flags_base_set2 = malloc(sizeof(unsigned long) * o_flags_base_set_size2);
        makePowerSet(o_flags_base2, o_flags_base_set2, flagBaseSize2);
    }

    flagSize2 = sizeof(o_flags2) / sizeof(unsigned long);

    if (flagSize2 != 0)
    {
        o_flags_set_size2 = 1 << flagSize2;
        o_flags_set2 = malloc(sizeof(unsigned long) * o_flags_set_size2);
        makePowerSet(o_flags2, o_flags_set2, flagSize2);
    }

    // 선행조건3
    flagBaseSize3 = sizeof(o_flags_base3) / sizeof(unsigned long);

    if (flagBaseSize3 != 0)
    {
        o_flags_base_set_size3 = 1 << flagBaseSize3;
        o_flags_base_set3 = malloc(sizeof(unsigned long) * o_flags_base_set_size3);
        makePowerSet(o_flags_base3, o_flags_base_set3, flagBaseSize3);
    }

    flagSize3 = sizeof(o_flags3) / sizeof(unsigned long);

    if (flagSize3 != 0)
    {
        o_flags_set_size3 = 1 << flagSize3;
        o_flags_set3 = malloc(sizeof(unsigned long) * o_flags_set_size3);
        makePowerSet(o_flags3, o_flags_set3, flagSize3);
    }
}

void tracingOn(int num)
{
    int tracingFD = -1;
    tracingFD = open(TRACING_ON, O_RDWR);
    if (num == 1)
    {
        if (tracingFD != -1)
        {
            write(tracingFD, "1", 2);
            close(tracingFD);
        }
    }
    else
    {
        if (tracingFD != -1)
        {
            write(tracingFD, "0", 2);
            close(tracingFD);
        }
    }
}

void trace_file_make(struct syscallInfo *rec)
{
    FILE *tracing_fp = NULL;
    FILE *tracing_trace_fp = NULL;
    char tracing_file_name[100] = {0};
    char tracing_trace_buffer[1000];
    char tracing_trace_buffer_temp[1000];
    char tracing_trace_buffer_temp2[1000];
    strcpy(tracing_file_name, TRACING_RESULT_FILE_PATH);
    int ftrace_pid_number = -1;
    int mode = 0; // 1이 입력
    char *token;
    char *pid_number_ptr;
    char pid_number[100] = {0};

    tracing_fp = fopen(tracing_file_name, "a");
    if (tracing_fp == NULL)
    {
        printf("open failed tracing_fp...\n");
        return;
    }

    tracing_trace_fp = fopen(TRACING_TRACE, "r");
    if (tracing_trace_fp == NULL)
    {
        printf("open failed tracing_fd...\n");
        return;
    }
    while (!feof(tracing_trace_fp))
    {
        fgets(tracing_trace_buffer, 900, tracing_trace_fp);
        strcpy(tracing_trace_buffer_temp, tracing_trace_buffer);
        token = strtok(tracing_trace_buffer_temp, " ");
        while (token != NULL)
        {
            if (!strcmp(token, "tracing_mark_write:"))
            {
                if (mode == 0) // start
                {
                    mode = 1;
                }
                else // end
                {
                    mode = 2;
                }
                break;
            }
            token = strtok(NULL, " ");
        }
        if (mode != 0)
        {
            if (strstr(tracing_trace_buffer, "exe-"))
            {
                fprintf(tracing_fp, "%s", tracing_trace_buffer);
            }
            else if (strstr(tracing_trace_buffer, "<...>-"))
            {
                fprintf(tracing_fp, "%s", tracing_trace_buffer);
            }
        }
        if (mode == 2)
        {
            mode = 0;
        }
    }
    fclose(tracing_trace_fp);
    fclose(tracing_fp);
    tracing_trace_fp = fopen(TRACING_TRACE, "w");
    fprintf(tracing_trace_fp, "%s", "");
    fclose(tracing_trace_fp);
}

// 0: base 1: base_set 2: flag, 3: flag_set
unsigned long resetFlagIndex(int kindOfFlag)
{
    switch (kindOfFlag)
    {
    case 0:
        o_flags_base_index = 0;
        break;
    case 1:
        o_flags_base_set_index = 0;
        break;
    case 2:
        o_flags_index = 0;
        break;
    case 3:
        o_flags_set_index = 0;
        break;
    default:
#ifdef __DEBUG
        printf("[error]resetFlagIndex out of range()\n");
#endif
        break;
    }
}

unsigned long resetFlagIndex1(int kindOfFlag)
{
    switch (kindOfFlag)
    {
    case 0:
        o_flags_base_index1 = 0;
        break;
    case 1:
        o_flags_base_set_index1 = 0;
        break;
    case 2:
        o_flags_index1 = 0;
        break;
    case 3:
        o_flags_set_index1 = 0;
        break;
    default:
#ifdef __DEBUG
        printf("[error]resetFlagIndex out of range()\n");
#endif
        break;
    }
}

unsigned long resetFlagIndex2(int kindOfFlag)
{
    switch (kindOfFlag)
    {
    case 0:
        o_flags_base_index2 = 0;
        break;
    case 1:
        o_flags_base_set_index2 = 0;
        break;
    case 2:
        o_flags_index2 = 0;
        break;
    case 3:
        o_flags_set_index2 = 0;
        break;
    default:
#ifdef __DEBUG
        printf("[error]resetFlagIndex out of range()\n");
#endif
        break;
    }
}

unsigned long resetFlagIndex3(int kindOfFlag)
{
    switch (kindOfFlag)
    {
    case 0:
        o_flags_base_index3 = 0;
        break;
    case 1:
        o_flags_base_set_index3 = 0;
        break;
    case 2:
        o_flags_index3 = 0;
        break;
    case 3:
        o_flags_set_index3 = 0;
        break;
    default:
#ifdef __DEBUG
        printf("[error]resetFlagIndex out of range()\n");
#endif
        break;
    }
}

// 0: base 1: base_set 2: flag, 3: flag_set
//syscall 정보, 
unsigned long getFlag(struct syscallInfo *rec, int argmentNumber, int kindOfFlag, int next)
{
    unsigned long flags = -1;
    switch (kindOfFlag)
    {
    case 0: // base_flag
        if (next)
        {
            o_flags_base_index++;
            if (o_flags_base_index >= flagBaseSize)
            {
                return 1;
            }
        }
        flags = o_flags_base[o_flags_base_index];
        break;
    case 1: // base_flag_set
        if (next)
        {
            o_flags_base_set_index++;
            if (o_flags_base_set_index >= o_flags_base_set_size)
            {
                return 1;
            }
        }
        flags = o_flags_base_set[o_flags_base_set_index];
        break;
    case 2: // flag
        if (next)
        {
            o_flags_index++;
            if (o_flags_index >= flagSize)
            {
                return 1;
            }
        }
        flags = o_flags[o_flags_index];
        break;
    case 3: // flag_set
        if (next)
        {
            o_flags_set_index++;
            if (o_flags_set_index >= o_flags_set_size)
            {
                return 1;
            }
        }
        flags = o_flags_set[o_flags_set_index];
        break;
    }
    switch (argmentNumber)
    {
    case 1:
        rec->a1 |= flags;
        break;
    case 2:
        rec->a2 |= flags;
        break;
    case 3:
        rec->a3 |= flags;
        break;
    case 4:
        rec->a4 |= flags;
        break;
    case 5:
        rec->a5 |= flags;
        break;
    case 6:
        rec->a6 |= flags;
        break;
    }
    return 0;
}

unsigned long getFlag1(struct syscallInfo *rec, int argmentNumber, int kindOfFlag, int next)
{
    unsigned long flags = -1;
    switch (kindOfFlag)
    {
    case 0: // base_flag
        if (next)
        {
            o_flags_base_index1++;
            if (o_flags_base_index1 >= flagBaseSize1)
            {
                return 1;
            }
        }
        flags = o_flags_base1[o_flags_base_index1];
        break;
    case 1: // base_flag_set
        if (next)
        {
            o_flags_base_set_index1++;
            if (o_flags_base_set_index1 >= o_flags_base_set_size1)
            {
                return 1;
            }
        }
        flags = o_flags_base_set1[o_flags_base_set_index1];
        break;
    case 2: // flag
        if (next)
        {
            o_flags_index1++;
            if (o_flags_index1 >= flagSize1)
            {
                return 1;
            }
        }
        flags = o_flags1[o_flags_index1];
        break;
    case 3: // flag_set
        if (next)
        {
            o_flags_set_index1++;
            if (o_flags_set_index1 >= o_flags_set_size1)
            {
                return 1;
            }
        }
        flags = o_flags_set1[o_flags_set_index1];
        break;
    }
    switch (argmentNumber)
    {
    case 1:
        rec->a1 |= flags;
        break;
    case 2:
        rec->a2 |= flags;
        break;
    case 3:
        rec->a3 |= flags;
        break;
    case 4:
        rec->a4 |= flags;
        break;
    case 5:
        rec->a5 |= flags;
        break;
    case 6:
        rec->a6 |= flags;
        break;
    }
    return 0;
}

unsigned long getFlag2(struct syscallInfo *rec, int argmentNumber, int kindOfFlag, int next)
{
    unsigned long flags = -1;
    switch (kindOfFlag)
    {
    case 0: // base_flag
        if (next)
        {
            o_flags_base_index2++;
            if (o_flags_base_index2 >= flagBaseSize2)
            {
                return 1;
            }
        }
        flags = o_flags_base2[o_flags_base_index2];
        break;
    case 1: // base_flag_set
        if (next)
        {
            o_flags_base_set_index2++;
            if (o_flags_base_set_index2 >= o_flags_base_set_size2)
            {
                return 1;
            }
        }
        flags = o_flags_base_set2[o_flags_base_set_index2];
        break;
    case 2: // flag
        if (next)
        {
            o_flags_index2++;
            if (o_flags_index2 >= flagSize2)
            {
                return 1;
            }
        }
        flags = o_flags2[o_flags_index2];
        break;
    case 3: // flag_set
        if (next)
        {
            o_flags_set_index2++;
            if (o_flags_set_index2 >= o_flags_set_size2)
            {
                return 1;
            }
        }
        flags = o_flags_set2[o_flags_set_index2];
        break;
    }
    switch (argmentNumber)
    {
    case 1:
        rec->a1 |= flags;
        break;
    case 2:
        rec->a2 |= flags;
        break;
    case 3:
        rec->a3 |= flags;
        break;
    case 4:
        rec->a4 |= flags;
        break;
    case 5:
        rec->a5 |= flags;
        break;
    case 6:
        rec->a6 |= flags;
        break;
    }
    return 0;
}

unsigned long getFlag3(struct syscallInfo *rec, int argmentNumber, int kindOfFlag, int next)
{
    unsigned long flags = -1;
    switch (kindOfFlag)
    {
    case 0: // base_flag
        if (next)
        {
            o_flags_base_index3++;
            if (o_flags_base_index3 >= flagBaseSize3)
            {
                return 1;
            }
        }
        flags = o_flags_base3[o_flags_base_index3];
        break;
    case 1: // base_flag_set
        if (next)
        {
            o_flags_base_set_index3++;
            if (o_flags_base_set_index3 >= o_flags_base_set_size3)
            {
                return 1;
            }
        }
        flags = o_flags_base_set3[o_flags_base_set_index3];
        break;
    case 2: // flag
        if (next)
        {
            o_flags_index3++;
            if (o_flags_index3 >= flagSize3)
            {
                return 1;
            }
        }
        flags = o_flags3[o_flags_index3];
        break;
    case 3: // flag_set
        if (next)
        {
            o_flags_set_index3++;
            if (o_flags_set_index3 >= o_flags_set_size3)
            {
                return 1;
            }
        }
        flags = o_flags_set3[o_flags_set_index3];
        break;
    }
    switch (argmentNumber)
    {
    case 1:
        rec->a1 |= flags;
        break;
    case 2:
        rec->a2 |= flags;
        break;
    case 3:
        rec->a3 |= flags;
        break;
    case 4:
        rec->a4 |= flags;
        break;
    case 5:
        rec->a5 |= flags;
        break;
    case 6:
        rec->a6 |= flags;
        break;
    }
    return 0;
}

unsigned long makeFlagToString(struct syscallInfo *rec, int argmentNumber, int kindOfFlag)
{
    char flagBuffer[10000] = {0};
    unsigned long flags = 0;
    int prevFlag = 0;
    if(strlen(rec->flagBuffer))
    {
        prevFlag = 1;
    }
    switch (argmentNumber)
    {
    case 1:
        flags = rec->a1;
        break;
    case 2:
        flags = rec->a2;
        break;
    case 3:
        flags = rec->a3;
        break;
    case 4:
        flags = rec->a4;
        break;
    case 5:
        flags = rec->a5;
        break;
    case 6:
        flags = rec->a6;
        break;
    }
    switch (kindOfFlag)     //set은 제외함 필요없음
    {
    case 0: // base_flag
        for (int i = 0; i < flagBaseSize; ++i)
        {
            if (flags & o_flags_base[i])
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags_base_char[i]);
                prevFlag = 1;
            }
            else if(flags == 0 && o_flags_base[i] == 0)
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags_base_char[i]);
                prevFlag = 1;
            }
        }
        break;
    case 2: // flag               
        for (int i = 0; i < flagSize; ++i)
        {
            if (flags & o_flags[i])
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags_char[i]);
                prevFlag = 1;
            }
            else if(flags == 0 && o_flags[i] == 0)
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags_char[i]);
                prevFlag = 1;
            }
        }
        break;
    }
    strcpy(rec->flagBuffer,flagBuffer);
    return 0;
}

unsigned long makeFlagToString1(struct syscallInfo *rec, int argmentNumber, int kindOfFlag)
{
    char flagBuffer[10000] = {0};
    unsigned long flags = 0;
    int prevFlag = 0;
    switch (argmentNumber)
    {
    case 1:
        flags = rec->a1;
        break;
    case 2:
        flags = rec->a2;
        break;
    case 3:
        flags = rec->a3;
        break;
    case 4:
        flags = rec->a4;
        break;
    case 5:
        flags = rec->a5;
        break;
    case 6:
        flags = rec->a6;
        break;
    }
    switch (kindOfFlag)     //set은 제외함 필요없음
    {
    case 0: // base_flag
        for (int i = 0; i < flagBaseSize1; ++i)
        {
            if (flags & o_flags_base1[i])
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags_base1_char[i]);
                prevFlag = 1;
            }
            else if(flags == 0 && o_flags_base1[i] == 0)
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags_base1_char[i]);
                prevFlag = 1;
            }
        }
        break;
    case 2: // flag               
        for (int i = 0; i < flagSize1; ++i)
        {
            if (flags & o_flags1[i])
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags1_char[i]);
                prevFlag = 1;
            }
            else if(flags == 0 && o_flags1[i] == 0)
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags1_char[i]);
                prevFlag = 1;
            }
        }
        break;
    }
    strcpy(rec->flagBuffer1,flagBuffer);
    return 0;
}

unsigned long makeFlagToString2(struct syscallInfo *rec, int argmentNumber, int kindOfFlag)
{
    char flagBuffer[10000] = {0};
    unsigned long flags = 0;
    int prevFlag = 0;
    switch (argmentNumber)
    {
    case 1:
        flags = rec->a1;
        break;
    case 2:
        flags = rec->a2;
        break;
    case 3:
        flags = rec->a3;
        break;
    case 4:
        flags = rec->a4;
        break;
    case 5:
        flags = rec->a5;
        break;
    case 6:
        flags = rec->a6;
        break;
    }
    switch (kindOfFlag)     //set은 제외함 필요없음
    {
    case 0: // base_flag
        for (int i = 0; i < flagBaseSize2; ++i)
        {
            if (flags & o_flags_base2[i])
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags_base2_char[i]);
                prevFlag = 1;
            }
            else if(flags == 0 && o_flags_base2[i] == 0)
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags_base2_char[i]);
                prevFlag = 1;
            }
        }
        break;
    case 2: // flag               
        for (int i = 0; i < flagSize2; ++i)
        {
            if (flags & o_flags2[i])
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags2_char[i]);
                prevFlag = 1;
            }
            else if(flags == 0 && o_flags2[i] == 0)
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags2_char[i]);
                prevFlag = 1;
            }
        }
        break;
    }
    strcpy(rec->flagBuffer2,flagBuffer);
    return 0;
}

unsigned long makeFlagToString3(struct syscallInfo *rec, int argmentNumber, int kindOfFlag)
{
    char flagBuffer[10000] = {0};
    unsigned long flags = 0;
    int prevFlag = 0;
    switch (argmentNumber)
    {
    case 1:
        flags = rec->a1;
        break;
    case 2:
        flags = rec->a2;
        break;
    case 3:
        flags = rec->a3;
        break;
    case 4:
        flags = rec->a4;
        break;
    case 5:
        flags = rec->a5;
        break;
    case 6:
        flags = rec->a6;
        break;
    }
    switch (kindOfFlag)     //set은 제외함 필요없음
    {
    case 0: // base_flag
        for (int i = 0; i < flagBaseSize3; ++i)
        {
            if (flags & o_flags_base3[i])
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags_base3_char[i]);
                prevFlag = 1;
            }
            else if(flags == 0 && o_flags_base3[i] == 0)
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags_base3_char[i]);
                prevFlag = 1;
            }
        }
        break;
    case 2: // flag               
        for (int i = 0; i < flagSize3; ++i)
        {
            if (flags & o_flags3[i])
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags3_char[i]);
                prevFlag = 1;
            }
            else if(flags == 0 && o_flags3[i] == 0)
            {
                if (prevFlag == 1)
                {
                    strcat(flagBuffer, " | ");
                }
                strcat(flagBuffer, o_flags3_char[i]);
                prevFlag = 1;
            }
        }
        break;
    }
    strcpy(rec->flagBuffer3,flagBuffer);
    return 0;
}

char *decode_argtype(struct syscallInfo *rec,char *sptr, unsigned long reg, enum argtype type, unsigned int argnum)
{
    switch (type)
    {
    case ARG_PATHNAME:
        sptr += sprintf(sptr, "\"%s\"", (char *)reg);
        break;
    case ARG_PID:
    case ARG_FD:
    case ARG_SOCKETINFO:
        sptr += sprintf(sptr, "%ld", (long)reg);
        break;
    case ARG_MODE_T:
        sptr += sprintf(sptr, "%o", (mode_t)reg);
        break;

    case ARG_ADDRESS:
    case ARG_NON_NULL_ADDRESS:
    case ARG_IOVEC:
    case ARG_SOCKADDR:
        sptr += sprintf(sptr, "0x%lx", reg);
        break;

    case ARG_MMAP:
        /* Although generic sanitise has set this to a map struct,
         * common_set_mmap_ptr_len() will subsequently set it to the ->ptr
         * in the per syscall ->sanitise routine. */
        sptr += sprintf(sptr, "%p", (void *)reg);
        break;

    case ARG_OP:
    case ARG_LIST:
        if(rec->flagCount == 1)
        {
            makeFlagToString(rec,argnum,0);
            sptr += sprintf(sptr, "%s", rec->flagBuffer);
            makeFlagToString(rec,argnum,2);
            sptr += sprintf(sptr, "%s", rec->flagBuffer);
        }
        else if(rec->flagCount == 2)
        {
            if(rec->flagBufferUsed == 0)
            {
                makeFlagToString(rec,argnum,0);
                rec->flagBufferUsed = 1;
                strcpy(rec->flagBuffer,"");
            }
            else if(rec->flagBufferUsed == 1)
            {
                makeFlagToString(rec,argnum,0);

            }
            sptr += sprintf(sptr, "%s", rec->flagBuffer);
        }
        //sptr += sprintf(sptr, "0x%lx", reg);
        break;

    case ARG_UNDEFINED:
    case ARG_LEN:
    case ARG_RANGE:
    case ARG_CPU:
    case ARG_IOVECLEN:
    case ARG_SOCKADDRLEN:
        if (((long)reg < -16384) || ((long)reg > 16384))
        {
            /* Print everything outside -16384 and 16384 as hex. */
            sptr += sprintf(sptr, "0x%lx", reg);
        }
        else
        {
            /* Print everything else as signed decimal. */
            sptr += sprintf(sptr, "%ld", (long)reg);
        }
        break;
    }

    return sptr;
}

char *render_arg(struct syscallInfo *rec, char *sptr, unsigned int argnum)
{
    const char *name = NULL;
    unsigned long reg = 0;
    enum argtype type = 0;

    switch (argnum)
    {
    case 1:
        type = rec->a1Type;
        name = parameterName[0];
        reg = rec->a1;
        break;
    case 2:
        type = rec->a2Type;
        name = parameterName[1];
        reg = rec->a2;

        break;
    case 3:
        type = rec->a3Type;
        name = parameterName[2];
        reg = rec->a3;
        break;
    case 4:
        type = rec->a4Type;
        name = parameterName[3];
        reg = rec->a4;
        break;
    case 5:
        type = rec->a5Type;
        name = parameterName[4];
        reg = rec->a5;
        break;
    case 6:
        type = rec->a6Type;
        name = parameterName[5];
        reg = rec->a6;
        break;
    }

    if (argnum != 1)
        sptr += sprintf(sptr, ", ");

    sptr += sprintf(sptr, "%s=", name);

    sptr = decode_argtype(rec, sptr, reg, type,argnum);

    return sptr;
}

void makeLogBuffer(struct syscallInfo *rec, int beforeTest)
{
    char logBuffer[10000] = {0};
    if (beforeTest)
    {
        char *sptr = logBuffer;
        sptr += sprintf(sptr, "%s(", syscallName);
        for (int i = 1; i <= parameterNameCount; ++i)
        {
            switch (i)
            {
            case 1:
                sptr = render_arg(rec, sptr, 1);
                // sptr = decode_argtype(sptr, rec->a1, rec->a1Type);
                break;
            case 2:
                sptr = render_arg(rec, sptr, 2);
                // sptr = decode_argtype(sptr, rec->a2, rec->a2Type);
                break;
            case 3:
                sptr = render_arg(rec, sptr, 3);
                // sptr = decode_argtype(sptr, rec->a3, rec->a3Type);
                break;
            case 4:
                sptr = render_arg(rec, sptr, 4);
                // sptr = decode_argtype(sptr, rec->a4, rec->a4Type);
                break;
            case 5:
                sptr = render_arg(rec, sptr, 5);
                // sptr = decode_argtype(sptr, rec->a5, rec->a5Type);
                break;
            case 6:
                sptr = render_arg(rec, sptr, 6);
                // sptr = decode_argtype(sptr, rec->a6, rec->a6Type);
                break;
            default:
#ifdef __DEBUG
                printf("parameter count is bigger than six!\n");
#endif
                break;
            }
        }
        sptr += sprintf(sptr, ") ");
    }
    else
    {
        char *sptr = logBuffer;
        sptr += sprintf(sptr, "%s",rec->logBuffer);
        if (rec->retval == -1)
        {
            sptr += sprintf(sptr, "= %ld (%s)",
                            (long)rec->retval, strerror(rec->errorNumber));
        }
        else
        {
            sptr += sprintf(sptr, "= ");
            if ((unsigned long)rec->retval > 10000)
                sptr += sprintf(sptr, "0x%lx", rec->retval);
            else
                sptr += sprintf(sptr, "%ld", (long)rec->retval);
        }
        sptr += sprintf(sptr, "\n");
    }

    strcpy(rec->logBuffer, logBuffer);
}

void resetArgumentRec(syscallInfo * rec)
{
    rec->a1 = 0;
    rec->a2 = 0;
    rec->a3 = 0;
    rec->a4 = 0;
    rec->a5 = 0;
    rec->a6 = 0;
  
}

void resetRecFlagBufferUsed(syscallInfo *rec)
{
    rec->flagBufferUsed = 0;
    rec->flagBufferUsed1 = 0;
    rec->flagBufferUsed2 = 0;
    rec->flagBufferUsed3 = 0;
}

void resetRecFlagBuffer(syscallInfo *rec)
{
    strcpy(rec->flagBuffer,"");
    strcpy(rec->flagBuffer1,"");
    strcpy(rec->flagBuffer2,"");
    strcpy(rec->flagBuffer3,"");
}

int doTest(syscallInfo *rec)
{
    int retval;
    int trace_marker_fd;
    int kata_fd;
    char cmd[10000] = {0};
    char trace_marker_start_message[10000] = {0};
    char trace_marker_end_message[10000] = {0};
    int trace_marker_start_message_len = 0;
    int trace_marker_end_message_len = 0;
    if (rec->kindOfContainer == 0 || rec->kindOfContainer == 1)
    {
        trace_marker_fd = open(TRACING_MARKER, O_WRONLY);
        if (trace_marker_fd == -1)
        {
            printf("cant not open trace marker fd\n");
            return -1;
        }
        memcpy(trace_marker_start_message, rec->logBuffer, strlen(rec->logBuffer) + 1);
        memcpy(trace_marker_end_message, rec->logBuffer, strlen(rec->logBuffer) + 1);
        strcat(trace_marker_start_message, "start");
        strcat(trace_marker_end_message, "end");
        trace_marker_start_message_len = strlen(trace_marker_start_message);
        trace_marker_end_message_len = strlen(trace_marker_end_message);
        write(trace_marker_fd, trace_marker_start_message, trace_marker_start_message_len);
    }
    else
    {
        kata_fd = open(KATA_FD_PATH, O_RDONLY);
        if (kata_fd == -1)
        {
            printf("cant not open kata fd\n");
            return -1;
        }
        system("sshpass -p 1234 ssh -t kataworker@192.168.122.109 \"echo 1 > /sys/kernel/debug/tracing/tracing_on\"");
        fsync(kata_fd);
        fsync(kata_fd);
        fsync(kata_fd);
    }
    retval = syscall(rec->nr, rec->a1, rec->a2, rec->a3, rec->a4, rec->a5, rec->a6);
    if (rec->kindOfContainer == 0 || rec->kindOfContainer == 1)
    {
        write(trace_marker_fd, trace_marker_end_message, trace_marker_end_message_len);
        close(trace_marker_fd);
    }
    else
    {
        fdatasync(kata_fd);
        fdatasync(kata_fd);
        fdatasync(kata_fd);
        sprintf(cmd, "sshpass -p 1234 ssh -t kataworker@192.168.122.109 \"echo 0 > /sys/kernel/debug/tracing/tracing_on; cat /sys/kernel/debug/tracing/trace | grep pool >> /home/kataworker/ftraceresult/%sTracing.txt; echo > /sys/kernel/debug/tracing/trace;\"", SYSCALL_NAME);
        system(cmd);
        close(kata_fd);
    }

    if (retval != -1)
    {
        rec->testSuccess = 1;
    }
    else
    {
        rec->testSuccess = 0;
    }
    rec->retval = retval;
    rec->errorNumber = errno;
}

void freeHeap()
{
    if (o_flags_set_size != 0)
    {
        free(o_flags_set);
    }
    if (o_flags_base_set_size != 0)
    {
        free(o_flags_base_set);
    }

    if (o_flags_set_size1 != 0)
    {
        free(o_flags_set1);
    }
    if (o_flags_base_set_size1 != 0)
    {
        free(o_flags_base_set1);
    }

    if (o_flags_set_size2 != 0)
    {
        free(o_flags_set2);
    }
    if (o_flags_base_set_size2 != 0)
    {
        free(o_flags_base_set2);
    }

    if (o_flags_set_size3 != 0)
    {
        free(o_flags_set3);
    }
    if (o_flags_base_set_size3 != 0)
    {
        free(o_flags_base_set3);
    }
}