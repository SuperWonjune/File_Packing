//packing.h

#ifndef _PACKING
#define _PACKING

#include <stdio.h>
#include <stdlib.h>

// ��ŷ ���� ��ü�� �������� ��� ����ü
// �ش�Ǵ� ��ŷ������ �´��� Ȯ���ϴ� packType
// ���Ե� ������ �� ����

typedef struct _packing_head {
	size_t packType;
	size_t fileNum;
} PackingHead;

// ��ŷ�� �� ������ ������ ����ִ� ����ü
// ������ �̸�, ������ ũ��, ��ŷ�� ���� ���� �ش� ������ ��ġ

typedef struct _packing_files {
	char fileName[128];
	size_t fileSize;
	size_t fileOffset;
} PackingFiles;




// pack, unpack �Լ� ���� ������ 1 ����, ���н� 0 ����
int pack (char (* fileName)[128], size_t fileNum, char * packName);
int unpack (char * fileName);
size_t getFileSize (FILE *fp);

#endif