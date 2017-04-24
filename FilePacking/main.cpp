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
	

	puts("1. 패킹하기");
	puts("2. 패킹풀기");
	printf(": ");
	scanf("%d", &command);

	switch (command)
	{
	case 1:
		// 패킹하기
		printf("파일 갯수 : ");
		scanf("%d", &fileNum);


		for (i = 0; i < fileNum; i++) {
			printf("%d번째로 패킹할 파일 이름 : ", i+1);
			scanf("%s",packFileNames[i]);
		}

		printf("패킹 파일 결과물의 이름 : ");
		scanf("%s", fileName);


		// 패킹 실행
		if (pack(packFileNames, fileNum, fileName)) {
			puts("패킹 성공");
		}
		else {
			puts("패킹 실패");
		}

		break;

	case 2:
		// 패킹 풀기
		printf("패킹 해제할 파일 이름 : ");
		scanf("%s", fileName);

		// 실행
		if (unpack(fileName)) {
			puts("패킹 해제 성공");
		}
		else {
			puts("패킹 해제 실패");
		}

		break;
	default:
		break;
	}

	return 0;
}