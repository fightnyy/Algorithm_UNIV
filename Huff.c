
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION 1.0
#define MAX_SIZE 256
#define CHARBITS 8
#define MAX_CODE_LENGTH 16
#define MAX_CODE_NUMBER 256
#define MAX_STRING_LENGTH 1024*100 // maximum size of compressed file is 100 kbyte
#define MAX_BIT_MASK_NUMBER 8
#define RIGHTBITS(n, x) ((x) & ((1U << (n)) - 1U))

typedef struct code_tag
{
int ascii;
char code[MAX_CODE_LENGTH];
int lengthOfCode;
unsigned frequency;
} CODE;


int HuffmanCodeTable(CODE codeTable[]);

void SetCode(CODE codeTable[], int cnt);

int EncryptString(char original_string[], int length_Of_Original_string,

CODE codeTable[], int cnt_of_Codes, char encoded_String[]);

void WriteBitString(char encoded_String[], int cnt_Of_EncodedBits);

int DecryptBitString(char encoded_String[], int cnt_Of_EncodeBit,

CODE codeTable[], int cnt_Of_Codes, char decoded_String[]);

int SearchAlphabet(char key, CODE codeTable[], int cnt_Of_Codes);

void AttachCode(char encoded_String[], int cnt_Of_Bits, char code[], int lengthOfCode);

int SearchCode(char key[], CODE codeTable[], int cnt_Of_Codes);

void WriteInFile(char encoded_String[], int cnt_Of_EncodedBits, CODE codeTable[], int cnt_of_codes);

void PrnFreq(CODE codeTable[], int cnt_of_codes);

void Error(char *msg);

void DownHeap(int i);

void GetTableSizeAndCodeBits(int *size_of_table, int *cnt_Of_EncodeBit);

int GetVersion();

void GetCodeTable(CODE codeTable[], int cnt_of_codes);

void GetCode(char encoded_string[]);

void PrnCodeTable(CODE codeTable[], int cnt_of_codes);

void WriteDecodedBit(char decoded_String[]);


char bitMask_OR[MAX_BIT_MASK_NUMBER];

char bitMask_AND[MAX_BIT_MASK_NUMBER];

int heap_size, heap[2*MAX_SIZE-1], parent[2*MAX_SIZE-1], left[2*MAX_SIZE-1], right[2*MAX_SIZE-1];

unsigned long int frequency[2*MAX_SIZE-1];

FILE *ifp, *ofp;


int main(int argc, char *argv[])

{

int i, j = 0;

char Original_String[MAX_STRING_LENGTH];

char encoded_String[MAX_STRING_LENGTH];

char decoded_String[MAX_STRING_LENGTH];

int lengthOfDecodedString;

int cnt_Of_EncodedBits = 0;

CODE codeTable[MAX_CODE_NUMBER];

int cnt_Of_Codes = 0;

char option;


if(argc != 4 || ((option = *argv[1]) != 'e' && option != 'E'

&& option != 'd' && option != 'D'))

{

printf("Usage : %s [e or E|d or D] input_file output_file\n", argv[0]);

exit(1);

}


ifp = fopen(argv[2], "rb");

if(ifp == NULL)

{

printf("Cannot open file : %s", argv[2]);

exit(1);

}


ofp = fopen(argv[3], "wb");

if(ofp == NULL)

{

printf("Cannot open file : %s", argv[3]);

exit(1);

}


for(i=0; i < 8; i++)

{

bitMask_OR[i] = 1 << (7-i);

bitMask_AND[i] = bitMask_OR[i] ^ 0xff;

}


if(option == 'e' || option == 'E')

{

while((i = getc(ifp)) != EOF)

{

Original_String[j++] = i;

if(j >= MAX_STRING_LENGTH)

Error("Error : larger than 100kbyte!\n");

}

Original_String[j]='\0';

rewind(ifp);

cnt_Of_Codes = HuffmanCodeTable(codeTable);

cnt_Of_EncodedBits = EncryptString(Original_String, strlen(Original_String), codeTable, cnt_Of_Codes, encoded_String);

WriteInFile(encoded_String, cnt_Of_EncodedBits, codeTable, cnt_Of_Codes);


}

else

{

if(GetVersion())

Error("Error : The file isn't equivalent the current program version!\n");
GetTableSizeAndCodeBits(&cnt_Of_Codes, &cnt_Of_EncodedBits);
GetCodeTable(codeTable, cnt_Of_Codes);
GetCode(encoded_String);
lengthOfDecodedString = DecryptBitString(encoded_String, cnt_Of_EncodedBits, codeTable, cnt_Of_Codes, decoded_String);
WriteDecodedBit(decoded_String);
}
fclose(ifp);
fclose(ofp);
return 0;
}



