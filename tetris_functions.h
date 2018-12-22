
/*
    Developed by Regson Jaques
    https://www.youtube.com/user/tictacKode/playlists
    https://github.com/tictackode
    https://www.facebook.com/groups/ProgDesafios/
*/
/*
ANOTACOES:

<<<pequena documentação>>>
NUMERO DOS BLOCOS:   1- bloco ativo no jogo, em movimento
                     2- bloco da moldura...paredes do jogo
                     3- peça imovel, que ja colidiu                       X
                                                                          X					X
NUMERO DAS PEÇAS:    0- XX      1-XXXXX      2- XX     3- XXXX      4-    X     5-X     6-  X
                                                XX        X               X       X			X
                                                                          X					XX
 */

#include <allegro.h>
#include <time.h>
#include <string>
#include "Glow.h"

using namespace std;

void isGameOver();
bool colisao2();
void Controle();
void PrintTela();
void setBackground();
void Moldura();
void zeraTela();

char tela[32][32];

//bitmaps declaration
BITMAP* buffer;
BITMAP* blocoVerm;
BITMAP* blocoAzul;
BITMAP* blocoVerde;
BITMAP* blocoCinza;
BITMAP* blocoAmarelo;
BITMAP* blocoRoxo;
BITMAP* blocoLaranja;
BITMAP* blocoRosa;

//audio samples
SAMPLE* hit;
SAMPLE* scored;
SAMPLE* soundGameOver;
SAMPLE* soundLevel;

//background images
BITMAP* fundo;
BITMAP* miku1;
BITMAP* headphone;
BITMAP* blueEyes;
BITMAP* shinny;
BITMAP* music1;
BITMAP* prog_desafios_logo;
BITMAP* tictackode_logo;

FONT* letra;
Glow glow;
//global variables
bool gameover = false;
bool rotateDone = false;
bool colidiu = false;
bool check = false;
bool done = false;
bool descePeca = true;

unsigned int mudaFundo=0;
int timeaux = 0;
bool play = false; // ajuda a tocar o som ao passar de nivel

//tamanho dos blocos, com 28 deu travamento

int tamBloco = 20;

//Dimensoes da tela
int TX = tamBloco * 32, TY=tamBloco * 32;
int slide = TX;

int pontos = 0;
int nivel = 1;
int pecaAtual = 0;
int PosX = 5, PosY = 2;
unsigned int auxSpeed = 0;
int gameSpeed[6] = { 800,650,500, 350, 200, 150 }; // velocidade em que as peças caem

volatile int timer;
void incrementaTimer(){ timer++; }
END_OF_FUNCTION(incrementaTimer)

inline void init()
{   //allegro 4.2 init function
    allegro_init();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, TX, TY, 0, 0);

    install_timer();
    install_keyboard();
    install_mouse();
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT,NULL);
}

inline void deinit()
{   //allegro 4.2 deinit function
    clear_keybuf();
    allegro_exit();
}

