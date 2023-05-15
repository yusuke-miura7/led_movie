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
	
	FILE *fp_read,*fp_write;
	//char fname[] = "bmp6432/test_0000.bmp";
	//char fname2[] = "bmp6432/sample.txt";
	char fname[] = "bmp256128/test_0000.bmp";
	char fname2[] = "bmp256128/sample.txt";
	
	int count=0;
	int r,g,b,start;
	
	//bmpファイルを開く
	fp_read=fopen(fname,"r");
	//読み込めなかった場合
	if(fp_read==NULL){
		printf("Not found file!\n");
		return 1;
	}
	
	printf("read from bmp file!\n");
	if(read16(fp_read)==0x4D42){
		printf("this is bmp file!\n");
		fseek(fp_read,10,SEEK_SET);
		start=read32(fp_read);
		printf("%d\n",start);
		//rgbの場所に移動
		fseek(fp_read,start,SEEK_SET);
		
		//書き込み用ファイルを開く
		fp_write=fopen(fname2,"w");
		begin=clock();
		while(count<0){
			count+=1;
			b=fgetc(fp_read);
			g=fgetc(fp_read);
			r=fgetc(fp_read);
			fprintf(fp_write,"%02X",r);
			fprintf(fp_write,"%02X",g);
			fprintf(fp_write,"%02X",b);
		}
		printf("%d\n",count);
		fclose(fp_write);
		end=clock();
	}
	fclose(fp_read);
	printf("Execution time = %lf [s]\n",(double)(end - begin) / CLOCKS_PER_SEC);
	
	char filename[64];
	char str[64];
	double num;
	
	//ファイル名生成ループ
	for(int n=1;n<=2;n++){
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
		printf("%s",filename);
	}
	
	return 0;
}
