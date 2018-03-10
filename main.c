asm(".code16gcc\n");
void setVideo(int);
void setPixelMem(int x,int y,int color);
void showimage(int start_adr);
void window(int x1,int y1,int x2,int y2,int color);
void setCharXY(int x,int y,int color,int c);
void print(char* , int , int , int );
void window(int x1,int y1,int x2,int y2,int color);
int hitkey();
void setVideo1(int mode);
void MainMenu();
void interface(int c1,int c2,int c3,int c4);
void navigation();
// Window with date
void Date();
int CMOSDate(int arg);
void printDate(int x,int y,int color,int c);
void showDate(int x, int y, int color);
int hitkeyPort();
//Window with status
void config();
void floppyDrive();
void existFPU();
void monitorType();
void existKeyboard();
void existMonitor();
void numberFloppy();
//Window with status
void status();
void extraMemory();

int _main() {
	int a;
	int start_adr=0;
	asm("mov ax,word ptr[ebp]");
	asm("mov ax,[eax]");
	asm("mov word ptr[ebp-0xC],ax");
	showimage(start_adr);
	hitkey();
	navigation();
	asm("jmp end");
	return 0;
}
void interface(int c1,int c2,int c3,int c4) {
	setVideo(0x3);
	char instr[]="1-Config, 2-Status, 3-Date/time, 4-Exit";
	char text[]="Solntsev OS v0.1";
	char text1[]="Config";
	char text2[]="Status";
	char text3[]="Date/time";
	char text4[]="Exit";
	setVideo1(0x3);
	print(text,25,0,0xE);
	window(2,7,14,10,c1);
	print(text1,5,8,c1);
	window(18,7,30,10,c2);
	print(text2,21,8,c2);
	window(34,7,46,10,c3);
	print(text3,36,8,c3);
	window(50,7,62,10,c4);
	print(text4,54,8,c4);
	print(instr,0,15,0xE);
}

void navigation() {
	int a=0,b=0,c;
	interface(0xE,0xe,0xE,0xE);
		while (1) {
			c=hitkey();
			if (c==0x2) {
			interface(2,0xE,0xE,0xE);a=1; }
			if (c==0x3) {
			interface(0xE,2,0xE,0xE);a=2; }
			if (c==0x4) {
			interface(0xE,0xE,2,0xE);a=3; }
			if (c==0x5) {
			interface(0xE,0xE,0xE,2);a=4; }
			if(c==0x1c) {
				switch(a) {
				case 1:
					setVideo(0x3);
					char title1[]="Config";
					print(title1,0,0,0x2E);
					while(1){
						config();
						b=hitkey();
						if(b==1)
							navigation();
					}
				case 2:
					setVideo(0x3);
					char title2[]="Status";
					print(title2,0,0,0x2E);
					while(1) {
						status();
						b=hitkey();
						if(b==1)
							navigation();
					}
				case 3:
					setVideo(0x3);
					char title3[]="Date/time";
					print(title3,0,0,0x2E);
					while(1){
						showDate(1,1,0xE);
						b=hitkey();
						if(b==1)
							navigation();
					}
				case 4:
					setVideo(0x3);
					char title4[]=" Goodbye!";
					print(title4,35,10,0xE);
					asm("jmp [0xf000]");
				}
			}
		}
	}
int hitkey() {
	int a;
	asm("mov ah,0");
	asm("int 0x16");
	asm("mov cl,8");
	asm("shr ax,cl");
}
void setVideo1(int mode) {
	asm("mov ah,0x0");
	asm("mov al,[ebp+0x8]");
	asm("int 0x10");
}
void setVideo(int mode) {
	asm("mov ax,0x4f02");
	asm("mov bx,word ptr[ebp+0x8]");
	asm("int 0x10");
	if(mode>0x100){
	asm("mov ax,0x0106");
	asm("mov dx,0x3CE");
	asm("out dx,ax");
	}
}
void setPixelMem(int x,int y,int color) {
	int pos = (y*320+x)*3;
	asm("mov ax,0xA000");
	asm("mov es,ax");
	asm("mov edi,dword ptr [ebp-0x4]");
	asm("mov ax,word ptr [ebp+0x10]");
	asm("mov word ptr es:[edi],ax");
	pos+=2;
	asm("mov edi,dword ptr [ebp-0x4]");
	asm("mov al,byte ptr [ebp+0x12]");
	asm("mov byte ptr es:[edi],al");
}