void geraTelaInicial()
{
	srand(time(0));
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			int aux = rand() % 10;
			//convertendo int to char
			tela[i][j] = '0' + aux;
		}
	}

}
void PrintBlocosColoridos()
{

	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			char pt = tela[i][j];

			switch (pt)
			{
				case '0':																													   break;
				case '1': blit(blocoAzul, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco); break;
				case '2': blit(blocoVerm, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco); break;
				case '3': blit(blocoAmarelo, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco); break;
				case '4': blit(blocoRoxo, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco); break;
				case '5': blit(blocoLaranja, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco); break;
				case '6': blit(blocoRosa, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco); break;
				case '7': blit(blocoCinza, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco); break;
				case '8':																													   break;
				case '9':																													   break;
			}
		}
	}
}
void ApagaBlocosColoridos()
{

		for (int j = 0; j < 32; j++)
		{
			for (int i = 0; i < 18; i++)
			{
			tela[i][j] = '0';

			}
			setBackground();
			Moldura();
			PrintBlocosColoridos();

			blit(buffer, screen, 0, 0, 0, 0, TX, TY);
			rest(40);
		}
}
void animation()
{
	mudaFundo = 0;
	setBackground();

	Moldura();
	geraTelaInicial();

	PrintBlocosColoridos();

	for (slide = 0; slide < TX; slide+=2)
	{
		setBackground();
		Moldura();
		PrintBlocosColoridos();

		blit(buffer, screen, 0, 0, TX - slide, 0, TX, TY);
		rest(2);
	}

	ApagaBlocosColoridos();

	zeraTela();
}
void setBackground()
{   //sets the background image
	switch (mudaFundo)
	{
		case 0: stretch_blit(music1, buffer, 0, 0, 640, 640, 0, 0, TX, TY); break;
		case 1: stretch_blit(miku1, buffer, 0, 0, 640, 640, 0, 0, TX, TY); break;
		case 2: stretch_blit(headphone, buffer, 0, 0, 640, 640, 0, 0, TX, TY); break;
		case 3: stretch_blit(blueEyes, buffer, 0, 0, 640, 640, 0, 0, TX, TY); break;
		case 4: stretch_blit(shinny, buffer, 0, 0, 640, 640, 0, 0, TX, TY); break;
		case 5: stretch_blit(fundo, buffer, 0, 0, 640, 640, 0, 0, TX, TY); break;
	}
}
void criaBloco(BITMAP* bmp, int red, int green, int blue)
{   // esta funcão gera o bitmap dos blocos

	int escuroR = 30,escuroG=30,escuroB=30;

	if (red < 30) { escuroR = 0; }
	if (green < 30) { escuroG = 0; }
	if (blue < 30) { escuroB = 0; }

	rectfill(bmp, 0, 0, tamBloco - (tamBloco / 10), tamBloco - (tamBloco / 10), makecol(red, green, blue));
	rectfill(bmp, 0, tamBloco - (tamBloco / 10), tamBloco, tamBloco, makecol(red - escuroR, green - escuroG, blue - escuroB));
	rectfill(bmp, tamBloco - (tamBloco / 10), 0, tamBloco, tamBloco, makecol(red - escuroR, green - escuroG, blue - escuroB));

}
void initBlockColor()
{
	// tamBloco é a variavel que guarda o tamanho dos blocos

	blocoAzul=create_bitmap(tamBloco, tamBloco);
	criaBloco(blocoAzul, 32, 32, 192);

	blocoVerde = create_bitmap(tamBloco, tamBloco);
	criaBloco(blocoVerde, 32, 192, 32);

	blocoVerm = create_bitmap(tamBloco, tamBloco);
	criaBloco(blocoVerm, 192, 32, 32);

	blocoAmarelo = create_bitmap(tamBloco, tamBloco);
	criaBloco(blocoAmarelo, 255,215,0);

	blocoRoxo = create_bitmap(tamBloco, tamBloco);
	criaBloco(blocoRoxo, 139, 0, 204);

	blocoCinza = create_bitmap(tamBloco, tamBloco);
	criaBloco(blocoCinza, 64, 64, 64);

	blocoLaranja = create_bitmap(tamBloco, tamBloco);
	criaBloco(blocoLaranja, 255, 153, 0);

	blocoRosa = create_bitmap(tamBloco, tamBloco);
	criaBloco(blocoRosa, 255, 0, 128);
}