int GetVersion()

{
int i, j = 0;
char buffer[10];
while((i = getc(ifp)) != EOF && j < 3)
{
buffer[j++] = i;
if(j >= MAX_STRING_LENGTH)
Error("Error : larger than 100kbyte!\n");
}
if(atof(buffer) == VERSION)
return 0;
else
return -1;
}


/*************************************************************

* 호프만 테이블 사이즈와 비트의 수 얻기

*

*************************************************************/


void GetTableSizeAndCodeBits(int *size_of_table, int *cnt_Of_EncodeBit)

{

int ch, i = 0;

char buffer[10];


while((ch = getc(ifp)) != EOF && ch != 0x2F)

buffer[i++] = ch;

buffer[i] = '\0';

*size_of_table = atoi(buffer);

i = 0;

while((ch = getc(ifp)) != EOF && ch != 0x20)

buffer[i++] = ch;

*cnt_Of_EncodeBit = atoi(buffer);

return;

}


/*************************************************************

* 호프만 코드 테이블을 메모리에 저장

*

*************************************************************/


void GetCodeTable(CODE codeTable[], int cnt_of_codes)

{

int i = 0, j = 0, tmp, is_block = 0;


while((tmp = getc(ifp)) != EOF || i < cnt_of_codes)

{

if(!is_block && tmp != '0' && tmp != '1')

{

codeTable[i].ascii = tmp;

}

else if(tmp == '0' || tmp == '1')

{

is_block = 1;

codeTable[i].code[j++] = tmp;

continue;

}

else if(is_block && tmp != '0' && tmp != '1')

{

codeTable[i].code[j] = '\0';

is_block = 0, j = 0;

if(cnt_of_codes == ++i)

break;

codeTable[i].ascii = tmp;

}

}

PrnCodeTable(codeTable, cnt_of_codes);

}


/*************************************************************

* 호프만 코드로 작성된 내용을 버퍼에 저장

*

*************************************************************/


void GetCode(char encoded_string[])

{

int ch, i = 0;


while((ch = getc(ifp)) != EOF)

{

encoded_string[i++] = ch;

if(i >= MAX_STRING_LENGTH)

Error("Error : larger than 100kbyte!\n");

}

encoded_string[i] = '\0';


}


void Error(char *msg)

{

printf("%s", msg);

exit(1);

}


void PrnFreq(CODE codeTable[], int cnt_of_codes)

{

int i, k = 0, size = 0;


printf("텍스트 분석 중...\n");

printf("---------------------------------\n");

printf("alphabet\tfrequency\n");

printf("---------------------------------\n");

for(i = 0; i <= cnt_of_codes; i++)

{

if((codeTable[i].ascii >= 65 && codeTable[i].ascii <= 90)

|| (codeTable[i].ascii >= 97 && codeTable[i].ascii <= 122))

{

printf("%c\t\t%d\n", codeTable[i].ascii, codeTable[i].frequency);

size += codeTable[i].frequency;

}

}

printf("---------------------------------\n");

printf("텍스트의 크기 : %d byte\n", size);

printf("---------------------------------\n\n\n");

}


