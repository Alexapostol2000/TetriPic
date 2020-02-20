#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

////////////-- header bitmap --////////////
#pragma pack(1)
struct bmp_fileheader
{
    unsigned char  fileMarker1; /* 'B' */
    unsigned char  fileMarker2; /* 'M' */
    unsigned int   bfSize; /* File's size */
    unsigned short unused1; /* 0 */
    unsigned short unused2; /* 0 */
    unsigned int   imageDataOffset; /* Offset to the start of image data */
};
struct bmp_infoheader
{
    unsigned int   biSize; /* Size of the info header - 40 bytes */
    signed int     width; /* Width of the image */
    signed int     height; /* Height of the image */
    unsigned short planes; /**/
    unsigned short bitPix;
    unsigned int   biCompression;
    unsigned int   biSizeImage; /* Size of the image data */
    int            biXPelsPerMeter;
    int            biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
};
typedef struct
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
} bmp_color;
#pragma pack()

////////////-- malloc_tetris --////////////
bmp_color** malloc_tetris(int latime, int inaltime){
    bmp_color**mem_linii=malloc(inaltime*sizeof(bmp_color*));
    int i;
    for(i=0;i<inaltime;i++){
		mem_linii[i]=malloc(latime*sizeof(bmp_color));
		int j;
		for(j=0;j<latime;j++){
			mem_linii[i][j].r = 0;
			mem_linii[i][j].g = 0;
			mem_linii[i][j].b = 0;
		}
    }
    return mem_linii;
}

////////////-- free_tetris --////////////
void free_tetris(bmp_color**mem_linii, int inaltime){
    int i;
    for(i=0;i<inaltime;i++){
		free(mem_linii[i]);
    }
    free(mem_linii);
}

void patrat(bmp_color**mem_linii, bmp_color culoare, int x, int y){
    int i, j;
    for(i=x;i<x+10;i++){
		for (j=y;j<y+10;j++){
			mem_linii[j][i]=culoare;
		}
    }
}

////////////-- write_tetris --////////////
void scrie_tetris(int latime, int inaltime, bmp_color**mem_linii, char nfisier[]){
    bmp_color**imagine=malloc_tetris(latime*10,inaltime*10);
    int i, j;
    for(i=0;i<latime;i++){
		for (j=0;j<inaltime;j++){
			patrat(imagine, mem_linii[j][i], i*10, j*10);
		}
    }
    struct bmp_fileheader header1 = { 0 };
    struct bmp_infoheader header2 = { 0 };
    FILE*fisier=fopen(nfisier, "wb");
    int padding_rand=0;
    while(((padding_rand+latime*30)%4)!=0) padding_rand++;
    header1.fileMarker1='B';
    header1.fileMarker2='M';
    header1.bfSize=sizeof(struct bmp_infoheader)+sizeof(struct bmp_fileheader)+(padding_rand+latime*30)*inaltime*10;
    header1.imageDataOffset=sizeof(struct bmp_infoheader)+sizeof(struct bmp_fileheader);
    header2.biSize=sizeof(struct bmp_infoheader);
    header2.width=latime*10;
    header2.height=inaltime*10;
    header2.planes=1;
    header2.bitPix=24;
    header2.biSizeImage=(unsigned int)((padding_rand+latime*30)*inaltime*10);
    fwrite(&header1, sizeof(struct bmp_fileheader), 1, fisier);
    fwrite(&header2, sizeof(struct bmp_infoheader), 1, fisier);
    for(i=0;i<inaltime*10;i++){
		fwrite(imagine[inaltime*10-i-1], latime*30, 1, fisier);
		// randurile invers
		if (padding_rand!=0) fwrite("\0\0\0\0", padding_rand, 1, fisier);
    }
    fclose(fisier);
    free_tetris(imagine, inaltime*10);
}

void piesa(char tip, bmp_color**h, int x, int y){
    bmp_color c;
    if(tip=='I'){
		c.r=0; c.g=0; c.b=255;
		h[y][x]=c;
		h[y+1][x]=c;
		h[y+2][x]=c;
		h[y+3][x]=c;
    }
    if(tip=='O'){
		c.r=255; c.g=255; c.b=0;
		h[y][x]=c;
		h[y][x+1]=c;
		h[y+1][x]=c;
		h[y+1][x+1]=c;
    }
    if(tip=='S'){
		c.r=255; c.g=0; c.b=0;
		h[y][x+1]=c;
		h[y][x+2]=c;
		h[y+1][x]=c;
		h[y+1][x+1]=c;
    }
    if(tip=='Z'){
		c.r=0; c.g=255; c.b=0;
		h[y][x]=c;
		h[y][x+1]=c;
		h[y+1][x+1]=c;
		h[y+1][x+2]=c;
    }
    if(tip=='L'){
		c.r=255; c.g=140; c.b=0;
		h[y][x]=c;
		h[y+1][x]=c;
		h[y+2][x]=c;
		h[y+2][x+1]=c;
    }
    if(tip=='J'){
		c.r=255; c.g=0; c.b=255;
		h[y][x+1]=c;
		h[y+1][x+1]=c;
		h[y+2][x+1]=c;
		h[y+2][x]=c;
    }
    if(tip=='T'){
		c.r=130; c.g=0; c.b=255;
		h[y][x]=c;
		h[y][x+1]=c;
		h[y][x+2]=c;
		h[y+1][x+1]=c;
    }
}

bmp_color**task1_generator_piesa(char tip, int latime, int inaltime) {
    int i, j;
    bmp_color**harta=malloc_tetris(latime+2, inaltime+2);
    for(i=0;i<inaltime+2;i++){
		for(j=0;j<latime+2;j++){
			harta[i][j].r = 255;
			harta[i][j].g = 255;
			harta[i][j].b = 255;
		}
    }
    piesa(tip, harta, 1, 1);
    return harta;
}

