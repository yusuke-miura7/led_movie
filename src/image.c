#include <bcm2835.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//#define MOSI RPI_V2_GPIO_P1_19 
//#define SCLK RPI_V2_GPIO_P1_23
#define CE0 RPI_V2_GPIO_P1_24
#define DC RPI_V2_GPIO_P1_22

#define x_size 64//横解像度
#define y_size 32//縦解像度

#define WAIT_US_TA 15 //D/C端子変化後
#define WAIT_US_TB 6 //コマンド4バイト受信後
#define WAIT_US_TC 6 //指定された描画範囲分のデータ受信後
#define WAIT_MS_TD 20 //電源投入後

u_int16_t read16(FILE *f){
	u_int16_t result;
	((u_int8_t *)&result)[0] = fgetc(f);
	((u_int8_t *)&result)[1] = fgetc(f);
	return result;
}

//コマンド送信
void send_command(unsigned char command_number, unsigned char command_1,unsigned char command_2, unsigned char command_3){
	unsigned char command_buff[4];
	
	command_buff[0]=command_number;
	command_buff[1]=command_1;
	command_buff[2]=command_2;
	command_buff[3]=command_3;
	
	bcm2835_gpio_write(DC,HIGH);
	bcm2835_delayMicroseconds(WAIT_US_TA);
	bcm2835_gpio_write(CE0,LOW);
	bcm2835_spi_transfern(command_buff,4);
	bcm2835_gpio_write(CE0,HIGH);
	bcm2835_delayMicroseconds(WAIT_US_TB);
}

//キャンバス設定
void set_canvas_size(unsigned short canvas_x,unsigned short canvas_y){
	send_command(0x00,canvas_x & 0x00ff,canvas_y & 0x00ff,((canvas_y >>4) & 0x00f0) | ((canvas_x >> 8) & 0x000f));
	bcm2835_delay(3);
}