/*************************************************************

* 버퍼에 저장된 호프만 코드를 사용 빈도수에 맞게 버퍼에 저장

*

*************************************************************/


void DownHeap(int i)

{

int j, k;

k = heap[i];


while ((j = 2 * i) <= heap_size)

{

if (j < heap_size && frequency[heap[j]] > frequency[heap[j + 1]])

j++;

if (frequency[k] <= frequency[heap[j]])

break;

heap[i] = heap[j];

i = j;

}

heap[i] = k;

}


/*************************************************************

* 호프만 알고리즘을 이용한 코드 테이블 작성

*

*************************************************************/


int HuffmanCodeTable(CODE codeTable[])

{

int i, j, k, avail, cnt_of_codes;

unsigned long int incount;


for(i = 0; i < MAX_SIZE; i++)

frequency[i] = 0;


while ((i = getc(ifp)) != EOF)

frequency[i]++;


heap[i] = 0;

heap_size = 0;


for(i = 0; i < MAX_SIZE; i++)

{

if(frequency[i] != 0)

heap[++heap_size] = i;

}


for(i = 1; i <= heap_size; i++)

{

codeTable[i-1].ascii = heap[i];

codeTable[i-1].frequency = frequency[heap[i]];

}


cnt_of_codes = heap_size;

PrnFreq(codeTable, cnt_of_codes);


for(i = heap_size / 2; i >= 1; i--)


DownHeap(i);

for(i = 0; i < 2 * MAX_SIZE - 1; i++)

parent[i] = 0;

k = heap[1];

avail = MAX_SIZE;

while (heap_size > 1)

{

i = heap[1];

heap[1] = heap[heap_size--];

DownHeap(1);

j = heap[1];

k = avail++;

frequency[k] = frequency[i] + frequency[j];

heap[1] = k;

DownHeap(1);

parent[i] = k;

parent[j] = -k;

left[k] = i;

right[k] = j;

}


incount = 0;

rewind(ifp);

SetCode(codeTable, cnt_of_codes);

return cnt_of_codes;

}


/*************************************************************

* 아스키 코드에 대한 코드 작성

*

*************************************************************/


void SetCode(CODE codeTable[], int cnt_of_codes)

{

int i, j, k, val = 0;

char temp[MAX_SIZE];


for(i = 0, j = 0, k = 0; i < cnt_of_codes; i++, j = 0, k = 0)

{

val = codeTable[i].ascii;

while ((val = parent[val]) != 0)

{

if (val > 0)

temp[j++] = '0';

else

{

temp[j++] = '1';

val = -val;

}

}


while(--j >= 0)

{

codeTable[i].code[k] = temp[j];

k++;

}

codeTable[i].code[k] = '\0';

codeTable[i].lengthOfCode = strlen(codeTable[i].code);

}

PrnCodeTable(codeTable, cnt_of_codes);

}


void PrnCodeTable(CODE codeTable[], int cnt_of_codes)

{

int i;

printf("Huffman code 생성 중...\n");

printf("---------------------------------\n");

printf("alphabet\tcode\n");

printf("---------------------------------\n");

for(i = 0; i < cnt_of_codes; i++)

if((codeTable[i].ascii >= 65 && codeTable[i].ascii <= 90) || (codeTable[i].ascii >= 97 && codeTable[i].ascii <= 122))

printf("%c\t\t%s\n", codeTable[i].ascii, codeTable[i].code);

printf("---------------------------------\n");

}


/*************************************************************

* 호프만 코드로 문자열 압축

*

*************************************************************/

int EncryptString(char original_String[], int length_Of_Original_String,

CODE codeTable[], int cnt_Of_Codes, char encoded_String[])

