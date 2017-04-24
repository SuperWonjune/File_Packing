//packing.h

#ifndef _PACKING
#define _PACKING

#include <stdio.h>
#include <stdlib.h>

// 패킹 파일 자체의 정보들이 담긴 구조체
// 해당되는 패킹파일이 맞는지 확인하는 packType
// 포함된 파일의 총 갯수

typedef struct _packing_head {
	size_t packType;
	size_t fileNum;
} PackingHead;

// 패킹된 각 파일의 정보가 담겨있는 구조체
// 파일의 이름, 파일의 크기, 패킹된 파일 내의 해당 파일의 위치

typedef struct _packing_files {
	char fileName[128];
	size_t fileSize;
	size_t fileOffset;
} PackingFiles;




// pack, unpack 함수 각각 성공시 1 리턴, 실패시 0 리턴
int pack (char (* fileName)[128], size_t fileNum, char * packName);
int unpack (char * fileName);
size_t getFileSize (FILE *fp);

#endif