//初期化
int init_ledpanel(void){
	//セットアップ
	if(!bcm2835_init()){
		printf("bcm2835_init failed. Are you running as root?\n");
		return 1;
	}
	
	if(!bcm2835_spi_begin()){
		printf("bcm2835_spi_begin failed. Are you runnning as root\n");
		return 1;
	}
	
	bcm2835_gpio_fsel(CE0,BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(CE0,HIGH);
	bcm2835_gpio_fsel(DC,BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(DC,LOW);
	
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	//bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8);//50MHz
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	delay(WAIT_MS_TD);
	
	//表示向きy反転
	send_command(0x14,1,0,0);
	bcm2835_delay(3);
	
	set_canvas_size(x_size,y_size);
}

//描画範囲設定
void set_draw_range(unsigned short draw_start_pixel_x,unsigned short draw_start_pixel_y,unsigned short draw_end_pixel_x,unsigned short draw_end_pixel_y){
	send_command(0x01,draw_start_pixel_x & 0x00ff,draw_end_pixel_x & 0x00ff,((draw_end_pixel_x >> 4) & 0x00f0) | ((draw_start_pixel_x >> 8) & 0x000f));
	send_command(0x02,draw_start_pixel_y & 0x00ff,draw_end_pixel_y & 0x00ff,((draw_end_pixel_y >> 4) & 0x00f0) | ((draw_start_pixel_y >> 8) & 0x000f));
}

//表示位置指定
void set_ledpanel_pos(unsigned char hub75_port, unsigned short pos_x, unsigned short pos_y){
	switch(hub75_port){
		case 0:
		send_command(0x04,pos_x & 0x00ff, pos_y & 0x00ff, ((pos_y >> 4) & 0x00f0) | ((pos_x >> 8) & 0x000f));
		break;
		case 1:
		send_command(0x05,pos_x & 0x00ff, pos_y & 0x00ff, ((pos_y >> 4) & 0x00f0) | ((pos_x >> 8) & 0x000f));
		break;
		case 2:
		send_command(0x06,pos_x & 0x00ff, pos_y & 0x00ff, ((pos_y >> 4) & 0x00f0) | ((pos_x >> 8) & 0x000f));
		break;
		case 3:
		send_command(0x07,pos_x & 0x00ff, pos_y & 0x00ff, ((pos_y >> 4) & 0x00f0) | ((pos_x >> 8) & 0x000f));
		break;
		default:
		break;
	}
}

//任意の場所にビットマップ画像を描画する
void draw_image_for_24bitBMP(unsigned char start_pixel_x,unsigned char start_pixel_y,unsigned char image_size_x,unsigned char image_size_y,unsigned char* draw_data){
	set_draw_range(start_pixel_x,start_pixel_y,(start_pixel_x+image_size_x-1),(start_pixel_y+image_size_y-1));
	
	bcm2835_gpio_write(DC,LOW);
	bcm2835_delayMicroseconds(WAIT_US_TA);
	bcm2835_gpio_write(CE0,LOW);
	for(int y =0;y<image_size_y;y++){
		for(int x =0;x<image_size_x;x++){
			bcm2835_spi_transfer(draw_data[3*x+2+192*y]);
			bcm2835_spi_transfer(draw_data[3*x+1+192*y]);
			bcm2835_spi_transfer(draw_data[3*x+192*y]);
		}
	}
	bcm2835_gpio_write(CE0,HIGH);
}

//リフレッシュレート設定関数
void set_refresh_rate(unsigned short rps,unsigned char extin){
	send_command(0x15,rps & 0x00ff, ((rps >> 8) & 0x003f),extin & 0x01);
	bcm2835_delayMicroseconds(7);
}

//ファイルを読み込んで画像を描画する
int draw_image(unsigned char start_pixel_x,unsigned char start_pixel_y,unsigned char image_size_x,unsigned char image_size_y){
	set_draw_range(start_pixel_x,start_pixel_y,(start_pixel_x+image_size_x-1),(start_pixel_y+image_size_y-1));
	
	//データ(RGB)受信
	bcm2835_gpio_write(DC,LOW);
	bcm2835_delayMicroseconds(WAIT_US_TA);
	bcm2835_gpio_write(CE0,LOW);
	//ファイルを開く
	//char fname[64] = "bmp6432/movie.txt";
	char fname[64] ="bmp6432/movie.txt";
	int count=0;
	int size=image_size_x*image_size_y;
	int r,g,b;
	clock_t begin,end;
	FILE *fp_read;
	fp_read=fopen(fname,"r");
	if(fp_read==NULL){
		printf("Not found file!\n");
		return 1;
	}
	int num=1;
	while(num<4324){
		num+=1;
		count=0;
		while(count<size){
			count+=1;
			fscanf(fp_read,"%2x%2x%2x",&r,&g,&b);
			bcm2835_spi_transfer(r);
			bcm2835_spi_transfer(g);
			bcm2835_spi_transfer(b);
		}
		bcm2835_delayMicroseconds(WAIT_US_TC);
		bcm2835_gpio_write(DC,HIGH);
		bcm2835_delayMicroseconds(WAIT_US_TA);
		bcm2835_gpio_write(DC,LOW);
		bcm2835_delayMicroseconds(WAIT_US_TA);
		bcm2835_delay(24);
	}
	fclose(fp_read);
	end=clock();
	bcm2835_gpio_write(CE0,HIGH);
	return 0;
}

int main(int argc,char **argv){
	//char check[] = "data/movie.txt";
	if(init_ledpanel()==1){
		return 1;
	}
	set_refresh_rate(2400,0);
	if(draw_image(0,0,64,32)==1){
		return 1;
	}
	//draw_image_for_24bitBMP(64,0,64,32,image);
	//draw_image_for_24bitBMP(128,0,64,32,image);
	bcm2835_delay(1000);
	
	/*
	while(1){
		for(int loop=0;loop<x_size*3;loop++){
			set_ledpanel_pos(0,loop,0);
			//スクロール速度
			bcm2835_delay(30);
		}
	}
	*/
	bcm2835_spi_end();
	bcm2835_close();
	return 0;
}