{

int i, pos, cnt_Of_Bits = 0;

for(i = 0; i < length_Of_Original_String; i++)

{

pos = SearchAlphabet(original_String[i], codeTable, cnt_Of_Codes);

if(pos >= 0)

{

AttachCode(encoded_String, cnt_Of_Bits, codeTable[pos].code, codeTable[pos].lengthOfCode);

cnt_Of_Bits += codeTable[pos].lengthOfCode;

}

else

{

printf("%c does not exist in code table!!\n", original_String[i]);

return -1;

}

}

return cnt_Of_Bits;


}


void WriteBitString(char encoded_String[], int cnt_Of_EncodedBits)

{

int i, pos = 0;

char a;

for(i=0; i < cnt_Of_EncodedBits; i++)

{

if(i%8==0)

{

putchar(' ');

a = encoded_String[pos++];

}

putchar(((a & bitMask_OR[0]) == 0) ? '0' : '1');

a <<= 1;

}

putchar('\n');

}



void WriteDecodedBit(char decoded_String[])

{

int i, j, length, val, tmp = 0, diff;

double progress = 0.0;


printf("압축 푸는 중 ...\n");

length = (int)strlen(decoded_String);

for(i =0; i < length; i++)

{

progress = (double)(i+1) / (double)length * 100.0;

if((int)progress%10 == 0 && (int)progress != 0 && (int)progress != 100)

{

if((val = (int)progress) != tmp)

{

if((diff = (val - tmp) / 10) > 1)

for(j = 1; j < diff; j++)

printf("%d %% 압축 품\n", tmp + j * 10);

printf("%d %% 압축 품\n", val);

tmp = val;

}

}

else if((int)progress/10 && (int)progress%10 > 5)

{

if((val = ((int)progress/10)*10) != tmp)

{

if((diff = (val - tmp) / 10) > 1)

for(j = 1; j < diff; j++)

printf("%d %% 압축 품\n", tmp + j * 10);

printf("%d %% 압축 품\n", val);

tmp = val;

}

}

else if((val = ((int)progress/10)*10) != tmp && (int)progress != 100)

{

if((diff = (val - tmp) / 10) > 1)

{

for(j = 1; j < diff; j++)

printf("%d %% 압축 품\n", tmp + j * 10);

}

printf("%d %% 압축 품\n", val);

tmp = val;

}

putc(decoded_String[i], ofp);

}

printf("완료.\n");

}


void WriteInFile(char encoded_String[], int cnt_Of_EncodedBits, CODE codeTable[],

int cnt_of_codes)

{

int i, j, ch = 0, incount, outcount, pos = 0, val, tmp = 0, diff;

double version = VERSION, progress = 0.0;

char *p, buffer[MAX_STRING_LENGTH];

printf("압축 중 ...\n");

fseek(ifp, 0L, SEEK_END);

incount = ftell(ifp);


// write a version

sprintf(buffer, "%.1f ", version);

for(i =0; i < (int)strlen(buffer); i++)

putc(buffer[i], ofp);


// write a size of the code tables

sprintf(buffer, "%d", cnt_of_codes);

for(i =0; i < (int)strlen(buffer); i++)

putc(buffer[i], ofp);

sprintf(buffer, "/%d ", cnt_Of_EncodedBits);

for(i =0; i < (int)strlen(buffer); i++)

putc(buffer[i], ofp);


// write contents of the code tables

for(i = 0; i < cnt_of_codes; i++)

{

putc(codeTable[i].ascii, ofp);

for(j = 0; j < codeTable[i].lengthOfCode; j++)

putc(codeTable[i].code[j], ofp);

}

putc(' ', ofp);


// write contents of compressed file

p = encoded_String;

for(i=0; (i < cnt_Of_EncodedBits) && (ch != EOF) && (*p != '\0'); i++)

{

progress = (double)i / (double)cnt_Of_EncodedBits * 100.0;

if((int)progress%10 == 0 && (int)progress != 0 && (int)progress != 100)

{

if((val = (int)progress) != tmp)

{

if((diff = (val - tmp) / 10) > 1)

{

for(j = 1; j < diff; j++)

printf("%d %% 압축\n", tmp + j * 10);

}

printf("%d %% 압축\n", val);

tmp = val;

}

}

else if((int)progress/10 && (int)progress%10 > 5)

{

if((val = ((int)progress/10)*10) != tmp)

{

if((diff = (val - tmp) / 10) > 1)

{

for(j = 1; j < diff; j++)

printf("%d %% 압축\n", tmp + j * 10);

}

printf("%d %% 압축\n", val);

tmp = val;

}

}

else if((val = ((int)progress/10)*10) != tmp && (int)progress != 100)

{

if((diff = (val - tmp) / 10) > 1)

{

for(j = 1; j < diff; j++)

printf("%d %% 압축\n", tmp + j * 10);

}

printf("%d %% 압축\n", val);

tmp = val;

}

if(i%8==0)

ch = putc( p[pos++], ofp );

}

printf("압축 완료.\n");

outcount = ftell(ofp);

printf("---------------------------------\n");

printf("텍스트 파일 크기 : %u bytes\n", incount);

printf("압축된 파일 크기 : %u bytes\n", outcount);

printf("압축 비율 : %.3f(%u/%u - 1)\n", ((double)incount / (double)outcount - 1.0)

, incount, outcount);

}


