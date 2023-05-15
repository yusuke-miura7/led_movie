#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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
	begin=clock();
	FILE *fp_read;
	char fname[64] ="bmp6432/data.txt";
	//char fname[64] ="bmp256128/data.txt";
	fp_read=fopen(fname,"r");
	if(fp_read==NULL){
		printf("Not found file!\n");
		return 1;
	}
	fclose(fp_read);
	end=clock();
	
	printf("Execution time = %lf [s]\n",(double)(end - begin) / CLOCKS_PER_SEC);
	
	return 0;
}
