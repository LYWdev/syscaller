#define _GNU_SOURCE
#define __DEBUG
#include <stdio.h>
#include <syscall.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

enum argtype
{
    ARG_UNDEFINED,
    ARG_FD,
    ARG_LEN,
    ARG_ADDRESS,
    ARG_MODE_T,
    ARG_NON_NULL_ADDRESS,
    ARG_PID,
    ARG_RANGE,
    ARG_OP,
    ARG_LIST,
    ARG_CPU,
    ARG_PATHNAME,
    ARG_IOVEC,
    ARG_IOVECLEN,
    ARG_SOCKADDR,
    ARG_SOCKADDRLEN,
    ARG_MMAP,
    ARG_SOCKETINFO,
};

typedef struct syscallInfo
{
    unsigned int nr;
    unsigned long a1;
    unsigned long a2;
    unsigned long a3;
    unsigned long a4;
    unsigned long a5;
    unsigned long a6;

    enum argtype a1Type;
    enum argtype a2Type;
    enum argtype a3Type;
    enum argtype a4Type;
    enum argtype a5Type;
    enum argtype a6Type;
    unsigned long retval;


    int testSuccess;
    int errorNumber;
    int kindOfContainer; // 0: 도커, 1: gVisor, 2: kata


    char logBuffer[10000];

    int flagCount;
    int flagCount1;
    int flagCount2;
    int flagCount3;

    //parameter로 flag가 두개있을 때 사용하는 변수
    int flagBufferUsed; 
    int flagBufferUsed1;
    int flagBufferUsed2;
    int flagBufferUsed3;

    //parameter로 flag가 한개이지만 flag자체는 두종류를 가질 때 사용하는 변수
    int prevFlagBufferUsed;
    int prevFlagBufferUsed1;
    int prevFlagBufferUsed2;
    int prevFlagBufferUsed3;


    char flagBuffer[10000];
    char flagBuffer1[10000];
    char flagBuffer2[10000];
    char flagBuffer3[10000];

    int failCount;

} syscallInfo;

int flagBaseSize;
int o_flags_base_index;
unsigned long *o_flags_base_set;
unsigned int o_flags_base_set_size;

int flagSize;
int o_flags_index;
unsigned long *o_flags_set;
unsigned int o_flags_set_size;
int o_flags_base_set_index;
int o_flags_set_index;

int flagBaseSize1;
int o_flags_base_index1;
unsigned long *o_flags_base_set1;
unsigned int o_flags_base_set_size1;

int flagSize1;
int o_flags_index1;
unsigned long *o_flags_set1;
unsigned int o_flags_set_size1;
int o_flags_base_set_index1;
int o_flags_set_index1;

int flagBaseSize2;
int o_flags_base_index2;
unsigned long *o_flags_base_set2;
unsigned int o_flags_base_set_size2;

int flagSize2;
int o_flags_index2;
unsigned long *o_flags_set2;
unsigned int o_flags_set_size2;
int o_flags_base_set_index2;
int o_flags_set_index2;

int flagBaseSize3;
int o_flags_base_index3;
unsigned long *o_flags_base_set3;
unsigned int o_flags_base_set_size3;

int flagSize3;
int o_flags_index3;
unsigned long *o_flags_set3;
unsigned int o_flags_set_size3;
int o_flags_base_set_index3;
int o_flags_set_index3;

int doTest(syscallInfo *rec);
void freeHeap();
void signal_handler(int signo);
void signal_alarm(int time);         //블로킹 되면 깨워줘야할 때 사용
void makePowerSet(unsigned long *flag_arr, unsigned long *flag_power_arr, int size);
void initTest(syscallInfo *rec);
void tracingOn(int num);
void trace_file_make(struct syscallInfo *rec);
unsigned long resetFlagIndex(int kindOfFlag);
unsigned long resetFlagIndex1(int kindOfFlag);
unsigned long resetFlagIndex2(int kindOfFlag);
unsigned long resetFlagIndex3(int kindOfFlag);
unsigned long getFlag(struct syscallInfo *rec, int argmentNumber, int kindOfFlag, int next);
unsigned long getFlag1(struct syscallInfo *rec, int argmentNumber, int kindOfFlag, int next);
unsigned long getFlag2(struct syscallInfo *rec, int argmentNumber, int kindOfFlag, int next);
unsigned long getFlag3(struct syscallInfo *rec, int argmentNumber, int kindOfFlag, int next);
unsigned long makeFlagToString(struct syscallInfo *rec, int argmentNumber, int kindOfFlag);
unsigned long makeFlagToString1(struct syscallInfo *rec, int argmentNumber, int kindOfFlag);
unsigned long makeFlagToString2(struct syscallInfo *rec, int argmentNumber, int kindOfFlag);
unsigned long makeFlagToString3(struct syscallInfo *rec, int argmentNumber, int kindOfFlag);
char *decode_argtype(struct syscallInfo *rec,char *sptr, unsigned long reg, enum argtype type, unsigned int argnum);
char *render_arg(struct syscallInfo *rec, char *sptr, unsigned int argnum);
void makeLogBuffer(struct syscallInfo *rec, int beforeTest);
void resetArgumentRec(syscallInfo * rec);
void resetRecFlagBufferUsed(syscallInfo *rec);
void resetRecFlagBuffer(syscallInfo *rec);