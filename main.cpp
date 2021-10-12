/********************************************************/
/*                     cube.cpp                                                 */
/********************************************************/
/*                Affiche a l'ecran un cube en 3D                      */
/********************************************************/

/* inclusion des fichiers d'en-tete freeglut */

#ifdef __APPLE__
#include <GLUT/glut.h> /* Pour Mac OS X */
#else
#include <GL/glut.h>   /* Pour les autres systemes */
#endif
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>

/*class Point*/
class Point{
    public :
	//coordonnées x, y et z du point
	double x;
	double y;
	double z;
	// couleur r, v et b du point
	float r;
	float g;
	float b;
};
//Tableau pour stocker les sommets du cube et leur couleur
Point pCube[8]={
    {-0.5,-0.5, 0.5,1.0,0.0,0.0},
    { 0.5, -0.5, 0.5,0.0,1.0,0.0},
    { 0.5, -0.5, -0.5,0.0,0.0,1.0},
    { -0.5, -0.5, -0.5,1.0,1.0,1.0},
    { -0.5,0.5, 0.5,1.0,0.0,0.0},
    { 0.5, 0.5, 0.5,0.0,1.0,0.0},
    { 0.5, 0.5, -0.5,0.0,0.0,1.0},
    { -0.5, 0.5, -0.5,1.0,1.0,1.0}};

//Tableau pour stocker les indices des sommets par face pour le cube
int fCube[6][4]={
  {0,3,2,1},
  {0,1,5,4},
  {1,2,6,5},
  {2,3,7,6},
  {0,4,7,3},
  {4,5,6,7}};

char presse;
int anglex,angley,x,y,xold,yold;
float r = 0.2;
float R = 1;
const int N=8;
Point tableauPoint[N];
Point tableauArret[N*N];

int faces[N*N][4];

static GLfloat theta[7] = { 0.0,0.0,0.0,0.0,0.0,0.0,0.0 };

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void tore();

void tore(){

    // Représentation des points
    for (int i = 0; i < N; i++)
    {
        float theta = ((float)i*2*M_PI)/N;
        tableauPoint[i].x=R*cos(theta);
        tableauPoint[i].y=0.0;
        tableauPoint[i].z=R*sin(theta);
    }
    //Représentation des points et faces
    for(int i=0; i<N; i++) {
        for (int j = 0; j < N; j++)
        {
            //Représentation des points du grand cercles pour chaque point
            float theta = ((float)j*2*M_PI)/N;
            tableauArret[i*N+j].x=tableauPoint[i].x+r*cos(theta)*tableauPoint[i].x;
            tableauArret[i*N+j].y=r*sin(theta);
            tableauArret[i*N+j].z=tableauPoint[i].z+r*cos(theta)*tableauPoint[i].z;

            //représentations des faces
            faces[i*N+j][0]=i*N+j;
            faces[i*N+j][1]=(((i+1)*N)%(N*N))+j;
            faces[i*N+j][2]=(((i+1)*N)%(N*N))+(j+1)%N;
            faces[i*N+j][3]=(i*N)+(j+1)%N;
        }
    }
}

int main(int argc,char **argv)
{
  /* initialisation de glut et creation
     de la fenetre */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(200,200);
  glutInitWindowSize(500,500);
  glutCreateWindow("cube");

  /* Initialisation d'OpenGL */
  glClearColor(0.0,0.0,0.0,0.0);
  glColor3f(1.0,1.0,1.0);
  glPointSize(2.0);
  glEnable(GL_DEPTH_TEST);

  /* enregistrement des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);

  glMatrixMode( GL_PROJECTION );
     glLoadIdentity();
   gluPerspective(60 ,1,.1,30.);



  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}


void affichage()
{
  int i,j;
  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glShadeModel(GL_SMOOTH);
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  glTranslatef(0.,0.,-5.);
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);
  gluLookAt(0,5,-7,0.0,1.25,0.0,0.0,1.0,0.0);

    // Dessin du cube
    for (i=0;i<N;i++)
    {
        glBegin(GL_LINES);
            glColor3f(1.0,0.0,0.0);
            glVertex3f(tableauPoint[i].x , tableauPoint[i].y , tableauPoint[i].z);
            glVertex3f(tableauPoint[(i+1)%N].x , tableauPoint[(i+1)%N].y , tableauPoint[(i+1)%N].z);
        glEnd();
    }
      /*glBegin(GL_POLYGON);
      for (j=0;j<4;j++){
          glColor3f(pCube[fCube[i][j]].r,pCube[fCube[i][j]].g,pCube[fCube[i][j]].b);
          glVertex3f(pCube[fCube[i][j]].x,pCube[fCube[i][j]].y,pCube[fCube[i][j]].z);
      }
      glEnd();*/
    for (j=0;j<N*N;j++)
    {
        glBegin(GL_QUADS);
            glColor3f(1.0,1.0,0.0);
            glVertex3f(tableauArret[faces[j][0]].x , tableauArret[faces[j][0]].y , tableauArret[faces[j][0]].z);
            glVertex3f(tableauArret[faces[j][1]].x , tableauArret[faces[j][1]].y , tableauArret[faces[j][1]].z);
            glVertex3f(tableauArret[faces[j][2]].x , tableauArret[faces[j][2]].y , tableauArret[faces[j][2]].z);
            glVertex3f(tableauArret[faces[j][3]].x , tableauArret[faces[j][3]].y , tableauArret[faces[j][3]].z);
        glEnd();
    }

    //Repère
    //axe x en rouge
    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(1, 0,0.0);
    glEnd();
    //axe des y en vert
    glBegin(GL_LINES);
    	glColor3f(0.0,1.0,0.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 1,0.0);
    glEnd();
    //axe des z en bleu
    glBegin(GL_LINES);
    	glColor3f(0.0,0.0,1.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 0,1.0);
    glEnd();

  glFlush();

  //On echange les buffers
  glutSwapBuffers();
}

void clavier(unsigned char touche,int x,int y)
{
  switch (touche)
    {
    case 'p': /* affichage du carre plein */
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glutPostRedisplay();
      break;
    case 'f': /* affichage en mode fil de fer */
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glutPostRedisplay();
      break;
    case 's' : /* Affichage en mode sommets seuls */
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      glutPostRedisplay();
      break;
    case 'd':
      glEnable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'D':
      glDisable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'q' : /*la touche 'q' permet de quitter le programme */
      exit(0);
    }
}



void reshape(int x,int y)
{
  if (x<y)
    glViewport(0,(y-x)/2,x,x);
  else
    glViewport((x-y)/2,0,y,y);
}

void mouse(int button, int state,int x,int y)
{
  /* si on appuie sur le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    presse = 1; /* le booleen presse passe a 1 (vrai) */
    xold = x; /* on sauvegarde la position de la souris */
    yold=y;
  }
  /* si on relache le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    presse=0; /* le booleen presse passe a 0 (faux) */
}

void mousemotion(int x,int y)
  {
    if (presse) /* si le bouton gauche est presse */
    {
      /* on modifie les angles de rotation de l'objet
	 en fonction de la position actuelle de la souris et de la derniere
	 position sauvegardee */
      anglex=anglex+(x-xold);
      angley=angley+(y-yold);
      glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }

    xold=x; /* sauvegarde des valeurs courante de le position de la souris */
    yold=y;
  }
