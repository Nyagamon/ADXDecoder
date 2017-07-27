
//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "clADX.h"

//--------------------------------------------------
// 文字列を10進数とみなして数値に変換(簡易版)
//--------------------------------------------------
int atoi(const char *s){
	int r=0;
	bool sign=false;if(*s=='+'){s++;}else if(*s=='-'){sign=true;s++;}
	while(*s){
		if(*s>='0'&&*s<='9')r=r*10+(*s-'0');
		else break;
		s++;
	}
	return sign?-r:r;
}
float atof(const char *s){
	int r1=0,r2=0,c=1;
	bool sign=false;if(*s=='+'){s++;}else if(*s=='-'){sign=true;s++;}
	while(*s){
		if(*s>='0'&&*s<='9')r1=r1*10+(*s-'0');
		else break;
		s++;
	}
	if(*s=='.'){
		s++;
		while(*s){
			if(*s>='0'&&*s<='9'){r2=r2*10+(*s-'0');c*=10;}
			else break;
			s++;
		}
	}
	float r=r1+((c>0)?r2/(float)c:0);
	return sign?-r:r;
}

//--------------------------------------------------
// 文字列を16進数とみなして数値に変換
//--------------------------------------------------
long long int atoi16(const char *s){
	long long int r=0;
	bool sign=false;if(*s=='+'){s++;}else if(*s=='-'){sign=true;s++;}
	while(*s){
		if(*s>='0'&&*s<='9')r=(r<<4)|(*s-'0');
		else if(*s>='A'&&*s<='F')r=(r<<4)|(*s-'A'+10);
		else if(*s>='a'&&*s<='f')r=(r<<4)|(*s-'a'+10);
		else break;
		s++;
	}
	return sign?-r:r;
}

//--------------------------------------------------
// メイン
//--------------------------------------------------
int main(int argc,char *argv[]){

	// コマンドライン解析
	unsigned int count=0;
	char *filenameOut=nullptr;
	//bool decodeFlg=false;
	unsigned long long int ciphKeyEx=0;
	unsigned long long int ciphKey=0xCF222F1FE0748978;
	char *ciphKeyword=nullptr;
	int mode=16;
	int loop=0;
	bool info=false;
	bool decrypt=false;
	int analyzeKeyLevel=0;
	int toCiphKeyEx=0;
	for(int i=1;i<argc;i++){
		if(argv[i][0]=='-'||argv[i][0]=='/'){
			switch(argv[i][1]){
			case 'o':if(i+1<argc){filenameOut=argv[++i];}break;
			//case 'd':decodeFlg=true;break;
			case 'k':if(i+1<argc){ciphKeyword=argv[++i];}break;
			case 'a':if(i+1<argc){ciphKeyEx=atoi16(argv[++i]);}break;
			case 'b':if(i+1<argc){ciphKey=atoi16(argv[++i]);}break;
			case 'm':if(i+1<argc){mode=atoi(argv[++i]);}break;
			case 'l':if(i+1<argc){loop=atoi(argv[++i]);}break;
			case 'i':info=true;break;
			case 'c':decrypt=true;break;
			case 'x':if(i+1<argc){analyzeKeyLevel=atoi(argv[++i]);if(analyzeKeyLevel<=0)analyzeKeyLevel=1;}break;
			case 'y':if(i+1<argc){toCiphKeyEx=atoi(argv[++i]);if(toCiphKeyEx<=0)toCiphKeyEx=1;}break;
			}
		}else if(*argv[i]){
			argv[count++]=argv[i];
		}
	}

	//if(decodeFlg){

	// 入力チェック
	if(!count){
		printf("Error: 入力ファイルを指定してください。\n");
		return -1;
	}

	// デコード
	for(unsigned int i=0;i<count;i++){

		// 2つ目以降のファイルは、出力ファイル名オプションが無効
		if(i)filenameOut=nullptr;

		// デフォルト出力ファイル名
		char path[0x400];
		if(!(filenameOut&&filenameOut[0])){
			strcpy_s(path,sizeof(path),argv[i]);
			char *d1=strrchr(path,'\\');
			char *d2=strrchr(path,'/');
			char *e=strrchr(path,'.');
			if(e&&d1<e&&d2<e)*e='\0';
			if(analyzeKeyLevel>0)strcat_s(path,sizeof(path),".txt");
			else strcat_s(path,sizeof(path),".wav");
			filenameOut=path;
		}

		// ヘッダ情報のみ表示
		if(info){
			printf("%s のヘッダ情報\n",argv[i]);
			clADX adx(0,0,nullptr);
			adx.PrintInfo(argv[i]);
			printf("\n");
		}

		// 復号化
		else if(decrypt){
			printf("%s を復号化中...\n",argv[i]);
			clADX adx(ciphKeyEx,ciphKey,ciphKeyword);
			if(!adx.Decrypt(argv[i])){
				printf("Error: 復号化に失敗しました。\n");
			}
		}

		// 特殊鍵解析
		else if(analyzeKeyLevel>0){
			clADX adx(0,0,nullptr);
			if(!adx.AnalyzeKey(argv[i],filenameOut,analyzeKeyLevel)){
				printf("Error: 解析に失敗しました。\n");
			}
		}

		// 特殊鍵に変換
		else if(toCiphKeyEx>0){
			clADX adx(0,0,nullptr);
			switch(toCiphKeyEx){
			case 1:if(!adx.ToCiphKeyEx(ciphKeyword,&ciphKeyEx)){printf("Error: 変換に失敗しました。\n");}break;
			case 2:if(!adx.ToCiphKeyEx(ciphKey,&ciphKeyEx)){printf("Error: 変換に失敗しました。\n");}break;
			}
			printf("%012llX\n",ciphKeyEx);
		}

		// デコード
		else{
			printf("%s をデコード中...\n",argv[i]);
			clADX adx(ciphKeyEx,ciphKey,ciphKeyword);
			if(!adx.DecodeToWavefile(argv[i],filenameOut,mode,loop)){
				printf("Error: デコードに失敗しました。\n");
			}
		}

	}

	//}

	return 0;
}
