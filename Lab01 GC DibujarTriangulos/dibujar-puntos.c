//	Program developed by
//
//	Informatika Fakultatea
//	Euskal Herriko Unibertsitatea
//	http://www.ehu.eus/if
//
// to compile it: gcc dibujar-puntos.c -lGL -lGLU -lglut
//
// This example shows :the use of GL_POINTS
//


#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

// texturaren informazioa

extern void load_ppm(char *file, unsigned char **bufferptr, int *dimxptr, int * dimyptr);
unsigned char *bufferra;
int dimx,dimy;

int num_triangles;
hiruki *triangulosptr;
int indice;

unsigned char * color_textura(float u, float v)
{
/* debe devolver un puntero al pixel adecuado, no al primero!! */
	int columna, fila, posicion;
	columna = u * dimx;
	fila = (1-v)*dimy;
	//fila = dimy - v*dimy;
	posicion = 3*dimx*fila+3*columna;

return(bufferra + posicion);
}


void dibujar_segmento(punto *izquierda, punto *derecha, float j){

	float i = izquierda->x;
	float v = izquierda->v;
	float u = izquierda->u;
	unsigned char* colorv;
	unsigned char r, g, b;

	float variacion_u = (derecha->u - izquierda->u)/(derecha->x - izquierda->x);

	while (i < derecha->x){
		colorv = color_textura(u, v);
		r = colorv[0];
		g = colorv[1];
		b = colorv[2];
		u += variacion_u;
		glBegin( GL_POINTS );
		glColor3ub(r,g,b);
		glVertex3f(i, j ,0.);
		glEnd();
		i++;
	}
}



void dibujar_triangulo(hiruki *tptr){
	punto *supptr, *infptr, *medptr;
	punto *corte_sup_med = malloc(sizeof(punto));
	punto *corte_sup_inf = malloc(sizeof(punto));
	punto *corte_med_inf = malloc(sizeof(punto));
	float pendiente_superior_inferior, pendiente_superior_medio, pendiente_medio_inferior;
	float variacionU_superior_inferior, variacionU_superior_medio, variacionU_medio_inferior;
	float variacionV_superior_inferior, variacionV_superior_medio, variacionV_medio_inferior;
	float j;

	if (tptr->p1.y > tptr->p2.y){
		supptr = &(tptr->p1);
		medptr = &(tptr->p2);
	} else {
		supptr = &(tptr->p2);
		medptr = &(tptr->p1);
	}

	if(tptr->p3.y < medptr->y) infptr = &(tptr->p3);
	else
		infptr = medptr;

	if(supptr->y > tptr->p3.y) medptr = &(tptr->p3);
	else{
		medptr = supptr;
		supptr = &(tptr->p3);
	}
	//hallados punto superior, medio e inferior


	pendiente_superior_medio = (medptr->x - supptr->x) / (supptr->y - medptr->y);
	pendiente_superior_inferior = (infptr->x - supptr->x) / (supptr->y - infptr->y);
	pendiente_medio_inferior = (infptr->x - medptr->x) / (medptr->y - infptr->y);

	variacionU_superior_medio = (medptr->u - supptr->u) / (supptr->y - medptr->y);
	variacionU_superior_inferior = (infptr->u - supptr->u) / (supptr->y - infptr->y);
	variacionU_medio_inferior = (infptr->u - medptr->u) / (medptr->y - infptr->y);

	variacionV_superior_medio = 1.0/((medptr->y - supptr->y) / (medptr->v - supptr->v));
	variacionV_superior_inferior = 1.0/((infptr->y - supptr->y) / (infptr->v - supptr->v));
	variacionV_medio_inferior = 1.0/((infptr->y - medptr->y) / (infptr->v - medptr->v));

	//halladas variaciones y pendientes

	corte_sup_med->x = supptr->x;
	corte_sup_med->y = supptr->y;
	corte_sup_med->u = supptr->u;
	corte_sup_med->v = supptr->v;

	
	corte_sup_inf->x = supptr->x;
	corte_sup_inf->y = supptr->y;
	corte_sup_inf->u = supptr->u;
	corte_sup_inf->v = supptr->v;

	
	corte_med_inf->x = medptr->x;
	corte_med_inf->y = medptr->y;
	corte_med_inf->u = medptr->u;
	corte_med_inf->v = medptr->v;


	//x = columna, y = fila

	j = supptr->y;
	while (j > medptr->y){
		corte_sup_med->x += pendiente_superior_medio;
		corte_sup_inf->x += pendiente_superior_inferior;

		corte_sup_med->u += variacionU_superior_medio;
		corte_sup_inf->u += variacionU_superior_inferior;

		corte_sup_med->v -= variacionV_superior_medio;
		corte_sup_inf->v -= variacionV_superior_inferior;

		if (corte_sup_med->x < corte_sup_inf->x){
			dibujar_segmento(corte_sup_med, corte_sup_inf, j);
		} else {
			dibujar_segmento(corte_sup_inf, corte_sup_med, j);
		}
		j--;
	}
	glFlush();


	j = medptr->y;
	while (j > infptr->y){
		corte_med_inf->x += pendiente_medio_inferior;
		corte_sup_inf->x += pendiente_superior_inferior;

		corte_med_inf->u += variacionU_medio_inferior;
		corte_sup_inf->u += variacionU_superior_inferior;

		corte_med_inf->v -= variacionV_medio_inferior;
		corte_sup_inf->v -= variacionV_superior_inferior;

		if (corte_med_inf->x < corte_sup_inf->x){
			dibujar_segmento(corte_med_inf, corte_sup_inf, j);
		} else {
			dibujar_segmento(corte_sup_inf, corte_med_inf, j);
		}
		j--;
	}
	glFlush();

}



static void marraztu(void)
{
float u,v;
float i,j;
unsigned char* colorv;
unsigned char r,g,b;

// borramos lo que haya...
glClear( GL_COLOR_BUFFER_BIT );

glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(0.0, 500.0, 0.0, 500.0,-250.0, 250.0);

dibujar_triangulo(triangulosptr+indice);

}
// This function will be called whenever the user pushes one key
static void teklatua (unsigned char key, int x, int y)
{
switch(key)
	{
	case 13:
		printf ("ENTER: que hay que dibujar el siguiente triángulo.\n");
		indice++;
		if (indice == num_triangles){
			indice = 0;
		}
		break;
	case 27:  // <ESC>
		exit( 0 );
		break;
	default:
		printf("%d %c\n", key, key );
	}

// The screen must be drawn to show the new triangle
glutPostRedisplay();
}

int main(int argc, char** argv)
{


	printf("This program draws points in the viewport \n");
	printf("Press <ESC> to finish\n");
	glutInit(&argc,argv);
	glutInitDisplayMode ( GLUT_RGB );
	glutInitWindowSize ( 500, 500 );
	glutInitWindowPosition ( 100, 100 );
	glutCreateWindow ( "GL_POINTS" );

	glutDisplayFunc(marraztu);
	glutKeyboardFunc(teklatua);
	/* we put the information of the texture in the buffer pointed by bufferra. The dimensions of the texture are loaded into dimx and dimy */
    load_ppm("foto.ppm", &bufferra, &dimx, &dimy);
    /***** lee la información de los triángulos... pero hay que definir las variables!*/
    cargar_triangulos(&num_triangles, &triangulosptr);
       
	glClearColor( 0.0f, 0.0f, 0.7f, 1.0f );

	glutMainLoop();

	return 0;
}