int latime_piesa(char tip) {
    if(tip=='I') return 1;
    if(tip=='O') return 2;
    if(tip=='S') return 3;
    if(tip=='Z') return 3;
    if(tip=='L') return 2;
    if(tip=='J') return 2;
    if(tip=='T') return 3;
    return 0;
}

int inaltime_piesa(char tip) {
    if(tip=='I') return 4;
    if(tip=='O') return 2;
    if(tip=='S') return 2;
    if(tip=='Z') return 2;
    if(tip=='L') return 3;
    if(tip=='J') return 3;
    if(tip=='T') return 2;
    return 0;
}

void task1_piesa(char tip){
    char nume[20] = "piesa_*.bmp";
    nume[6]=tip;
    bmp_color**harta=task1_generator_piesa(tip, latime_piesa(tip), inaltime_piesa(tip));
    scrie_tetris(latime_piesa(tip)+2, inaltime_piesa(tip)+2, harta, nume);
    free_tetris(harta, inaltime_piesa(tip)+2);
}

void rotatie_90(bmp_color**sursa, bmp_color**destinatie, int latime, int inaltime){
    int i, j;
    for(i=0;i<inaltime;i++){
		for(j=0;j<latime;j++){
			destinatie[j][inaltime-i-1]=sursa[i][j];
		}
    }
}

void task2_piesa(char tip){
    char nume1[20] = "piesa_*_90.bmp";
    char nume2[20] = "piesa_*_180.bmp";
    char nume3[20] = "piesa_*_270.bmp";
    nume1[6]=tip;
    nume2[6]=tip;
    nume3[6]=tip;
    int inaltime=inaltime_piesa(tip)+2, latime=latime_piesa(tip)+2;
    bmp_color**h1=task1_generator_piesa(tip, latime, inaltime);
    bmp_color**h2=malloc_tetris(inaltime, latime);

    ///////////////////////////-- 90 --///////////////////////////
    rotatie_90(h1, h2, latime, inaltime);
    scrie_tetris(inaltime, latime, h2, nume1);

    ///////////////////////////-- 180 --//////////////////////////
    rotatie_90(h2, h1, inaltime, latime);
    scrie_tetris(latime, inaltime, h1, nume2);

    ///////////////////////////-- 270 --//////////////////////////
    rotatie_90(h1, h2, latime, inaltime);
    scrie_tetris(inaltime, latime, h2, nume3);

    free_tetris(h1, inaltime);
    free_tetris(h2, latime);
}

int main(int argc, char* argv[]){
    if (argc==2){
		char task = argv[1][0];
		if (task=='1'){
			task1_piesa('I');
			task1_piesa('O');
			task1_piesa('S');
			task1_piesa('Z');
			task1_piesa('L');
			task1_piesa('J');
			task1_piesa('T');
		}
		if (task=='2'){
			task2_piesa('I');
			task2_piesa('O');
			task2_piesa('S');
			task2_piesa('Z');
			task2_piesa('L');
			task2_piesa('J');
			task2_piesa('T');
		}
		if (task=='3'){
			FILE*piese=fopen("cerinta3.in", "r");
			int nr, inaltime, latime;
			fscanf(piese, "%d %d %d\n", &nr, &inaltime, &latime);
			bmp_color**harta=malloc_tetris(latime, inaltime+4);
			int i, j;
			for(i=0;i<4;i++){
				for(j=0;j<latime;j++){
					harta[i][j].r=255;
					harta[i][j].g=255;
					harta[i][j].b=255;
				}
			}
			while(nr!=0){
				nr--;
				int rot, pos;
				char tip;
				fscanf(piese, "%c %d %d\n", &tip, &rot, &pos);
				bmp_color**piesa=task1_generator_piesa(tip, latime_piesa(tip), inaltime_piesa(tip));
				int p_latime=latime_piesa(tip), p_inaltime=inaltime_piesa(tip);
				if (rot!=0){ // 90
					rot-=90;
					bmp_color**noua=malloc_tetris(p_inaltime+2, p_latime+2);
					rotatie_90(piesa, noua, p_latime+2, p_inaltime+2);
					p_latime=inaltime_piesa(tip);
					p_inaltime=latime_piesa(tip);
					free_tetris(piesa, p_latime+2);
					piesa=noua;
				}
				if (rot!=0){ // 180
					rot-=90;
					bmp_color**noua=malloc_tetris(p_inaltime+2, p_latime+2);
					rotatie_90(piesa, noua, p_latime+2, p_inaltime+2);
					p_latime=latime_piesa(tip);
					p_inaltime=inaltime_piesa(tip);
					free_tetris(piesa, p_latime+2);
					piesa=noua;
				}
				if (rot!=0){ // 270
					rot-=90;
					bmp_color**noua=malloc_tetris(p_inaltime+2, p_latime+2);
					rotatie_90(piesa, noua, p_latime+2, p_inaltime+2);
					p_latime=inaltime_piesa(tip);
					p_inaltime=latime_piesa(tip);
					free_tetris(piesa, p_latime+2);
					piesa=noua;
				}
				int pjos=inaltime+4-p_inaltime;
				for(i=0;i<p_inaltime;i++){
					for(j=0;j<p_latime;j++){
						bmp_color cp=piesa[i+1][j+1];
						if(cp.r!=255||cp.g!=255||cp.b!=255) harta[pjos+i][pos+j]=cp;
					}
				}
				free_tetris(piesa, p_inaltime+2);
			}
			scrie_tetris(latime, inaltime+4, harta, "task3.bmp");
			free_tetris(harta, inaltime+4);
		}
    }
    return 0;
}