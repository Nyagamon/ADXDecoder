#pragma once

//--------------------------------------------------
// ADXクラス
//--------------------------------------------------
class clADX{
public:
	clADX(unsigned long long int ciphKeyEx=0,unsigned long long int ciphKey=0,const char *keyword=nullptr);
	~clADX();

	// ADXチェック
	static bool CheckFile(void *data,unsigned int size);

	// デコードしてWAVEファイルに保存
	bool DecodeToWavefile(const char *filenameADX,const char *filenameWAV,int mode=16,int loop=0);
	bool DecodeToWavefileStream(void *fpADX,const char *filenameWAV,int mode=16,int loop=0);

	// ヘッダ情報をコンソール出力
	bool PrintInfo(const char *filenameADX);

	// 復号化
	bool Decrypt(const char *filenameADX);

	// 特殊鍵解析
	bool AnalyzeKey(const char *filenameADX,const char *filenameTXT,int analyzeKeyLevel=1);

	// 特殊鍵に変換
	bool ToCiphKeyEx(const char *keyword,unsigned long long int *ciphKeyEx);
	bool ToCiphKeyEx(unsigned long long int ciphKey,unsigned long long int *ciphKeyEx);

private:
	__declspec(align(4)) struct stHeader{
		unsigned short adx;
		unsigned short copyrightOffset;
		unsigned char encodeType;
		unsigned char blockSize;
		unsigned char sampleBitdepth;
		unsigned char channelCount;
		unsigned int samplingRate;
		unsigned int totalSamples;
		unsigned short highpassFrequency;
		unsigned char version;
		unsigned char encryptionFlag;
	}_header;
	__declspec(align(4)) struct stInfo3{
		unsigned int unknown;
		unsigned int loopFlag;
		unsigned int loopStartSampleIndex;
		unsigned int loopStartByteIndex;
		unsigned int loopEndSampleIndex;
		unsigned int loopEndByteIndex;
	}_info3;
	__declspec(align(4)) struct stInfo4{
		unsigned int unknown[4];
		unsigned int loopFlag;
		unsigned int loopStartSampleIndex;
		unsigned int loopStartByteIndex;
		unsigned int loopEndSampleIndex;
		unsigned int loopEndByteIndex;
	}_info4;
	class clCipher{
	public:
		clCipher();
		bool Init(int type,unsigned short start,unsigned short mult,unsigned short add,const char *ketword=nullptr,unsigned long long int key=0);
		unsigned long long int GetKeyEx(void);
		unsigned int GetKey(void);
		void UpdateKey(void);
	private:
		unsigned int _type,_start,_mult,_add,_and;
		void Init0(void);
		void Init8(const char *ketword);
		void Init9(unsigned long long int key);
	}_cipher;
	unsigned short _start,_mult,_add;
	unsigned long long int _ciph_key;
	char *_ciph_keyword;
	double _coef1,_coef2;
	double _hist1[0x10],_hist2[0x10];
	double _wave[0x10][32];
	bool DecodeHeader(void *data,unsigned int size);
	bool DecodeBlock(void *data,unsigned int size,unsigned int channel);
	bool DecodeToWavefile_Decode(void *fp1,void *fp2,unsigned int address,unsigned int count,void *modeFunction);
	static void DecodeToWavefile_DecodeModeFloat(double f,void *fp);
	static void DecodeToWavefile_DecodeModeDouble(double f,void *fp);
	static void DecodeToWavefile_DecodeMode8bit(double f,void *fp);
	static void DecodeToWavefile_DecodeMode16bit(double f,void *fp);
	static void DecodeToWavefile_DecodeMode24bit(double f,void *fp);
	static void DecodeToWavefile_DecodeMode32bit(double f,void *fp);
};
