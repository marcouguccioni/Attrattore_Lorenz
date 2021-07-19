#include "/home/marco/programmazione/passe_par_tout.h"
#define N 100000
using namespace std;

static int angolo1=0,angolo2=0,distanza=0;
static double punti [N][3], punti2 [N][3];
static int conteggio=0,incremento=0;

void display() 
{
 glClear(GL_COLOR_BUFFER_BIT); 
 glPushMatrix();
 glRotatef((GLfloat)angolo1, 1.0, 0.0, 0.0);//ruota rispetto x
 glRotatef((GLfloat)angolo2, 0.0, 1.0, 0.0);//ruota rispetto y
 glTranslatef((GLfloat) distanza,0.0,0.0); //trasla lungo x
 
 glColor3f(1,0,0); //rosso
 glBegin(GL_LINE_STRIP); 
 for(int i=0 ; i<conteggio && i<N ; ++i) //particella 1
   glVertex3dv(punti[i]);
 glEnd();
 
 glColor3f(0,0,1); //blu
 glBegin(GL_LINE_STRIP); 
 for(int i=0 ; i<conteggio && i<N ; ++i) //particella 2
   glVertex3dv(punti2[i]); 
 glEnd();

 if(conteggio<N) 
 {
  if( conteggio + incremento > N ) conteggio=N;
  else conteggio+=incremento; 
 }
 
 glColor3f(1,1,1); // bianco
 glBegin(GL_LINES); //linea
   glVertex3d(0,0,0); 
   glVertex3d(50,0,0);//asse x
   glVertex3d(0,0,0);
   glVertex3d(0,50,0);//asse y
   glVertex3d(0,0,0);
   glVertex3d(0,0,50); //asse z
 glEnd();

 glRasterPos3d(50,0,0);
 glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'X'); //asse x
 glRasterPos3d(0,50,0);
 glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Y'); //asse y
 glRasterPos3d(0,0,50);
 glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Z'); //asse z
 
 glFlush(); 
 glutSwapBuffers(); //senza non fa il disegno
 glPopMatrix();
}

void rimodella_finestra(int w, int h) 
{
  GLfloat j = (GLfloat) h / (GLfloat) w;
  glViewport(0, 0, (GLint) w, (GLint) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-5.0, 5.0, -j*2, j*2, 1.0, 300.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -60.0);
}

void tastiera(unsigned char tasto,int x,int y) 
{
 if(tasto=='f') conteggio=N;
 if(tasto=='r') conteggio=0;
 if(tasto=='a') distanza+=5;
 if(tasto=='s') distanza-=5;
 if(tasto=='v') if(incremento > 0) incremento+=5;
 if(tasto=='c') if( incremento-5 >0) incremento-=5;
 if(tasto=='i') {angolo1=0,angolo2=0,distanza=0;}
 if(tasto==27) exit(0);
 if(tasto==13) incremento=10;
}

void tastiera2(int tasto,int x,int y)
{
 if(tasto==GLUT_KEY_LEFT) angolo2+=5;
 if(tasto==GLUT_KEY_RIGHT) angolo2-=5;
 if(tasto==GLUT_KEY_UP) angolo1-=5;
 if(tasto==GLUT_KEY_DOWN) angolo1+=5;
}

void animazione() 
{
 glutPostRedisplay();
}

void LORENZ(double *x,double *x0,double t,int &conta) 
//ATTRATTORE DI LORENZ
{
 const double a=10.0,b=8.0/3.0,c=28.0;
 double f0[3];
 double Y[3];
 double f1[3];
 double Z[3];
 double f2[3];
 double W[3];
 double f3[3];
 double h;
 h=t/(double)N; 
 while(h>1) {h/=10;}

 for(int i=0 ; i<N ; ++i)
  { //RUNGE-KUTTA

     //equazioni di Lorenz calcolate in x0
     f0[0] = a*(x0[1]-x0[0]),
     f0[1] = c*x0[0]-x0[0]*x0[2]-x0[1],
     f0[2] = x0[0]*x0[1]-b*x0[2];

    //campo ausiliario Y
     for(int i=0 ; i<3 ; ++i) Y[i]=x0[i]+(h/2)*f0[i]; 

    //equazioni di Lorenz calcolate in Y
     f1[0] = a*(Y[1]-Y[0]),
     f1[1] = c*Y[0]-Y[0]*Y[2]-Y[1],
     f1[2] = Y[0]*Y[1]-b*Y[2];

    //campo ausiliario Z
     for(int i=0 ; i<3 ; ++i) Z[i]=x0[i]+(h/2)*f1[i];

    //equazioni di Lorenz calcolate in Z
     f2[0] = a*(Z[1]-Z[0]),
     f2[1] = c*Z[0]-Z[0]*Z[2]-Z[1],
     f2[2] = Z[0]*Z[1]-b*Z[2];

    //campo ausiliario W
     for(int i=0 ; i<3 ; ++i) W[i]=x0[i]+h*f2[i];

    //equazioni di Lorenz calcolate in W
     f3[0] = a*(W[1]-W[0]),
     f3[1] = c*W[0]-W[0]*W[2]-W[1],
     f3[2] = W[0]*W[1]-b*W[2];

    //ALGORITMO DI RUNGE KUTTA
     for(int i=0 ; i<3 ; ++i) 
       x[i]=x0[i]+(h/6)*(f0[i]+2*f1[i]+2*f2[i]+f3[i]);

    /*cout<<setprecision(5)<<fixed<<setw(11)<<x[0]
    <<setw(11)<<x[1]
    <<setw(11)<<x[2]<<endl;*/

    if(conta)
    { 
      for(int t=0; t<3; ++t) {punti2[i][t]=x0[t];x0[t]=x[t];}
    }
    else
    {
      for(int t=0; t<3; ++t) {punti[i][t]=x0[t];x0[t]=x[t];}
    }

  }//fine ciclo runge-kutta
  ++conta;
}