void showimage(int start_adr) {
	setVideo(0x10F);
	start_adr+=512*15;
	char* text=start_adr+0x37;

	int color;
	int i,j;
	for(j=98;j>0;j--)
		for(i=-304;i<0;i++){
			color=(*(text+2))<<16;
			color+=(*(text+1))<<8;
			color+=(*text);
			setPixelMem(i,j,color);
			text+=3;
		}
}
void setCharXY(int x,int y,int color,int c) {
	int pos = y*80*2+x*2;
	asm("mov ax,0xB800");
	asm("mov es,ax");
	asm("mov di,0");
	asm("mov di, word ptr [ebp-0x4]");
	asm("mov ah,byte ptr[ebp+0x10]");
	asm("mov al,byte ptr[ebp+0x14]");
	asm("mov word ptr es:[di],ax");
}
void print(char *t, int x, int y, int color){
int i=0;
for(i=0;t[i];i++)
	setCharXY((x+i),y,color,t[i]);
}
void window(int x1,int y1,int x2,int y2,int color) {
	int i,j;
	for(i=y1;i<=y2;i++)
		for(j=x1;j<=x2;j++) {
			if(j==x1 || j==x2) {
				setCharXY(j,i,color,179);
				continue;
			}
			if(i==y1 || i==y2) {
				setCharXY(j,i,color,196);
				continue;
			}
		}
	setCharXY(x1,y1,color,218);
	setCharXY(x2,y1,color,191);
	setCharXY(x1,y2,color,192);
	setCharXY(x2,y2,color,217);
}
void showDate(int x, int y, int color){
	char time[]="Time: ",date[]="Date: ";
	int i,j,i_1,c,x1=x,tmp=0;
	print(date,x1,y,color);
	x1+=6;
	for(i=7;i<=9;i++){
		if(i==9){
			c=CMOSDate(0x32);
			printDate(x1,y,color,c);
			x1+=2;
		}
		c=CMOSDate(i);
		printDate(x1,y,color,c);
		if(i!=9) {
			x1+=2;
			setCharXY(x1,y,color,'.');
			x1++;
		}
	}
	x1=x,y++;
	print(time,x1,y,color);
	x1+=6;
	while(tmp!=1){
		x1=x+6;
		for(i=4;i>=0;i-=2){
			c=CMOSDate(i);
			printDate(x1,y,color,c);
			if(i!=0) {
				x1+=2;
				setCharXY(x1,y,color,':');
				x1++;
			}
		}
		tmp=hitkeyPort();
	}
}
void printDate(int x,int y,int color,int c){
	int c1=c;
	c=c&15;
	c1>>=4;
	c1=c1&15;
	setCharXY(x,y,color,c1+48);
	x++;
	setCharXY(x,y,color,c+48);
}
int CMOSDate(int arg){
	asm("mov al,byte ptr[bp+0x8]");
	asm("out 0x70,al");
    asm("in al,0x71");
}
int hitkeyPort(){
		asm("in al, 0x60");
		asm("and ax,0xff");
	}
