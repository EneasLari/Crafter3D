//Tzagiollari Enea A.M 2367---Rentas Nikolaos A.M 2343
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define PI 3.141592654f
#define CUBESIZE 6
int  N=10;//Scene size

float cRadius = 2.0f; // our radius distance from our character
int limpmove=0;//the degrees of the arms and legs,initially straight down
int handi=20;//the amount of incresement of degres of the arms and legs
float xpos = -1, ypos = -1, zpos=-1; //initial position of the camera or player
float xrot = 30;/* is the rotation on the x-axis(up and down) */ 
float yrot = 0;/*yrot is the rotation on the y-axis(left and right)*/ 
float angle=0.0; //angle of rotation
int *SceneArray;//an 3d array that stores 0 or 1 0 for no cube or 1 for a random cube
float lastx, lasty;//for mouse movement
int ***cubetype;//plegma pou arxikopiei kai kanei update ton scene;
int nextxz[2];//epomeni thesi kubou sto idio epipedo
bool thirdperson=false;
int score=50;
int lifes=3;
int storedcubes=0;
char playerInfo[50];

void readSceneFile(char *file);//reads from a file 0 or 1 for building the levels
bool updateCube();//func for updating the color of cube if the player takes one cube
void renderString(float x, float y, char *string);//render a string on the specifix xy on the screen
void cube (float x,float y,float z,int type);//draw a cube
void Scene(int N);//builds the scene with N size
void colapse(void);//colapses the cubes that are flying
void init (int size);//makes the first initializations
void enable (void);//enables opengl funcionalities
void renderACharacter();//render the player in 3d person
void thirdPerson(void);//render the 3d person on the scene
void camera (void);
void display (void);
void reshape (int w, int h);//reshape the scene if we change the resolution of window
void nextXZ();//calculates the next postion cordinates of player
void keyboard(unsigned char key, int x, int y);     
void mouseButtonHandler(int button, int state, int x, int y);
void mouseMovement(int x, int y);



void readSceneFile(char *file)
{
    FILE *myFile;
    myFile = fopen(file, "r");
    int num=N*N*N;
    SceneArray=(int*) malloc(sizeof(int)*num);
    int i;

    if (myFile == NULL)
    {
        printf("Error Reading File\n");
        exit (0);
    }
    for (i = 0; i < num; i++)
    {
        fscanf(myFile, "%d,", &SceneArray[i] );

    }
    fclose(myFile);

}


void cube (float x,float y,float z,int type) {
    if(type==0)//magenta
    {        
            glPushMatrix();
            glTranslated(x,y,z); //translate the cube
            glColor3f(0.0f, 0.0f, 0.0f);
            glutWireCube(CUBESIZE);
            glColor3f(1.0f, 0.0f, 1.0f);

            glutSolidCube(CUBESIZE); //draw the cube
            glPopMatrix();
    }
    if(type==1)//red
    {
    		glPushMatrix();
    		glTranslated(x,y,z); //translate the cube
            glColor3f(0.0f, 0.0f, 0.0f);
            glutWireCube(CUBESIZE);
    		glColor3f(1.0f, 0.0f, 0.0f);
    		glutSolidCube(CUBESIZE); //draw the cube
    		glPopMatrix();
    }
    if(type==2)//blue
    {
    		glPushMatrix();
    		glTranslated(x,y,z); //translate the cube
            glColor3f(0.0f, 0.0f, 0.0f);
            glutWireCube(CUBESIZE);
    		glColor3f(0.0f, 0.0f, 1.0f);
    		glutSolidCube(CUBESIZE); //draw the cube
    		glPopMatrix();
    }
    if(type==3)//green
    {
    		glPushMatrix();
    		glTranslated(x,y,z); //translate the cube
            glColor3f(0.0f, 0.0f, 0.0f);
            glutWireCube(CUBESIZE);
    		glColor3f(0.0f, 1.0f, 0.0f);
    		glutSolidCube(CUBESIZE); //draw the cube
    		glPopMatrix();
    }
    if(type==4)//yellow
    {
    		glPushMatrix();
    		glTranslated(x,y,z); //translate the cube
            glColor3f(0.0f, 0.0f, 0.0f);
            glutWireCube(CUBESIZE);
    		glColor3f(1.0f, 1.0f, 0.0f);
    		glutSolidCube(CUBESIZE); //draw the cube
    		glPopMatrix();
    }
}
void randomizeCubes(int size)
{
	cubetype=(int***) malloc(sizeof(int**)*size);
	for (int i=0;i<size;i++)
	{
		cubetype[i]=(int**) malloc(sizeof(int*)*size);
	}
	for (int j=0;j<size ;j++)
	{
		for(int k=0;k<size;k++)
		{
			cubetype[j][k]=(int*) malloc(sizeof(int)*size);
		}
	}
	int tnum;
    int l=0;
	for (int y=0;y<size ;y++)
	{
		for (int x=0;x<size;x++)
		{

			for (int z=0;z<size ;z++)
			{   
                    
				    tnum=SceneArray[l];
                    if(tnum==1){
                        int r=(int)(rand() % (4 + 1 - 1) + 1);
                        cubetype[x][y][z]=r;
                        l++; 
                    }
                    else
                    {
                        cubetype[x][y][z]=-1;
                        l++;
                    }

			}
		}
	}
    cubetype[size/2][0][size/2]=0;
	
}
void Scene(int N)
{
	for (int x=0;x<N;x++)
	{
		for (int y=0;y<N;y++)
		{

			for (int z=0;z<N ;z++)
			{
				cube(x*CUBESIZE,y*CUBESIZE,z*CUBESIZE,cubetype[x][y][z]);
				
			}
		}
	}
}