/*************************************************************

* 호프만 코드로 문자열 압축 풀기

*

*************************************************************/


int DecryptBitString(char encoded_String[], int cnt_Of_EncodeBit,

CODE codeTable[], int cnt_Of_Codes, char decoded_String[])

{

int i, index, pos=0, length_Of_DecodedString=0;

char code[MAX_CODE_LENGTH] = {'\0'};

char a;

for (i = 0; i < cnt_Of_EncodeBit; i++)

{

if(i%8 ==0)

a=encoded_String[pos++];


if(a & bitMask_OR[0])

strcat(code, "1");

else

strcat(code, "0");

a <<= 1;

index=SearchCode(code, codeTable, cnt_Of_Codes);

if(index != -1)

{

decoded_String[length_Of_DecodedString] = codeTable[index].ascii;

length_Of_DecodedString++;

code[0] = '\0';

}

}

decoded_String[length_Of_DecodedString] = '\0';

return length_Of_DecodedString;

}


/*************************************************************

* 호프만 테이블 내의 주어진 알파벳 문자의 인덱스 찾기

*

*************************************************************/


int SearchAlphabet(char key, CODE codeTable[], int cnt_Of_Codes)

{

int i, index = -1;

for (i = 0; i <cnt_Of_Codes; i++)

{

if(codeTable[i].ascii == key)

{

index=i;

break;

}

}

return index;

}


/*************************************************************

* 호프만 테이블 내의 주어진 코드의 인덱스 찾기

*

*************************************************************/


int SearchCode(char key[], CODE codeTable[], int cnt_Of_Codes)

{

int i, index=-1;

for (i = 0; i < cnt_Of_Codes; i++)

{

if(strcmp(codeTable[i].code, key) == 0)

{

index=i;

break;

}

}

return index;

}


/*************************************************************

*하나의 버퍼에 호프만 코드 저장

*

*************************************************************/


void AttachCode(char encoded_String[], int cnt_Of_Bits, char code[],

int lengthOfCode)

{

int i, lengthOfEncodedString, lastBitPosition;


lengthOfEncodedString = cnt_Of_Bits/8;

lastBitPosition = cnt_Of_Bits%8;

for (i = 0; i < lengthOfCode; i++)

{

if(code[i]=='1')

encoded_String[lengthOfEncodedString] |= bitMask_OR[lastBitPosition];

else

encoded_String[lengthOfEncodedString] &= bitMask_AND[lastBitPosition];

lastBitPosition++;


if(lastBitPosition == 8)

{

lastBitPosition = 0;

lengthOfEncodedString = lengthOfEncodedString + 1;

}

}

}


