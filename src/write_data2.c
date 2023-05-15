#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

u_int16_t read16(FILE *f){
	u_int16_t result;
	((u_int8_t *)&result)[0] = fgetc(f);
	((u_int8_t *)&result)[1] = fgetc(f);
	return result;
}

u_int32_t read32(FILE *f){
	u_int32_t result;
	((u_int8_t *)&result)[0] = fgetc(f);
	((u_int8_t *)&result)[1] = fgetc(f);
	((u_int8_t *)&result)[2] = fgetc(f);
	((u_int8_t *)&result)[3] = fgetc(f);
	return result;
}

int main(void){
	clock_t begin,end;
	char filename[64];
	char str[64];
	double num;
	int r,g,b,start;
	int size=64*32;
	
	//ファイル名生成ループ
	FILE *fp_read,*fp_write;
	char fname[64] = "data/data.txt";//書き込むファイル
	//char fname[64] = "bmp256128/data.txt";
	fp_write=fopen(fname,"a");
	begin=clock();
	for(int n=1;n<=4324;n++){
		num=n;
		sprintf(str,"%d",n);
		strcpy(filename,"bmp6432/test_");
		num = (double)n/1000;
		while(num < 1.0){
			strcat(filename,"0");
			num *= 10.0;
		}
		strcat(filename,str);
		strcat(filename,".bmp");
		//それぞれのファイルに対しての処理
		//printf("%s\n",filename);
		fp_read=fopen(filename,"r");
		if(fp_read==NULL){
			printf("%s\n",filename);
			printf("Not found file!\n");
			return 1;
		}
		fseek(fp_read,10,SEEK_SET);
		start=read32(fp_read);
		fseek(fp_read,start,SEEK_SET);
		int count=0;
		while(count<size){
			count+=1;
			b=fgetc(fp_read);
			g=fgetc(fp_read);
			r=fgetc(fp_read);
			fprintf(fp_write,"%02X",r);
			fprintf(fp_write,"%02X",g);
			fprintf(fp_write,"%02X",b);
		}
		fclose(fp_read);
	}
	fclose(fp_write);
	end=clock();
	printf("Execution time = %lf [s]\n",(double)(end-begin)/CLOCKS_PER_SEC);
	return 0;
}