void Background()
{
	//Função que desenha um fundo em degrade cinza
	for (int i = 0; i <= 64; i++)
	{
		rectfill(buffer, 0, i * 10, 640, (i * 10) + 10, makecol(32 + i, 32 + i, 32 + i));
	}

}
void levelUp()
{   // funcao para aumentar a velocidade do jogo cada vez que o jogador fizer 500 pontos

	if (pontos == 400)
	{
		auxSpeed = 1;
		nivel=2;

		if (play == false) { play_sample(soundLevel, 255, 128, 1000, 0); /* toca o audio */  play = true; }
	}
	else if (pontos == 800)
	{
		auxSpeed=2;
		nivel=3;

		if (play == true) { play_sample(soundLevel, 255, 128, 1000, 0); /* toca o audio */  play = false; }
	}
	else if (pontos == 1200)
	{
		auxSpeed = 3;
		nivel=4;

		if (play == false) { play_sample(soundLevel, 255, 128, 1000, 0); /* toca o audio */  play = true; }
	}
	else if (pontos == 1600)
	{
		auxSpeed = 4;
		nivel=4;
		if (play == true) { play_sample(soundLevel, 255, 128, 1000, 0); /* toca o audio */  play = false; }
	}
	else if (pontos == 2000)
	{
		auxSpeed = 5;
		nivel=5;
		play_sample(soundLevel, 255, 128, 1000, 0); // toca o audio

		if (play == false) { play_sample(soundLevel, 255, 128, 1000, 0); /* toca o audio */  play = true; }
	}

}
void Moldura()
{

	int i = 0, j = 0;
	for (i = 0; i<17; i++)
	{
		j = 0;
		tela[i][j] = '2';
		j = 31;
		tela[i][j] = '2';
	}

	for (j = 0; j<32; j++)
	{
		i = 0;
		tela[i][j] = '2';
		i = 16;
		tela[i][j] = '2';
	}

}

void PrintTela()
{
	int i = 0, j = 0;
	slide = TX;
	//Background();  fundo desenhado no allegro

	setBackground();

	Moldura();
    glow.glow();
	textprintf_ex(buffer, letra, (tamBloco * 18)-2, (tamBloco * 3)-2, makecol(0, 0, 0), -1, "Pontos:%d", pontos);
	textprintf_ex(buffer, letra, (tamBloco * 18)-2, (tamBloco * 6)-2, makecol(0, 0, 0), -1, "Nivel :%d", nivel);

	textprintf_ex(buffer, letra, (tamBloco*18), (tamBloco*3), makecol(0, 255, 0),-1, "Pontos:%d", pontos);
	textprintf_ex(buffer, letra, (tamBloco*18), (tamBloco*6), makecol(0, 255, 0),-1, "Nivel :%d", nivel);

	textprintf_ex(buffer, font, (tamBloco*18), (tamBloco*18), makecol(0, glow.fontaux, 0),-1, "Se inscreva no canal ");
	textprintf_ex(buffer, font, (tamBloco*18), (tamBloco*19), makecol(0, glow.fontaux, 0),-1, "tictackode no you tube!");
	textprintf_ex(buffer, font, (tamBloco*18), (tamBloco*21), makecol(0, glow.fontaux, 0),-1, "Confira o grupo no Facebook: ");
	textprintf_ex(buffer, font, (tamBloco*18), (tamBloco*22), makecol(0, glow.fontaux, 0),-1, "Programacao: Desafios");

	blit(prog_desafios_logo, buffer,0,0, tamBloco*18, tamBloco*23, 300,93);
   // blit(tictackode_logo, buffer,0,0, tamBloco*18, tamBloco*14, 240,240);
    //void stretch_blit(BITMAP *source, BITMAP *dest, int source_x, source_y, source_width, source_height, int dest_x, dest_y, dest_width, dest_height);
    stretch_blit(tictackode_logo, buffer, 0, 0, 240, 240,tamBloco*27, 200, 100, 100);

	for (i = 0; i<18; i++)
	{
		for (j = 0; j<32; j++)
		{

			char pt = tela[i][j];

			if (pt == '1')
			{


				if (pecaAtual == 0) { blit(blocoVerde, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco); }
				else if (pecaAtual == 1) { blit(blocoVerm, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco); }
				else if (pecaAtual == 2){ blit(blocoAzul, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco); }
				else if (pecaAtual == 3){ blit(blocoAmarelo, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco); }
				else if (pecaAtual == 4){ blit(blocoRoxo, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco); }
				else if (pecaAtual == 5){ blit(blocoLaranja, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco); }
				else if (pecaAtual == 6){ blit(blocoRosa, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco); }

			}
			if (pt == '2')
			{

				blit(blocoVerm, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco);
			}

			if (pt == '3')
			{

				blit(blocoCinza, buffer, 0, 0, (i * tamBloco), (j * tamBloco), (i * tamBloco) + tamBloco, (j * tamBloco) + tamBloco);
			}

		}
	}

	blit(buffer, screen, 0, 0, TX-slide, 0, TX, TY);

}