void colapse(void)
{
    bool added=false;
    for(int i=0;i<N-1;i++){
        for (int y=N-1;y>1 ;y--)
        {
            for (int x=0;x<N;x++)
            {
                for (int z=0;z<N;z++)
                {   
                    if(cubetype[x][y-1][z]<0 && cubetype[x][y][z]>0){

                        cubetype[x][y-1][z]=cubetype[x][y][z];

                        cubetype[x][y][z]=-1;
                        if(added==false){score+=10;added=true;}
                    }
                }
            }
        }
    }
    
    
}

void init (int size) {
    N=size;
    xpos=(N/2)*CUBESIZE;
    ypos=CUBESIZE;
    zpos=(N/2)*CUBESIZE;
    randomizeCubes(size);
    
}

void enable (void) {
    glEnable (GL_DEPTH_TEST); //enable the depth testing
    glEnable (GL_LIGHTING); //enable the lighting
    glEnable (GL_LIGHT0); //enable LIGHT0, our Diffuse Light
    glEnable (GL_COLOR_MATERIAL);
    glShadeModel (GL_SMOOTH); //set the shader to smooth shader

}
void renderACharacter()
{
    glColor3ub((GLubyte) 244, (GLubyte)164, (GLubyte)96);
    glPushMatrix();
    glTranslatef(0.0f,0.9f,0.0f);
    glutSolidCube(0.5);
    //glutSolidSphere(0.4, 50, 50);
    glRotatef(90,1.0f, 0.0f, 0.0f);
    glutSolidCone(0.2,1.0,50,50);
    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.0f,0.0f,0.0f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.7f,0.0f,0.0f);
    glRotatef(limpmove, 1.0f, 0.0f, 0.0f); // Rotate our object around the y axis
    glScalef(0.7f, 2.0f, 0.7f);
    glutSolidCube(0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.7f,0.0f,0.0f);
    glRotatef(-limpmove, 1.0f, 0.0f, 0.0f); // Rotate our object around the y axis
    glScalef(0.7f, 2.0f, 0.7f);
    glutSolidCube(0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.2f,-0.8f,0.0f);
    glRotatef(limpmove, 1.0f, 0.0f, 0.0f); // Rotate our object around the y axis
    glScalef(0.7f, 2.0f, 0.7f);
    glutSolidCube(0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.2f,-0.8f,0.0f);
    glRotatef(-limpmove, 1.0f, 0.0f, 0.0f); // Rotate our object around the y axis
    glScalef(0.7f, 2.0f, 0.7f);
    glutSolidCube(0.5);
    glPopMatrix();
}

void camera (void) {
    
        glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
        glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
        glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera

}
void thirdPerson(void)
{
    //glPushMatrix();
    glTranslatef(0.0f, -0.5f, -cRadius);
    glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
    glColor3f(1.0f, 0.0f, 0.0f);
    //glutSolidCube(1); //Our character to follow
    renderACharacter();
    //glPopMatrix();
}
void display (void) {
    glClearColor (0.0,0.0,0.0,1.0); //clear the screen to black
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
    enable();
    glLoadIdentity();
    
    if(thirdperson==true)
    {
        thirdPerson();
        camera();
    }else
    {
        camera();
    }  
    Scene(N);
    sprintf(playerInfo, "Score = %d Lifes = %d Cubes=%d", score,lifes,storedcubes);
    renderString(250, 50,playerInfo);
    glutSwapBuffers(); //swap the buffers
    
    angle++; //increase the angle
    
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
    glMatrixMode (GL_PROJECTION); //set the matrix to projection
    glLoadIdentity ();
    gluPerspective (90, (GLfloat)w / (GLfloat)h, 1.0, 1000.0); //set the perspective (angle of sight, width, height, ,depth)
    glMatrixMode (GL_MODELVIEW); //set the matrix back to model

}

