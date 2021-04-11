// Implementation of sphere displying with OpenGL and GLUT.
//
// Main source of inspiration is
// http://www.songho.ca/opengl

#include <GLUT/glut.h>

#include <array>
#include <iostream>
#include <sstream>
#include <string_view>

using Color = std::array<float, 4>;

constexpr float CAMERA_DISTANCE = 1.7f;
constexpr int SCREEN_HEIGHT = 600;
constexpr int SCREEN_WIDTH = 800;

void DrawString(const std::string_view str, int x, int y, const Color& color,
                void* font) {
  glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_COLOR);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);

  glColor4fv(color.data());
  glRasterPos2i(x, y);

  for (auto i = 0; i < str.size(); ++i) {
    glutBitmapCharacter(font, str[i]);
  }

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glPopAttrib();
}

void TimerFn(int millisec) {
  glutTimerFunc(millisec, TimerFn, millisec);
  glutPostRedisplay();
}

void ShowInfo() {
  // Backup curren ModelView
  glPushMatrix();
  glLoadIdentity();

  // set to 2d orthogonal projections
  glMatrixMode(GL_PROJECTION_MATRIX);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

  std::stringstream ss;
  ss << "First line";
  DrawString(ss.str(), 15, SCREEN_HEIGHT - 20, {1, 1, 1, 1},
             GLUT_BITMAP_8_BY_13);

  // restore projection matrix
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void DisplayFn() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // Save the initial ModelView matrix before modifing it.
  glPushMatrix();

  // transform camera
  glTranslatef(0, 0, -CAMERA_DISTANCE);
  glRotatef(0.f, 1.f, 0.f, 0.f);
  glRotatef(0.f, 0.f, 1.f, 0.f);

  ShowInfo();

  glPopMatrix();

  glutSwapBuffers();
}

/* initialize GLUT. */
int InitializeGLUT(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  glutInitWindowPosition(100, 100);

  // create a window with OpenGL context
  // Window will not displayed until glutMainLoop is called.
  // returns unique ID.
  int handle = glutCreateWindow(argv[0]);

  glutDisplayFunc(DisplayFn);
  // Redraw only given milliseconds
  glutTimerFunc(33, TimerFn, 33);

  return handle;
}

// initialize OpenGL
void InitGL() {
  glShadeModel(GLU_SMOOTH);
  glClearColor(0, 0, 0, 0);
}

int main(int argc, char** argv) {
  InitializeGLUT(argc, argv);
  InitGL();

  glutMainLoop();

  return 0;
}
