#include <stdio.h>
#include <pthread.h>
#include <ncurses.h>
#include <math.h>
#include <time.h>

/*
ncurses: apt-get install libncurses5-dev
gcc -o pong pong.c -lncurses -lpthread
*/

int x_pelota, y_pelota, x_paleta, y_paleta, dpx_pelota, dpy_pelota;
int dificultad, key;
int error_hilo1, error_hilo2;
pthread_t hiloPosiciones, hiloMoverPelota;

void print_marco()//Imprime el marco
{	
	int i;	
	for(i = 0; i < LINES; i++)
	{
		move(i,0);
		printw("|");
		move(i, COLS-1);
		printw("|");
	}
	for(i = 0; i < COLS; i++)
	{
		move(0, i);
		printw("_");
		move(LINES - 1, i);
		printw("-");
	}
}
void print_pelota()//Imprime la pelota en una posicion aleatoria
{
	x_pelota = (int) ( rand() % ((LINES/2+4)-(LINES/2-4)+1) + (LINES/2 - 4) );
	y_pelota = (int) ( rand() % ( (((COLS-1) - COLS/9) + 4)-(((COLS-1) - COLS/9) - 4)+1 ) + (((COLS-1) - COLS/9) - 4) );
	move(x_pelota, y_pelota);
	printw("O");
}
void pintar_paleta()//Imprime la paleta que hace rebotar a la pelota
{
	y_paleta = 3;
	x_paleta = LINES/2 - 3;
	int i;
	for( i = 0; i < 4; i++)
	{
		move(x_paleta + i, y_paleta);
		printw("#");
	}
}
void dir_pelota_random()//Direccion en la que la pelota se movera la primera vez
{
	dpx_pelota = ( rand() % (+1 - (-1) +1) + (-1) );
	dpy_pelota = -1;
}
void *hiloMovPosiciones()
{
	for(;;)
	{
		int i;
		clear();
		print_marco();
		//------------MOVIMIENTO DE LA PALETA----------------
		if(y_paleta != 0 && y_paleta <= COLS -1 && x_paleta != 0 && (x_paleta - 3) != LINES - 1)// SE ENCUENTRE DENTRO DEL MARCO
		{}
		else
		{
			if(y_paleta <= 0)
			{
				y_paleta = 1;	
			}
			if (y_paleta >= COLS - 1)
			{
				y_paleta = COLS - 2;
			}
			if (x_paleta <= 0)
			{
				x_paleta = 1;
			}
			if ((x_paleta + 3) >= LINES - 1)
			{
				x_paleta = LINES - 4;
			}
		}
		for( i = 0; i < 4; i++ )
	       {
			move(x_paleta + i, y_paleta);
			printw("#");
	       }
		//----------MOVIMIENTO DE LA PELOTA (FIN, CHOCA PALETA)-------------------
		move(x_pelota, y_pelota);
		printw("O");
		if(y_pelota <= 0) //La pelota choco con el marco
		{
			kill(hiloPosiciones);
			kill(hiloMoverPelota);
			clear();
			move(LINES/2, COLS/2);
			printw(	"FIN DEL JUEGO."
				"\nPresione E para salir_");
		}
		if ( (y_pelota == y_paleta) && ((x_pelota == x_paleta) 
		|| (x_pelota == x_paleta + 1) || (x_pelota == x_paleta + 2)  || (x_pelota == x_paleta + 3)) )// Si rebota con la paleta
		{
			dpy_pelota = - dpy_pelota;
			y_pelota = y_pelota + dpy_pelota;//Actualiza posicion pelota
		}
		usleep(20*1000);
		refresh(); 
	}
}
void *hiloMovPelota()
{
	for(;;)
	{
		if (!( (y_pelota == y_paleta) && ((x_pelota == x_paleta) 
		|| (x_pelota == x_paleta + 1) || (x_pelota == x_paleta + 2)  || (x_pelota == x_paleta + 3))) ) // Si no rebota con la paleta
		{
			x_pelota = x_pelota + dpx_pelota;
			y_pelota = y_pelota + dpy_pelota;
		}
		if (x_pelota <= 1) //Rebota con la pared arriba
		{
			dpx_pelota = - dpx_pelota;
		}

		if (x_pelota >= LINES - 1) //Rebota con la pared abajo
		{
			dpx_pelota = - dpx_pelota;    
		}

		if (y_pelota >= COLS - 1) ////Rebota con la pared derecha
		{
			dpy_pelota = - dpy_pelota;    
		}
		usleep( dificultad*1000 );
	}
}
void menu()
{
	char d;
	clear();
	move(3,3);
	printw(	"\n-Pong-Controles:\n"
		"\t w = Mover la paleta hacia arriba.\n"
		"\t S = Mover la paleta hacia abajo.\n"
		"\t E = Salir.\n"
		"\n-Indique el nivel de dificultad:\n"
		"\t1 - Fácil.\n"
		"\t2 - Medio.\n"
		"\t3 - Difícil.\n\n");
	d = getch();
	switch(d)
	{
		case '1': dificultad = 200; break;
		case '2': dificultad = 100; break;
		case '3': dificultad = 50; break;
		default:
		clear();
		move(3,3);
		printw("No se ha seleccionado ningún nivel de dificultad. Pulse una tecla para continuar");
			menu();
		getch();
		break;
	}
}
int cerrar()//Cierra
{
	endwin();
	return 0;
}
int main()
{
	initscr();
	noecho();
	
	print_marco();
	pintar_paleta();
	print_pelota();
	dir_pelota_random();
	menu();
	srand( time(NULL) );
 
	error_hilo1 = pthread_create(&hiloPosiciones, NULL, hiloMovPosiciones, NULL);
	error_hilo2 = pthread_create(&hiloMoverPelota, NULL, hiloMovPelota, NULL);
	
	if (error_hilo1 != 0 && error_hilo2 != 0) //Error al crear los hilos
	{
		clear();
		move(LINES/2,COLS/2);
		printw("ERROR AL CREAR LOS HILOS.");
		return -1;	
	}
	while( key != 'e' && key != 'E' )//Pulsa controles
	{
		key = getch();
		if (key == 'w' || key == 'W') { x_paleta = x_paleta - 1; }
		if (key == 's' || key == 'S') { x_paleta = x_paleta + 1; }
	}
	cerrar(); 
	kill(&hiloPosiciones); 
	kill(&hiloMoverPelota); 
	return 0;
}