void nextXZ()
{
    float yrotrad; 
    float dx,dz;
    yrotrad = (yrot / 180 * PI);
    dx = float(sin(yrotrad)) ;
    dz = -float(cos(yrotrad)) ;
    nextxz[0]=xpos;
    nextxz[1]=zpos;
    if (dx < 0.3 && dx > -0.3) 
    {
         nextxz[0] += 0;
    } 
    else if (dx < -0.3) 
    {
        nextxz[0] -= CUBESIZE;
    } 
    else if (dx > 0.3) 
    {
        nextxz[0] += CUBESIZE;
    }
    if (dz < 0.3 && dz > -0.3) 
    {
        nextxz[1] += 0;
    } 
    else if (dz < -0.3) 
    {
        nextxz[1] -= CUBESIZE;
    } 
    else if (dz > 0.3) 
    {
        nextxz[1] += CUBESIZE;
    }
    
}

void keyboard(unsigned char key, int x, int y)
{
    if (key=='w')
    {
        limpmove+=handi;
        if(limpmove>45 || limpmove<-30)
        {
            handi=handi*(-1);
        }
        //printf("xpos=%f zpos=%f\n",xpos,zpos );
        nextXZ();
        int i=nextxz[0]/CUBESIZE;
        int j=(int)ypos/CUBESIZE;
        int k=nextxz[1]/CUBESIZE;
        if(i>=0 && j>=0 && k>=0 && i<N && j<N && k<N){
            if(cubetype[i][j][k]<0)
            {
                xpos=nextxz[0];
                zpos=nextxz[1];
            
            }
            if(cubetype[i][j][k]>0 && cubetype[i][j+1][k]<0)
            {
                xpos=nextxz[0];
                zpos=nextxz[1];
                ypos=ypos+CUBESIZE;
                score+=5;
                if(j==N-1)//an eisai sto teleutaio epipedo pare mia zoi kai 100 pontous
                {
                	score=score+100;
                	lifes++;
                }
            }
            if(cubetype[i][j-1][k]<0)
            {
                int p=j-1;
                while(p>0 && cubetype[i][p][k]==-1 && score>=5)
                {
                    xpos=nextxz[0];
                    zpos=nextxz[1];
                    ypos=ypos-CUBESIZE;
                    p--;
                    score-=5;

                }
                
            }
        }
    }
    if (key=='q')//delete the cube in front of you
    {   
        nextXZ();
        int i=nextxz[0]/CUBESIZE;
        int j=(int)ypos/CUBESIZE;
        int k=nextxz[1]/CUBESIZE;
        if(i>=0 && j>=0 && k>=0 && i<N && j<N && k<N){
        if(cubetype[i][j][k]>0 ){
            
            cubetype[nextxz[0]/CUBESIZE][(int)ypos/CUBESIZE][nextxz[1]/CUBESIZE]=-1;
    }   }}
    
    if (key=='d')
    {
        yrot += 1;
        if (yrot >360) yrot =0;
        //printf("%f\n",yrot);
    }

    if (key=='a')
    {
        yrot -= 1;
        if (yrot <-360) yrot =0;
        //printf("%f\n",yrot);
    }
    if (key=='s')
    {
        xrot += 1;
        if (xrot >360) xrot -= 360;
    }

    if (key=='z')
    {
        xrot -= 1;
        if (xrot < -360) xrot += 360;
    }
      
    if(key=='r')
    {
        colapse();
    }
    if(key=='e')
    {   
        nextXZ();
        int i=nextxz[0]/CUBESIZE;
        int j=1;
        int k=nextxz[1]/CUBESIZE;
        if(i>=0 && j>=0 && k>=0 && i<N && j<N && k<N && score>=20){
            while(j<N){
                cubetype[i][j][k]=-1;
                j++;
            }
            score-=20;
            lifes++;   
        }
    }
    if (key==27)//esc
    {
        exit(0);
    }
    if(key==32)//spacebar
    {

        if(score>=5 && updateCube()==true)
        {
            storedcubes++;
            score-=5;
        }
    }
    if(key=='p')//change to third person
    {
        //printf("%s\n","thirdperson" );
        thirdperson=true;
    }
    if(key=='o')//change to first person
    {
        thirdperson=false;
    }


}
void mouseButtonHandler(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) 
        {
            nextXZ();
            int level=(int)ypos;
            int i=nextxz[0]/CUBESIZE;
            int k=nextxz[1]/CUBESIZE;
            if(i>=0 && k>=0 && i<N  && k<N && storedcubes>0){
                while(cubetype[i][level/CUBESIZE][k]>0 && level<(CUBESIZE*N-CUBESIZE))
                {

                    level+=CUBESIZE;

                }
                if(level<(CUBESIZE*N-CUBESIZE))
                {
                    int r=(int)(rand() % (4 + 1 - 1) + 1);
                    cubetype[i][level/CUBESIZE][k]=r;
                    storedcubes--;
                    score+=5;
                }   
            }

        }
    }
    if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
        	nextXZ();
        	int level=(int)ypos;
            int i=nextxz[0]/CUBESIZE;
            int k=nextxz[1]/CUBESIZE;
            if (i==N-1  && xpos<(N-1)*CUBESIZE || i==0 && xpos>0 || k==0 && zpos >0|| k==N-1 && zpos<(N-1)*CUBESIZE)
            {
                 if(nextxz[0]>xpos  && xpos>0)
                 {           
                 	cubetype[i][level/CUBESIZE][k]=-1;
                 }
                 if(nextxz[0]<xpos)
                 {
                 	cubetype[i][level/CUBESIZE][k]=-1;
                 }
                 if(nextxz[1]>zpos)
                 {
                 	cubetype[i][level/CUBESIZE][k]=-1;
                 }
                 if(nextxz[1]<zpos)
                 {
                 	cubetype[i][level/CUBESIZE][k]=-1;
                 }
            }
            else
            {
        		 if(nextxz[0]>xpos && i<N-1  && cubetype[i+1][level/CUBESIZE][k]<0)
                 {
                 	cubetype[i+1][level/CUBESIZE][k]=cubetype[i][level/CUBESIZE][k];
                 	cubetype[i][level/CUBESIZE][k]=-1;
                 }
                 if(nextxz[0]<xpos  && i>0 && cubetype[i-1][level/CUBESIZE][k]<0)
                 {
                 	cubetype[i-1][level/CUBESIZE][k]=cubetype[i][level/CUBESIZE][k];
                 	cubetype[i][level/CUBESIZE][k]=-1;
                 }
                 if(nextxz[1]>zpos && k<N-1  && cubetype[i][level/CUBESIZE][k+1]<0)
                 {
                 	cubetype[i][level/CUBESIZE][k+1]=cubetype[i][level/CUBESIZE][k];
                 	cubetype[i][level/CUBESIZE][k]=-1;
                 }
                 if(nextxz[1]<zpos && k>0  && cubetype[i][level/CUBESIZE][k-1]<0)
                 {
                 	cubetype[i][level/CUBESIZE][k-1]=cubetype[i][level/CUBESIZE][k];
                 	cubetype[i][level/CUBESIZE][k]=-1;
                 } 
            } 
        }
    }
}

