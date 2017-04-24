#include <stdio.h>
#include <stdlib.h>

#define MAX_FILE 1000

#include "packing.h"


int main() {
	int i;

	int command = 0;
	size_t fileNum;
	char packFileNames[MAX_FILE][128];
	char fileName[128];
	

	puts("1. ��ŷ�ϱ�");
	puts("2. ��ŷǮ��");
	printf(": ");
	scanf("%d", &command);

	switch (command)
	{
	case 1:
		// ��ŷ�ϱ�
		printf("���� ���� : ");
		scanf("%d", &fileNum);


		for (i = 0; i < fileNum; i++) {
			printf("%d��°�� ��ŷ�� ���� �̸� : ", i+1);
			scanf("%s",packFileNames[i]);
		}

		printf("��ŷ ���� ������� �̸� : ");
		scanf("%s", fileName);


		// ��ŷ ����
		if (pack(packFileNames, fileNum, fileName)) {
			puts("��ŷ ����");
		}
		else {
			puts("��ŷ ����");
		}

		break;

	case 2:
		// ��ŷ Ǯ��
		printf("��ŷ ������ ���� �̸� : ");
		scanf("%s", fileName);

		// ����
		if (unpack(fileName)) {
			puts("��ŷ ���� ����");
		}
		else {
			puts("��ŷ ���� ����");
		}

		break;
	default:
		break;
	}

	return 0;
}