void limpa1()
{
	for (int i = 0; i<18; i++)
	{
		for (int j = 0; j<32; j++)
		{
			if (tela[i][j] == '1')
			{
				tela[i][j] = '0';
			}
		}
	}
}

void DrawPiece(int piece, int x, int y)
{

	if (piece == 0)
	{
		//

		tela[x][y] = '1';
		tela[x + 1][y] = '1';

	}
	if (piece == 1)
	{
		// reta
		for (int z = 0; z<5; z++)
		{
			tela[x + z][y] = '1';
		}

	}
	if (piece == 2)
	{
		// quadrado
		tela[x][y] = '1';
		tela[x + 1][y] = '1';
		tela[x][y + 1] = '1';
		tela[x + 1][y + 1] = '1';

	}
	if (piece == 3)
	{
		//  L
		tela[x][y] = '1';
		tela[x + 1][y] = '1';
		tela[x + 2][y] = '1';
		tela[x + 3][y] = '1';
		tela[x][y + 1] = '1';
	}

	if (piece == 4)
	{
		tela[x][y] = '1';
		tela[x][y - 1] = '1';
		tela[x][y - 2] = '1';
		tela[x][y - 3] = '1';
		tela[x][y - 4] = '1';
	}

	if (piece == 5)
	{
		//
		tela[x][y] = '1';
		tela[x][y+1] = '1';
	}

	if (piece == 6)
	{
		//  L

		tela[x][y - 1] = '1';
		tela[x][y - 2] = '1';
		tela[x][y - 3] = '1';
		tela[x][y] = '1';
		tela[x+1][y ] = '1';
	}
}

void DownPiece(int pecaAtual, int PosX, int PosY)
{
	limpa1();

  	DrawPiece(pecaAtual, PosX, PosY++);
}

void change1to3()
{

	for (int i = 0; i<18; i++)
	{
		for (int j = 0; j<32; j++)
		{
			if (tela[i][j] == '1')
			{
				tela[i][j] = '3';
			}
		}
	}

}

void rotate()
{
	//  função para cuidar da rotação das peças

	//melhorar essa condição

	if (pecaAtual == 4)
	{
		if (PosX<12)
		{
				 pecaAtual = 1;  limpa1();  DrawPiece(1, PosX, PosY);

		}
	}
	else if(pecaAtual == 1)
	{
		if (PosX<12)
		{
			pecaAtual = 4;  limpa1();  DrawPiece(4, PosX, PosY);

		}
	}
	else if ((pecaAtual == 5) || (pecaAtual == 0))
	{
		if (PosX<15)
		{
			switch (pecaAtual)
			{

				case 0: pecaAtual = 5;  limpa1();  DrawPiece(5, PosX, PosY); break;
				case 5: pecaAtual = 0;  limpa1();  DrawPiece(0, PosX, PosY); break;

			}
		}
	}
	else if ((pecaAtual == 3) || (pecaAtual == 6))
	{
		if (PosX<13)
		{
			switch (pecaAtual)
			{
				case 3: pecaAtual = 6;  limpa1();  DrawPiece(6, PosX, PosY); break;
				case 6: pecaAtual = 3;  limpa1();  DrawPiece(3, PosX, PosY); break;
			}
		}
	}
}
void zeraTela()
{

	for (int i = 0; i<18; i++)
	{
		for (int j = 0; j<32; j++)
		{
			tela[i][j] = '0';
		}
	}
}