void mouseMovement(int x, int y) {
    int diffx=x-lastx; //check the difference between the current x and the last x position
    int diffy=y-lasty; //check the difference between the current y and the last y position
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
   
    
    if (diffy <= 3 && diffy >= -3 )
        xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
    if (diffx <= 3 && diffx >= -3)
        yrot += (float) diffx;    //set the xrot to yrot with the addition of the difference in the x position
}

bool updateCube()
{
	bool done=false;
    int currentcube=cubetype[(int)xpos/CUBESIZE][(int)(ypos-1)/CUBESIZE][(int)zpos/CUBESIZE];
    if(currentcube==1){cubetype[(int)xpos/CUBESIZE][(int)(ypos-1)/CUBESIZE][(int)zpos/CUBESIZE]=4;done=true;}
    if(currentcube==3){cubetype[(int)xpos/CUBESIZE][(int)(ypos-1)/CUBESIZE][(int)zpos/CUBESIZE]=1;done=true;}
    if(currentcube==4){cubetype[(int)xpos/CUBESIZE][(int)(ypos-1)/CUBESIZE][(int)zpos/CUBESIZE]=2;done=true;}
    return done;
 
}


void renderString(float x, float y, char *string) {
    char *c;
    int x1=x;

    glMatrixMode(GL_PROJECTION);

    glPushMatrix();
    glLoadIdentity();
    glDisable( GL_DEPTH_TEST ) ; //disable the depth test so renders on top
    gluOrtho2D(0, 800, 800, 0);
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();

    glLoadIdentity();
    for (c=string; *c != '\0'; c++) {
        glRasterPos2f(x1,y);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        x1 = x1 + glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c) + 3;
    }
    glEnable( GL_DEPTH_TEST ) ; // Turn depth testing back on
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    
}


int main (int argc, char **argv) {
    readSceneFile("myScene.f");
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH); 
    glutInitWindowSize (1200, 800); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Crafter"); 
    init (N); 
    glutDisplayFunc (display); 
    glutIdleFunc (display); 
    glutReshapeFunc (reshape);

    //glutPassiveMotionFunc(mouseMovement); //check for mouse movement
    glutMouseFunc(mouseButtonHandler);

    glutKeyboardFunc (keyboard); 
    glutMainLoop (); 
    return 0;
}