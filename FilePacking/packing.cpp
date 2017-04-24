#include "packing.h"

#define PACK_VERIFICATION 18

// 구현하면서 막혔던점

// fread에서 파일이 계속 안읽혔다.
// 알고보니 getFileSize 함수에서 함수 길이를 알던 도중 포인터를 끝으로 보내고
// rewind를 하지않았다.


int pack (char (* fileName)[128], size_t fileNum, char * packName) {
	int i;
	// 패킹 결과물 파일의 파일 포인터
	FILE * fPack;
	// 패킹할 파일들의 파일 포인터
	FILE * fFiles;
	// 버퍼
	char * buffer;
	// 패킹할 각 파일의 오프셋을 구하기 위해서 직전 파일의 오프셋을 설정해놓는다.
	size_t lastOffset = 0;
	

	// 패킹 파일 열기 (쓰기모드)
	fPack = fopen(packName, "wb");
	

	// 패킹 파일의 헤더 구조체 초기화
	PackingHead * packHead = (PackingHead *)malloc(sizeof(PackingHead));

	// 패킹 파일의 맨 처음 헤더 부분 설정 및 파일로 출력
	packHead->fileNum = fileNum;
	packHead->packType = PACK_VERIFICATION;
	fwrite(packHead, sizeof(PackingHead), 1, fPack);


	// 헤더 크기만큼 오프셋 증가
	lastOffset += sizeof(PackingHead);
	
	// 헤더 구조체 해제
	free(packHead);

	

	// 헤더의 그 다음 부분은 각 파일의 정보들이 담긴 구조체들의 연속
	// 하나씩 파일을 읽어들이면서 구조체 하나를 동적할당시키고 정보를 넣은 뒤 헤더 부분에 덧붙여 써간다
	for (i = 0; i < fileNum; i++) {

		// 각 파일의 정보들이 담긴 구조체 동적 할당
		PackingFiles * packFiles = (PackingFiles*)malloc(sizeof(PackingFiles));

		// 자료 파일 열기 (읽기모드)
		fFiles = fopen(fileName[i], "rb");

		if (fFiles == 0) {
			printf("%s 파일 읽기 실패\n", fileName[i]);
			return 0;
		}

		// 구조체 각 요소들 설정
		sprintf(packFiles->fileName, "%s", fileName[i]);
		packFiles->fileSize = getFileSize(fFiles);
		packFiles->fileOffset = lastOffset;

		// 다음 파일의 오프셋 설정을 위해
		// 지금 파일의 크기를 마지막 오프셋 값에서 더한다.
		lastOffset += packFiles->fileSize;

		// 패킹 파일에 현재 인덱스의 파일 정보를 출력한다.
		fwrite(packFiles, sizeof(PackingFiles), 1, fPack);

		// 구조체 동적 할당 해제
		free(packFiles);
		// 각 자료 파일 닫기 (읽기모드)
		fclose(fFiles);
	}


	// 이제 각 파일들을 하나하나씩 그대로 fPack 파일 포인터 뒤에 덧붙여 저장해나간다.

	for (i = 0; i < fileNum; i++) {

		// 자료 파일 열기 (읽기모드)
		fFiles = fopen(fileName[i], "rb");

		if (fFiles == 0) {
			printf("%s 파일 읽기 실패\n", fileName[i]);
			return 0;
		}

		// 각 파일의 크기에 맞게 메모리를 동적할당하고 파일 포인터로 불러들인 후에
		// fPack 포인터로 패킹 파일에 출력한다.

		// buffer 메모리 동적 할당 및 파일으로부터 데이터 입력
		buffer = (char *)malloc(getFileSize(fFiles));

		// 왜 안읽혀지지?
		fread(buffer, getFileSize(fFiles), 1, fFiles);


		// 패킹 파일로 출력
		fwrite(buffer, getFileSize(fFiles), 1, fPack);

		// 메모리 동적할당 해제
		free(buffer);
		// 각 자료 파일 닫기 (읽기모드)
		fclose(fFiles);
	}
	
	// 패킹 파일 닫기 (쓰기모드)
	fclose(fPack);
	return 1;
}

int unpack (char * fileName) {
	int i;
	// 팩 안의 파일 수
	int fileNum;
	// 패킹 파일의 파일 포인터
	FILE * fPack;
	// 해제될 파일들의 포인터
	FILE * fFiles;
	// 버퍼
	char * buffer;


	// 패킹 파일 열기 (읽기모드)
	fPack = fopen(fileName, "rb");

	if (fPack == 0) {
		puts("파일 읽기를 실패했습니다.");
		return 0;
	}

	// 패킹 파일의 헤더 구조체 초기화
	PackingHead * packHead = (PackingHead *)malloc(sizeof(PackingHead));
	// 헤더 정보를 읽어들임
	fread(packHead, sizeof(PackingHead), 1, fPack);

	// 헤더 정보와 미리 설정해놓은 PACK_VERIFICATION 값과 맞지 않을 시 종료
	if (packHead->packType != PACK_VERIFICATION) {
		puts("이 파일은 우리가 만든게 아니에요");
		return 0;
	}

	// 헤더 정보를 통해 팩 안의 파일 수 설정
	fileNum = packHead->fileNum;


	// 각 파일들의 정보가 담긴 헤더 구조체들을 파일 갯수만큼 동적할당
	PackingFiles * packFiles = (PackingFiles*)malloc(sizeof(PackingFiles)*fileNum);
	// 파일로부터 구조체 정보들을 읽어옴.
	fread(packFiles, sizeof(PackingFiles), fileNum, fPack);
	

	// 이제 원본 파일들을 읽으면서 하나하나씩 언패킹
	for (i = 0; i < fileNum; i++) {
		// 패킹 해제될 파일 열기 (쓰기모드)
		fFiles = fopen(packFiles[i].fileName, "wb");

		// 각 파일의 크기에 맞게 메모리를 동적할당하고 버퍼로 데이터들을 불러들인 후에
		// fFiles 포인터로 하나하나씩 언패킹한다.

		// packFiles 구조체에 있는 정보를 바탕으로 각 파일의 용량에 맞게 buffer 메모리 동적 할당
		buffer = (char *)malloc(packFiles[i].fileSize);
		fread(buffer, packFiles[i].fileSize, 1, fPack);

		// 버퍼로부터 파일 출력
		fwrite(buffer, packFiles[i].fileSize, 1, fFiles);

		// 메모리 동적할당 해제
		free(buffer);
		// 패킹 해제될 파일 닫기 (쓰기모드)
		fclose(fFiles);
	}

	// 패킹 파일 닫기 (읽기모드)
	fclose(fPack);
	return 1;
}

size_t getFileSize (FILE *fp) {
	int fileSize = 0;
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	rewind(fp);
	return fileSize;
}