int main(int arg1,char **arg2)
{
  double x0[3],x[3],t;
  int conta=0;
  char c;
  
  cout<<"Questo programma, mostra graficamente l'attrattore di Lorenz per 2 punti nello spazio cartesiano tridimensionale, attraverso l'algoritmo di Runge-Kutta.\nIl programma ignora ulteriori dati inseriti in input, successivi ai parametri richiesti.\n"<<endl;

cout<<"Inserire l'incremento della variabile indipendente t: esso determina graficamente la lunghezza delle curve individuate dai punti.\nNOTA: per apprezzare \"l'effetto farfalla\" si consiglia un valore di t>50.\nNon inserire valori inferiori a t=1 o superiori a t=200." <<endl;
  while(true)
  {
   cin>>t; 
   if(!cin) 
    {
     cin . clear( ); 
     cout<<"parametro non valido,riprova:\n";
     do; while(c=cin.get() != '\n'); 
    }
   else break;
  }

  if(t==0) {cout<<"incremento nullo,nessuna evoluzione del sistema.\n";return 0;}
  if(t<0) {cout<<"incremento non valido,fornito un valore negativo.\n"; return 1;}
  if(t<1) {cout<<"incremento troppo piccolo per individuare un'evoluzione significativa.\n"; return 2;}
  if(t>200) {cout<<"valore troppo elevato.\n"; return 3;}
  cout<<"valore di t inserito: "<<setprecision(10)<<t<<"\n"<<endl;
  
  //utilizzata per eliminare quello che si scrive dopo
  do c=cin.get(); while(c!='\n');
  
  cout<<"Inserire le coordinate iniziali del primo punto (ROSSO) nel sistema di riferimento X,Y,Z. \nATTENZIONE: il programma accetta solo valori nell'intervallo reale [-100,100]\nNOTA: inserendo valori nulli lungo le coordinate x e y (0,0,z),non si avrÃ  alcuna evoluzione significativa."<<endl;
  while(true)
 {
   for(double&i : x0) 
   {
    cin>>i; //condizioni iniziali particella rossa
    if(i>100 || i<-100) {cout<<"valore fuori range\n"; return 4;}
   }  
   if(!cin) 
    {
     cin . clear( ); 
     cout<<"terna non valida,riprovare:\n";
     do; while(c=cin.get() != '\n'); 
    }
    else break;
  }
  if(x0[0]==0. && x0[1]==0. ) cout<<"nessuna evoluzione significativa per il punto rosso."<<endl;
  cout<<"coordinate punto rosso inserite: ("<<setprecision(10)<<x0[0]<<" , "<<x0[1]<<" , "<<x0[2]<<")\n"<<endl;
  
  do c=cin.get(); while (c!='\n');
  
  LORENZ(x,x0,t,conta); //particella rossa
 
  cout<<"Inserire le coordinate iniziali del secondo punto (BLU) rispettando le precedenti restrizioni:"<<endl;
  while(true)
  {
   for(double&i : x0) 
   {
    cin>>i; //coordinate inziali particella blu
    if(i>100 || i<-100) {cout<<"valore fuori range\n"; return 5;}
   }  
   if(!cin) 
    {
     cin . clear( ); 
     cout<<"terna non valida,riprovare:\n";
     do; while(c=cin.get() != '\n'); 
    }
    else break;
   }

  if(x0[0]==0. && x0[1]==0.) cout<<"nessuna evoluzione significativa per il punto blu."<<endl;
  cout<<"coordinate punto blu inserite: ("<<setprecision(10)<<x0[0]<<" , "<<x0[1]<<" , "<<x0[2]<<")\n"<<endl;

  do c=cin.get(); while (c!='\n');
 
  LORENZ(x,x0,t,conta); //particella blu
  
  cout<<endl;
  cout<<"PREMI INVIO PER INIZIARE.\n"<<endl;
  cout<<"CONTROLLO ANIMAZIONE: \n"<<endl;
  cout<<"RUOTA: frecce"<<endl;
  cout<<"TRASLA: 'a','s'"<<endl;
  cout<<"RICOMINCIA: 'r'"<<endl;
  cout<<"FINISCI: 'f'"<<endl;
  cout<<"VELOCIZZA: 'v'"<<endl;
  cout<<"RALLENTA: 'c'"<<endl;
  cout<<"RIPOSIZIONA: 'i'\n"<<endl;

  cout<<"Per terminare il programma:\npremere esc posizionandosi sulla finestra."<<endl;
 
  glutInit(&arg1,arg2);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowPosition(500,500);
  glutInitWindowSize(700, 700); 
  glutCreateWindow("Attrattore di Lorenz");
  glutDisplayFunc(display);
  glutReshapeFunc(rimodella_finestra);
  glutKeyboardFunc(tastiera);
  glutSpecialFunc(tastiera2);
  glutIdleFunc(animazione);
  glutMainLoop(); 
  return 0;
}

//compilare con  ccg LORENZ.C  glut /usr/lib/x86_64-linux-gnu 
//eseguire con ./LORENZ
//ccg è il compilatore di passepartout e vicino a LORENZ.C c'è la path del percorso di GLUT.








