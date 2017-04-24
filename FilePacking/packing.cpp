#include "packing.h"

#define PACK_VERIFICATION 18

// �����ϸ鼭 ��������

// fread���� ������ ��� ��������.
// �˰��� getFileSize �Լ����� �Լ� ���̸� �˴� ���� �����͸� ������ ������
// rewind�� �����ʾҴ�.


int pack (char (* fileName)[128], size_t fileNum, char * packName) {
	int i;
	// ��ŷ ����� ������ ���� ������
	FILE * fPack;
	// ��ŷ�� ���ϵ��� ���� ������
	FILE * fFiles;
	// ����
	char * buffer;
	// ��ŷ�� �� ������ �������� ���ϱ� ���ؼ� ���� ������ �������� �����س��´�.
	size_t lastOffset = 0;
	

	// ��ŷ ���� ���� (������)
	fPack = fopen(packName, "wb");
	

	// ��ŷ ������ ��� ����ü �ʱ�ȭ
	PackingHead * packHead = (PackingHead *)malloc(sizeof(PackingHead));

	// ��ŷ ������ �� ó�� ��� �κ� ���� �� ���Ϸ� ���
	packHead->fileNum = fileNum;
	packHead->packType = PACK_VERIFICATION;
	fwrite(packHead, sizeof(PackingHead), 1, fPack);


	// ��� ũ�⸸ŭ ������ ����
	lastOffset += sizeof(PackingHead);
	
	// ��� ����ü ����
	free(packHead);

	

	// ����� �� ���� �κ��� �� ������ �������� ��� ����ü���� ����
	// �ϳ��� ������ �о���̸鼭 ����ü �ϳ��� �����Ҵ��Ű�� ������ ���� �� ��� �κп� ���ٿ� �ᰣ��
	for (i = 0; i < fileNum; i++) {

		// �� ������ �������� ��� ����ü ���� �Ҵ�
		PackingFiles * packFiles = (PackingFiles*)malloc(sizeof(PackingFiles));

		// �ڷ� ���� ���� (�б���)
		fFiles = fopen(fileName[i], "rb");

		if (fFiles == 0) {
			printf("%s ���� �б� ����\n", fileName[i]);
			return 0;
		}

		// ����ü �� ��ҵ� ����
		sprintf(packFiles->fileName, "%s", fileName[i]);
		packFiles->fileSize = getFileSize(fFiles);
		packFiles->fileOffset = lastOffset;

		// ���� ������ ������ ������ ����
		// ���� ������ ũ�⸦ ������ ������ ������ ���Ѵ�.
		lastOffset += packFiles->fileSize;

		// ��ŷ ���Ͽ� ���� �ε����� ���� ������ ����Ѵ�.
		fwrite(packFiles, sizeof(PackingFiles), 1, fPack);

		// ����ü ���� �Ҵ� ����
		free(packFiles);
		// �� �ڷ� ���� �ݱ� (�б���)
		fclose(fFiles);
	}


	// ���� �� ���ϵ��� �ϳ��ϳ��� �״�� fPack ���� ������ �ڿ� ���ٿ� �����س�����.

	for (i = 0; i < fileNum; i++) {

		// �ڷ� ���� ���� (�б���)
		fFiles = fopen(fileName[i], "rb");

		if (fFiles == 0) {
			printf("%s ���� �б� ����\n", fileName[i]);
			return 0;
		}

		// �� ������ ũ�⿡ �°� �޸𸮸� �����Ҵ��ϰ� ���� �����ͷ� �ҷ����� �Ŀ�
		// fPack �����ͷ� ��ŷ ���Ͽ� ����Ѵ�.

		// buffer �޸� ���� �Ҵ� �� �������κ��� ������ �Է�
		buffer = (char *)malloc(getFileSize(fFiles));

		// �� ����������?
		fread(buffer, getFileSize(fFiles), 1, fFiles);


		// ��ŷ ���Ϸ� ���
		fwrite(buffer, getFileSize(fFiles), 1, fPack);

		// �޸� �����Ҵ� ����
		free(buffer);
		// �� �ڷ� ���� �ݱ� (�б���)
		fclose(fFiles);
	}
	
	// ��ŷ ���� �ݱ� (������)
	fclose(fPack);
	return 1;
}

int unpack (char * fileName) {
	int i;
	// �� ���� ���� ��
	int fileNum;
	// ��ŷ ������ ���� ������
	FILE * fPack;
	// ������ ���ϵ��� ������
	FILE * fFiles;
	// ����
	char * buffer;


	// ��ŷ ���� ���� (�б���)
	fPack = fopen(fileName, "rb");

	if (fPack == 0) {
		puts("���� �б⸦ �����߽��ϴ�.");
		return 0;
	}

	// ��ŷ ������ ��� ����ü �ʱ�ȭ
	PackingHead * packHead = (PackingHead *)malloc(sizeof(PackingHead));
	// ��� ������ �о����
	fread(packHead, sizeof(PackingHead), 1, fPack);

	// ��� ������ �̸� �����س��� PACK_VERIFICATION ���� ���� ���� �� ����
	if (packHead->packType != PACK_VERIFICATION) {
		puts("�� ������ �츮�� ����� �ƴϿ���");
		return 0;
	}

	// ��� ������ ���� �� ���� ���� �� ����
	fileNum = packHead->fileNum;


	// �� ���ϵ��� ������ ��� ��� ����ü���� ���� ������ŭ �����Ҵ�
	PackingFiles * packFiles = (PackingFiles*)malloc(sizeof(PackingFiles)*fileNum);
	// ���Ϸκ��� ����ü �������� �о��.
	fread(packFiles, sizeof(PackingFiles), fileNum, fPack);
	

	// ���� ���� ���ϵ��� �����鼭 �ϳ��ϳ��� ����ŷ
	for (i = 0; i < fileNum; i++) {
		// ��ŷ ������ ���� ���� (������)
		fFiles = fopen(packFiles[i].fileName, "wb");

		// �� ������ ũ�⿡ �°� �޸𸮸� �����Ҵ��ϰ� ���۷� �����͵��� �ҷ����� �Ŀ�
		// fFiles �����ͷ� �ϳ��ϳ��� ����ŷ�Ѵ�.

		// packFiles ����ü�� �ִ� ������ �������� �� ������ �뷮�� �°� buffer �޸� ���� �Ҵ�
		buffer = (char *)malloc(packFiles[i].fileSize);
		fread(buffer, packFiles[i].fileSize, 1, fPack);

		// ���۷κ��� ���� ���
		fwrite(buffer, packFiles[i].fileSize, 1, fFiles);

		// �޸� �����Ҵ� ����
		free(buffer);
		// ��ŷ ������ ���� �ݱ� (������)
		fclose(fFiles);
	}

	// ��ŷ ���� �ݱ� (�б���)
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