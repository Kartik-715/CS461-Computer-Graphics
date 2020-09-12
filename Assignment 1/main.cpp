#include <bits/stdc++.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


using namespace std ; 

class obj3dmodel
{
   struct vertex
   {
        double x;
        double y;
        double z;
   };
   struct face
   {
       unsigned int v1,v2,v3;
   };

   vector<vertex> vertexes;
   vector<face> faces;

public:
  void readfile(const char* filename);
  void draw();
};

void obj3dmodel::readfile(const char * filename) 
{
   string s;
   ifstream fin(filename);

   if(!fin)
         return;
         
   while(fin >> s)
   {
         switch(*s.c_str())
         {
            case 'v':
            {
                vertex v;
                fin>>v.x>>v.y>>v.z;
                this->vertexes.push_back(v);
            }
            break;            
            case 'f':
            {
                face f;
                fin >> f.v1 >> f.v2 >> f.v3 ;
                faces.push_back(f);
            }
            break;
        }
   }     
}

float _angle = 0 ;
float _cameraAngle = 30.0 ; 

void update(int value)
{
	_angle += 2.0f;
	if(_angle > 360.f)
	{
	    _angle-=360;
	}
	glutPostRedisplay();
	glutTimerFunc(25,update,0);
}

void obj3dmodel::draw()
{     
    glClearColor (0.0,0.0,0.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(_cameraAngle,0.0f,0.5f,0.5f); //rotate object by 30 degree with respect to y-axis
    glTranslatef(0.0f, 0.0f, -10.0f);

    glPushMatrix();
    glTranslatef(5.0f, -1.0f, 0.0f);
    glScalef(7.0f,7.0f,7.0f);
    glRotatef(_angle,1.0f,3.0f,2.0f); //rotating object continuously by 2 degree

    glBegin(GL_TRIANGLES) ;
    glColor3d(1,1,1) ;  

    for(int i=0 ; i<faces.size() ; i++)
    {                         
        vertex v1 = vertexes[faces[i].v1-1];
        vertex v2 = vertexes[faces[i].v2-1];
        vertex v3 = vertexes[faces[i].v3-1];

        glVertex3d(v1.x,v1.y,v1.z);
        glVertex3d(v2.x,v2.y,v2.z);
        glVertex3d(v3.x,v3.y,v3.z);
    }

    glEnd() ; 
    glFlush() ; 
    glPopMatrix();
    glutSwapBuffers();
 }

obj3dmodel m_model ; 

void displayModel()
{
      m_model.draw() ;    
}

void handleResize(int w,int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(double)w/(double)h,1.0,200.0);
}

int main(int argc, char ** argv)
{
    glutInit(&argc, argv) ;
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(80,80) ;
    glutInitWindowSize(600,600) ; 

    glutCreateWindow("Model") ; 
    glEnable(GL_DEPTH_TEST) ; 

    m_model.readfile("lowpolybunny.obj") ; 
    glutDisplayFunc(displayModel) ;
    glutReshapeFunc(handleResize) ; 
    glutTimerFunc(25,update,0);

    glutMainLoop() ; 

}