void restartGame()
{
	clear(buffer);
	zeraTela();
	limpa1();
	Moldura();
	pontos = 0;
	PosX = 5;
	PosY = 2;
	pecaAtual = rand() % 7;
	DrawPiece(pecaAtual, 3, 3);
	PrintTela();

}

void liberaMemoria()
{
	destroy_bitmap(buffer);
	destroy_bitmap(blocoAzul);
	destroy_bitmap(blocoCinza);
	destroy_bitmap(blocoVerde);
	destroy_bitmap(blocoVerm);
	destroy_bitmap(blocoRoxo);
	destroy_bitmap(blocoRosa);
	destroy_bitmap(blocoLaranja);
	destroy_bitmap(headphone);
	destroy_bitmap(shinny);
	destroy_bitmap(miku1);
	destroy_bitmap(blueEyes);
	destroy_bitmap(fundo);

}

void Controle()
{


		if (key[KEY_SPACE])
		{
			restartGame();
			key[KEY_SPACE] = false;
		}
		else if (key[KEY_ESC])
		{

			done = true;
			liberaMemoria();
			deinit();
			exit(0);

		}
		else if (key[KEY_A])
		{

			tamBloco++;
			TX = tamBloco * 32;
			TY = tamBloco * 32;
			buffer = create_bitmap(TX, TY);
			set_gfx_mode(GFX_AUTODETECT_WINDOWED, TX, TY, 0, 0);

			initBlockColor();
			PrintTela();

		}
		else if (key[KEY_Z])
		{

			tamBloco--;
			TX = tamBloco * 32;
			TY = tamBloco * 32;
			buffer = create_bitmap(TX, TY);
			set_gfx_mode(GFX_AUTODETECT_WINDOWED, TX, TY, 0, 0);

			initBlockColor();
			PrintTela();


		}
		else if (key[KEY_DOWN])
		{

			DownPiece(pecaAtual, PosX, PosY++);
			key[KEY_DOWN] = false;

		}
		else if (key[KEY_LEFT])
		{



			if ((pecaAtual == 0)||(pecaAtual==1))
			{
				/*
				XX
				*/

				if (tela[PosX -1][PosY] == '0')
				{
					limpa1();
					DrawPiece(pecaAtual, PosX--, PosY);

				}
			}


			if (pecaAtual == 2)
			{
				/*
				XX
				XX
				*/
				if ((tela[PosX - 1][PosY] == '0')&&(tela[PosX - 1][PosY +1] == '0'))
				{
					limpa1();
					DrawPiece(pecaAtual, PosX--, PosY);

				}


			}

			if (pecaAtual == 3)
			{
				/*
				XXXX
				X
				*/
				if ((tela[PosX - 1][PosY] == '0') && (tela[PosX - 1][PosY + 1] == '0'))
				{
					limpa1();
					DrawPiece(pecaAtual, PosX--, PosY);

				}

			}

			if (pecaAtual == 4)
			{
				/*
				X
				X
				X
				X
				X
				*/
				if ((tela[PosX - 1][PosY] == '0') && (tela[PosX - 1][PosY - 1] == '0') && (tela[PosX - 1][PosY - 2] == '0') && (tela[PosX - 1][PosY - 3] == '0') && (tela[PosX - 1][PosY - 4] == '0'))
				{

					limpa1();
					DrawPiece(pecaAtual, PosX--, PosY);


				}
			}
			if (pecaAtual == 5)
			{
				/*
				X
				X
				*/

				if ((tela[PosX - 1][PosY] == '0') && (tela[PosX - 1][PosY + 1] == '0'))
				{

					limpa1();
					DrawPiece(pecaAtual, PosX--, PosY);


				}
			}
			if (pecaAtual == 6)
			{
				/*
				X
				X
				X
				XX
				*/

				if ((tela[PosX -1][PosY] == '0') && (tela[PosX-1][PosY - 1] == '0') && (tela[PosX-1][PosY - 2] == '0') && (tela[PosX-1][PosY - 3] == '0'))
				{
					limpa1();
					DrawPiece(pecaAtual, PosX--, PosY);

				}

			}

			key[KEY_LEFT] = false;

		}
		else if (key[KEY_RIGHT])
		{


			if(pecaAtual == 0)
			{
				/*
					XX
				*/

				if (tela[PosX + 2][PosY] == '0')
				{
					limpa1();
					DrawPiece(pecaAtual, PosX++, PosY);

				}
			}

			if (pecaAtual == 1)
			{
				/*
						XXXXX
				*/

				if (tela[PosX + 5][PosY] == '0')
				{
					limpa1();
					DrawPiece(pecaAtual, PosX++, PosY);

				}
			}

			if (pecaAtual == 2)
			{
				/*
				XX
				XX
				*/
				if (tela[PosX + 2][PosY] == '0')
				{
					limpa1();
					DrawPiece(pecaAtual, PosX++, PosY);

				}
				if (tela[PosX + 2][PosY + 1] == '0')
				{
					limpa1();
					DrawPiece(pecaAtual, PosX++, PosY);

				}

			}

			if (pecaAtual == 3)
			{
				/*
				XXXX
				X
				*/
				if ((tela[PosX + 4][PosY] == '0') && (tela[PosX + 1][PosY + 1] == '0'))
				{
					limpa1();
					DrawPiece(pecaAtual, PosX++, PosY);

				}

			}

			if (pecaAtual == 4)
			{

				/*
				X
				X
				X
				X
				X
				*/

				if ((tela[PosX + 1][PosY] == '0') && (tela[PosX + 1][PosY - 1] == '0') && (tela[PosX + 1][PosY - 2] == '0') && (tela[PosX + 1][PosY - 3] == '0' )&& (tela[PosX + 1][PosY-4] == '0') )
				{

					limpa1();
					DrawPiece(pecaAtual, PosX++, PosY);


				}
			}

			if (pecaAtual == 5)
			{
				/*
				X
				X
				*/

				if ((tela[PosX + 1][PosY] == '0') && (tela[PosX + 1][PosY+1] == '0'))
				{

					limpa1();
					DrawPiece(pecaAtual, PosX++, PosY);


				}
			}

			if (pecaAtual == 6)
			{
				/*
					X
					X
					X
					XX
				*/

				if ((tela[PosX+2][PosY] == '0') && ((tela[PosX+2][PosY-1] == '0') && (tela[PosX+2 ][PosY-2] == '0') && (tela[PosX+2][PosY-3] == '0')))
				{
					limpa1();
					DrawPiece(pecaAtual, PosX++, PosY);

				}

			}

			key[KEY_RIGHT] = false;
		}
		else if (key[KEY_UP])
		{
			//	if (rotateDone == false)
			//	{
			rotate();
			//	}
			//	rotateDone = true;
			key[KEY_UP] = false;
		}
		//clear_keybuf();
}

