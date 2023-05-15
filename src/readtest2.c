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
	//int size=2048;
	int size=32768;
	
	int counter=0;
	int r,g,b;
	begin=clock();
	while(counter<60){
		FILE *fp_read;
		char fname[] = "bmp256128/sample.txt";
		int count=0;
		fp_read=fopen(fname,"r");
		if(fp_read==NULL){
			printf("Not found file!\n");
			return 1;
		}
		
		while(count<size){
			count+=1;
			fscanf(fp_read,"%2x%2x%2x",&r,&g,&b);
		}
		counter+=1;
		fclose(fp_read);
	}
	end=clock();
	
	printf("Execution time = %lf [s]\n",(double)(end - begin) / CLOCKS_PER_SEC);
	
	return 0;
}
