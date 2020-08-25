#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include "lib/libbmp.h"

using namespace std;

typedef bitset<8> byte_t;
typedef byte_t char_t;

typedef bool bit_t;

struct Pixel {
	long int x;
	long int y;
};

enum COLORID {
	GREEN,
	BLUE,
	RED
};

bit_t getColorBit(BmpImg image, Pixel point, COLORID &colorId) {
	byte_t convertByte;
	switch (colorId)
	{
	case RED:
		convertByte = image.red_at(point.x, point.y);
		break;
	case GREEN:
		convertByte = image.green_at(point.x, point.y);
		break;
	case BLUE:
		convertByte = image.blue_at(point.x, point.y);
		break;
	}

	if (colorId == 2)
		colorId = static_cast<COLORID>(0);
	else
		colorId = static_cast<COLORID>(colorId + 1);

	return convertByte[0];
}

bit_t countNullByte(bit_t pipe, short &counter){
	if (pipe == 0)
		++counter;
	else
		counter = 0;

	if (counter == 8)
		throw exception();
	return pipe;
}

int main()
{
	BmpImg Image;
	if (Image.read("source.bmp") != BMP_OK)
		return EXIT_FAILURE;

	const long int imageHeight = Image.get_width();
	const long int imageWidth = Image.get_width();


	COLORID colorCounter = static_cast<COLORID>(0);

	short int nullByteCounter = 0;

	vector<bit_t> bitsVector;

	cout << "Wait until end. It may take long long time...\n";

	try {

		for (long int horizontalCounter = 0; horizontalCounter < imageWidth; horizontalCounter++)
		{
			for (long int verticalCounter = 0; verticalCounter < imageHeight; verticalCounter++) {
				Pixel currentPixel{
					horizontalCounter,
					verticalCounter
				};
				bitsVector.push_back(
					countNullByte(
						getColorBit(Image, currentPixel, colorCounter),
						nullByteCounter
					)
				);
				bitsVector.push_back(
					countNullByte(
						getColorBit(Image, currentPixel, colorCounter),
						nullByteCounter
					)
				);
				bitsVector.push_back(
					countNullByte(
						getColorBit(Image, currentPixel, colorCounter),
						nullByteCounter
					)
				);
			}
		}
	}
	catch (exception e) {}

	std::string decryptedText;

	const long int bitsCount = bitsVector.size();

	short int counter = 0;

	char_t nextChar;
	
	for (long unsigned int i = 0; i < bitsCount - 6; i++)
	{
		if (counter == 8) {
			counter = 0;
			decryptedText += (char)nextChar.to_ulong();
		}
		nextChar.set(7 - counter, bitsVector[i]);
		counter++;
	}
	decryptedText += (char)nextChar.to_ulong();

	cout << endl << decryptedText << endl << endl;

	std::system("pause");
}