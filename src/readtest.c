#include <bcm2835.h>
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
	int size=2048;
	//int size=32768;
	FILE *fp_read;
	char fname[] = "bmp6432/sample.txt";
	//char fname[] = "bmp256128/sample.txt";
	
	int count=0;
	int r,g,b,x;
	
	fp_read=fopen(fname,"r");
	//読み込めなかった場合
	if(fp_read==NULL){
		printf("Not found file!\n");
		return 1;
	}
	
	printf("read from txt file!\n");
	begin=clock();
	while(count<size){
		count+=1;
		fscanf(fp_read,"%2x%2x%2x",&r,&g,&b);
		//printf("%X\n%X\n%X\n",r,g,b);
		//b=fgetc(fp_read);
		//g=fgetc(fp_read);
		//r=fgetc(fp_read);
		//printf("%X\n",fgetc(fp_read));
		//fprintf(fp_write,"%02X",g);
		//fprintf(fp_write,"%02X",b);
	}
	end=clock();
	fclose(fp_read);
	
	return 0;
}
