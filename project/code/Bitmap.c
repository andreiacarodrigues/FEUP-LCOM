#include "Bitmap.h"

#include "stdio.h"
#include "Graphics.h"


Bitmap* loadBitmap(const char* filename) {
	// allocating necessary size
	Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

	// open filename in read binary mode
	FILE *filePtr;
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// read the bitmap file header
	BitmapFileHeader bitmapFileHeader;
	fread(&bitmapFileHeader, 2, 1, filePtr);

	// verify that this is a bmp file by check bitmap id
	if (bitmapFileHeader.type != 0x4D42) {
		fclose(filePtr);
		return NULL;
	}

	int rd;
	do {
		if ((rd = fread(&bitmapFileHeader.size, 4, 1, filePtr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, filePtr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.offset, 4, 1, filePtr)) != 1)
			break;
	} while (0);

	if (rd = !1) {
		fprintf(stderr, "Error reading file\n");
		exit(-1);
	}

	// read the bitmap info header
	BitmapInfoHeader bitmapInfoHeader;
	fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

	// move file pointer to the begining of bitmap data
	fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

	// allocate enough memory for the bitmap image data
	unsigned char* bitmapImage = (unsigned char*) malloc(
			bitmapInfoHeader.imageSize);

	// verify memory allocation
	if (!bitmapImage) {
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	fread(bitmapImage, bitmapInfoHeader.imageSize, 1, filePtr);

	// make sure bitmap image data was read
	if (bitmapImage == NULL) {
		fclose(filePtr);
		return NULL;
	}

	// close file and return bitmap image data
	fclose(filePtr);

	bmp->bitmapData = bitmapImage;
	bmp->bitmapInfoHeader = bitmapInfoHeader;

	return bmp;
}

void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment) {
	if (bmp == NULL)
		return;

	int width = bmp->bitmapInfoHeader.width;
	int drawWidth = width;
	int height = bmp->bitmapInfoHeader.height;

	if (alignment == ALIGN_CENTER)
		x -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		x -= width;

	if (x + width < 0 || x > getHorResolution() || y + height < 0
			|| y > getVerResolution())
		return;

	int xCorrection = 0;
	if (x < 0) {
		xCorrection = -x;
		drawWidth -= xCorrection;
		x = 0;

		if (drawWidth > getHorResolution())
			drawWidth = getHorResolution();
	} else if (x + drawWidth >= getHorResolution()) {
		drawWidth = getHorResolution() - x;
	}

	char* bufferStartPos;
	char* imgStartPos;

	int i;
	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;

		if (pos < 0 || pos >= getVerResolution())
			continue;

		bufferStartPos = getBuffer();
		bufferStartPos += x * 2 + pos * getHorResolution() * 2;

		imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;

		memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
	}
}

void drawFilteredBitmap(Bitmap* bmp, int x, int y, int colorToFilter, Alignment alignment)
{
	if (bmp == NULL)
		return;

	char* bufferStartPos;
	unsigned int width = bmp->bitmapInfoHeader.width;
	unsigned int height = bmp->bitmapInfoHeader.height;

	if (alignment == ALIGN_CENTER)
		x -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		x -= width;

	int i, j;
	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;

		if (pos < 0 || pos >= getVerResolution())
			continue;

		bufferStartPos = getBuffer() + x * 2
				+ (y + height - i) * getHorResolution() * 2;

		for (j = 0; j < width * 2; j++, bufferStartPos++)
		{
			if (x + j < 0 || x * 2 + j >= getHorResolution() * 2)
				continue;

			int pos = j + i * width * 2;
			unsigned short tmp1 = bmp->bitmapData[pos];
			unsigned short tmp2 = bmp->bitmapData[pos + 1];
			unsigned short temp = (tmp1 | (tmp2 << 8));

			if (temp != colorToFilter)
			{
		 		*bufferStartPos = bmp->bitmapData[j + i * width * 2];
				j++;
				bufferStartPos++;
				*bufferStartPos = bmp->bitmapData[j + i * width * 2];
			}
			else
			{
				j++;
				bufferStartPos++;
			}
		}
	}
}

Bitmap* flipBitmap(Bitmap* bmp)
{
	if (bmp == NULL)
		return;

	int width = bmp->bitmapInfoHeader.width;
	int height = bmp->bitmapInfoHeader.height;

	unsigned int  i  = 0;
	unsigned char *first = bmp->bitmapData;
	unsigned char *last = bmp->bitmapData + width * 2;
	unsigned char aux;

	for(i; i < height; i++)
	{
		unsigned int  j  = 0;

		for(j; j < width; j++)
		{
			aux = *first;
			*first = *last;
			*last = aux;
			first++;
			last--;
		}

		first += width;
		last +=  width * 3;
	}
	return bmp;
}

void deleteBitmap(Bitmap* bmp) {
	if (bmp == NULL)
		return;

	free(bmp->bitmapData);
	free(bmp);
}

const char* path(const char* name)
{
	// Creates and writes the pathname to reader
	char reader[256];
	sprintf(reader, "/home/lcom/lcom1516-t6g15/project/images/%s.bmp", name);

	// Creates pathname and copies the reader into it
	char* pathname = (char*) malloc(256);
	strcpy(pathname, reader);

	// Returns the pathname
	return pathname;
}