void Desce3(int linha)
{

   int i = 0, line=linha;

    for( ; line>1;line--)
	{
       	for(i=1  ; i<16;i++) //varre cada bloco em uma linha
	    {
             //  Recebe o bloco que estiver em cima
             tela[i][line]=tela[i][line-1];

        }

	}
}

void delLine(int linha)
{
	// funcao para deletar uma linha completa
	int i = 0;

 for(int a=0;a<6;a++)
 {

	for (i = 0; i<17; i++)
	{
		tela[i][linha] = '0';
	}
	PrintTela();
	rest(50);
		for (i = 0; i<17; i++)
	{
		tela[i][linha] = '1';
	}
	PrintTela();
	rest(50);

}
     for (i = 0; i<17; i++)
	{
		tela[i][linha] = '0';
	}

}

void changeBackground()
{

	if (mudaFundo < 4)
	{
		mudaFundo++;
	}
	else
	{
		mudaFundo = 0;
	}
}

void IsLine()
{
	// verifica se ha uma linha completa
	int conta = 0;

	for (int j = 1; j<32; j++)
	{
		for (int i = 1; i<17; i++)
		{
			if (tela[i][j] == '3')
			{
				conta++;
			}
			if (conta==15)
			{
				play_sample(scored, 255, 128, 1000, 0);
				delLine(j);
				Desce3(j);

				changeBackground();

				pontos += 200;
				conta = 0;
				i = 17;
				j = 32;
			}
		}
		conta = 0;
	}
}