void config() {
	char text1[] = " Floppy drive:";
	char text2[] = " FPU:";
	char text3[] = " Keyboard:";
	char text4[] = " Monitor Type:";
	char text5[] = " Monitor exist:";
	char text6[] = " Number of floppy:";
	print(text1,0,4,0xE);
	floppyDrive();
	print(text2,0,1,0xE);
	existFPU();
	print(text3,0,3,0xE);
	existKeyboard();
	print(text4,0,2,0xE);
	monitorType();
	print(text5,0,5,0xE);
	existMonitor();
	print(text6,0,6,0xE);
	numberFloppy();
}
void monitorType() {
	int a;
	char text1[] = "EGA/VGA";
	char text2[] = "40x25 CGA";
	char text3[] = "80x25 CGA";
	char text4[] = "MDA";
	asm("mov al,0x14");
	asm("out 0x70,al");
	asm("in al,0x71");
	asm("shr al,4");
	asm("mov byte ptr[ebp-0x4],al");
	switch(a) {
		case 0b00: print(text1,14,2,0x4);
		break;
		case 0b01: print(text2,14,2,0x4);
		break;
		case 0b10: print(text3,14,2,0x4);
		break;
		case 0b11: print(text4,14,2,0x4);
		break;
	}
}
void existFPU() {
	int a;
	char text1[] = "no";
	char text2[] = "yes";
	asm("mov al,0x14");
	asm("out 0x70,al");
	asm("in al,0x71");
	asm("shr al,1");
	asm("shl al,7");
	asm("shr al,7");
	asm("mov byte ptr[ebp-0x4],al");
	if(a==0b1) 
		print(text2,5,1,0x4);
	else print(text1,5,1,0x4);
	 // 00000111
	}
	void existKeyboard(){
	char a;
	char text1[] = "no";
	char text2[] = "yes";
	asm("mov al,0x14");
	asm("out 0x70,al");
	asm("in al,0x71");
	asm("shr al,2");
	asm("mov byte ptr[ebp-0x4],al");
	if(a==0x1)
		print(text1,10,3,0x4);
	else print(text2,10,3,0x4);
}
void floppyDrive() {
	int a,b;
	char text1[] = "yes";
	char text2[] = "no";
	asm("mov al,0x14");
	asm("out 0x70,al");
	asm("in al,0x71");
	asm("shl al,7");
	asm("mov byte ptr[ebp-0x4],al");
	if(a==0b10000000)
		print(text1,14,4,0x4);
	else 
	print(text2,14,4,0x4);
}
void existMonitor() {
	int a;
	char text1[] = "yes";
	char text2[] = "no";
	asm("mov al,0x14");
	asm("out 0x70,al");
	asm("in al,0x71");
	asm("shr al,3");
	asm("shl al,7");
	asm("mov byte ptr[ebp-0x4],al");
	if(a==0b00000111)
		print(text1,15,5,0x4);
	else 
		print(text2,15,5,0x4);
}
void numberFloppy() {
	int a;
	char text1[] = "1";
	char text2[] = "0";
	asm("mov al,0x14");
	asm("out 0x70,al");
	asm("in al,0x71");
	asm("shr al,6");
	asm("mov byte ptr[ebp-0x4],al");
	if(a==0b0)
		print(text1,18,6,0x4);
	else 
		print(text2,18,6,0x4);
}
void status() {
	char text1[] = " FPU:";
	char text2[] = " Monitor Type:";
	char text3[] = " Extra Memory:";
	print(text1,0,1,0xE);
	existFPU();
	print(text2,0,2,0xE);
	monitorType();
	print(text3,0,3,0xE);
	extraMemory();
}
void extraMemory() {
	int a=0;
	int b=0;
	char text1[] = "65535 KB";
	char text2[] = "0 KB";
	asm("mov al,0x17");
	asm("out 0x70,al");
	asm("in al,0x71"); 
	asm("mov byte ptr[ebp-0x8],al");
	setCharXY(0,2,0x4,b);
	asm("mov al,0x17");
	asm("out 0x70,al");
	asm("in al,0x71");
	asm("mov byte ptr[ebp-0x4],al");
	setCharXY(0,3,0x4,a);
	if(b==0xFF&&a==0xFF)
		print(text1,14,3,0x4);
	else print(text2,14,3,0x4);
}