bool colisao2()
{

	colidiu = false;

	if (pecaAtual == 3)
	{

			if ((tela[PosX][PosY + 1]=='3')||(tela[PosX][PosY+1 ]=='2')||\
            (tela[PosX+1][PosY ] =='3')||(tela[PosX][PosY ]=='2')||\
            (tela[PosX+2][PosY  ] =='3')||(tela[PosX][PosY  ]=='2')||\
            (tela[PosX+3][PosY  ] =='3')||(tela[PosX][PosY ]=='2'))
			{
				colidiu = true;
				change1to3();
				limpa1();
				play_sample(hit, 128, 128, 1000, 0);
			}

	}
	if (pecaAtual == 0)
	{
		/*
			XX
		*/
		if ((tela[PosX][PosY + 1] == '3') || (tela[PosX][PosY + 1] == '2') || (tela[PosX + 1][PosY + 1] == '3') || (tela[PosX + 1][PosY + 1] == '2') )
		{
			DownPiece(pecaAtual, PosX, PosY++);
			colidiu = true;
			change1to3();
			limpa1();
			play_sample(hit, 128, 128, 1000, 0);
		}
	}
	if (pecaAtual == 1)
	{
		//   XXXXX


			if (((tela[PosX ][PosY+1 ] == '3') || (tela[PosX ][PosY +1] == '2'))||\
			   ((tela[PosX+1 ][PosY+1 ] == '3') || (tela[PosX+1 ][PosY +1] == '2'))||\
			   ((tela[PosX +2][PosY+1 ] == '3') || (tela[PosX+2 ][PosY +1] == '2'))||\
			   ((tela[PosX +3][PosY+1 ] == '3') || (tela[PosX +3][PosY +1] == '2'))||\
			   ((tela[PosX +4][PosY+1 ] == '3') || (tela[PosX +4][PosY +1] == '2')))
			{
				DownPiece(pecaAtual, PosX, PosY++);
				colidiu = true;
				change1to3();
				limpa1();
				play_sample(hit, 128, 128, 1000, 0);
			}

	}
	if ((pecaAtual == 2))
	{
		if ((tela[PosX][PosY + 1] == '3') || (tela[PosX][PosY + 1] == '2') || (tela[PosX + 1][PosY + 1] == '3') || (tela[PosX + 1][PosY + 1] == '2'))
		{
			colidiu = true;
			change1to3();
			limpa1();
			play_sample(hit, 128, 128, 1000, 0);
		}
	}

	if (pecaAtual == 4)
	{

		/*
		X
		X
		X
		X
		X
		*/

		if ((tela[PosX][PosY + 1] == '3') || (tela[PosX][PosY + 1] == '2') )
		{
			DownPiece(pecaAtual, PosX, PosY++);
			colidiu = true;
			change1to3();
			limpa1();
			play_sample(hit, 128, 128, 1000, 0);
		}
	}

	if (pecaAtual == 5)
	{

		/*
				X
				X
		*/

		if ((tela[PosX][PosY + 2] == '3') || (tela[PosX][PosY + 2] == '2'))
		{
			DownPiece(pecaAtual, PosX, PosY++);
			colidiu = true;
			change1to3();
			limpa1();
			play_sample(hit, 128, 128, 1000, 0);
		}
	}

	if (pecaAtual == 6)
	{
		/*
				X
				X
				X
				XX
		*/


		if ((tela[PosX][PosY + 1] == '3') || (tela[PosX][PosY + 1] == '2') || (tela[PosX+1][PosY + 1] == '3') || (tela[PosX+1][PosY + 1] == '2'))
		{
			DownPiece(pecaAtual, PosX, PosY++);
			colidiu = true;
			change1to3();
			limpa1();
			play_sample(hit, 128, 128, 1000, 0);
		}
	}


	return colidiu;
}

void loopPrincipal()
{

     while ((!done)||(!key[KEY_ESC]))
    {


		install_int_ex(incrementaTimer, MSEC_TO_TIMER(gameSpeed[auxSpeed]));
		if (descePeca == true) { timeaux = timer; descePeca = false; }


        bool check=colisao2();  // checa se há colisão
        if (check)
        {
            check=false;
            pecaAtual=rand()%7;

            if((pecaAtual==4)||(pecaAtual==6)) { PosX=5; PosY=5; }
            else { PosX=5; PosY=1; }

            DrawPiece(pecaAtual, PosX,PosY);
        }


		if (timer > timeaux) {
			DownPiece(pecaAtual, PosX, PosY++); descePeca = true;    }

		IsLine();   // confere se houve linha completa

		Controle();   //   le as teclas

        PrintTela(); // atualiza tela do jogo

        levelUp();  // confere pontuação para aumentar nivel

		isGameOver();

    } // while
}

void isGameOver()
{

	for (int i = 1; i<16; i++)
	{

		if (tela[i][1] == '3')
		{
		    i=16;
		    done=true;
			play_sample(soundGameOver, 255, 128, 1000, 0); // toca o audio
			while(!key[KEY_ESC])
            {

				textprintf_ex(screen, letra, 180-2, 300-2, makecol(0, 0, 0),-1, "GAME OVER");
			    textprintf_ex(screen, letra, 180, 300, makecol(255, 0, 0),-1, "GAME OVER");

        		if (key[KEY_SPACE])
	     		{
					done=false;
					restartGame();
					loopPrincipal();
	      	    }

			}
			// se chegou aqui é pra encerrar o jogo
			done = true;

		}
	}
}
BITMAP* loadImage(BITMAP* image,int img_width,int img_height,const char* image_path)
{

        image=create_bitmap(img_width,img_height);
        image=load_bitmap(image_path,0);
        std::string buf("error: couldn't find image ESC to exit : ");
        buf.append(image_path);

        if(!image)
        {
            key[KEY_ESC]=false;
            //Função que desenha um fundo em degrade cinza
            for (int i = 0; i <= 64; i++)
            {
                rectfill(screen, 0, i * 10, 640, (i * 10) + 10, makecol(32 + i, 32 + i, 32 + i));
            }
            while(!key[KEY_ESC])
            {
                textprintf_ex(screen, font, 20, 20, makecol(255, 0, 0),-1,buf.c_str());
            }

            readkey(); liberaMemoria(); deinit(); exit(1);
        }

    return image;
}
void loadAll()
{
        letra=load_font("fontes/font26.pcx",NULL,NULL);

        buffer=create_bitmap(TX,TY);

        fundo=loadImage(fundo,640,640,"img/fundo_dark_girl.bmp");
		miku1 = loadImage(miku1,640,640,"img/miku1.bmp");
		headphone = loadImage(headphone,640,640,"img/headphone.bmp");
		blueEyes = loadImage(blueEyes,640,640,"img/blueEyes.bmp");
		shinny = loadImage(shinny,640,640,"img/shinny.bmp");
		music1=loadImage(fundo,640,640,"img/music1.bmp");
        prog_desafios_logo = loadImage(prog_desafios_logo,300,93,"img/prog_desafios_logo.bmp");
        tictackode_logo = loadImage(tictackode_logo,240,240,"img/tictackode_logo.bmp");

		hit = load_sample("sons/phase2.wav");
		scored = load_sample("sons/laser_x.wav");
		soundGameOver = load_sample("sons/buzzer3.wav");
		soundLevel = load_sample("sons/cheering.wav");

}
