#include "GLWidget.h"

static void qNormalizeAngle(int &angle) {
  while (angle < 0)
    angle += 360 * 16;
  while (angle > 360 * 16)
    angle -= 360 * 16;
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
  xRotate = yRotate = zRotate = 0;
  xTranslate = yTranslate = 0.;
  qtPurple = QColor::fromCmykF(.15, .0, .15, .0);

  RegionDraw = new CRegionDraw(10, 5, 3);
  Region = new CRegion(10, 5, 3);

  ScaleValue = SceneFactor = 1.;
  QObject::connect(Region, SIGNAL(geometryChanged(int, int, int)), RegionDraw,
                   SLOT(setRegionGeometry(int, int, int)));
  QObject::connect(Region, SIGNAL(redrawColumn(int, bool)), RegionDraw,
                   SLOT(redrawColumn(int, bool)));
}

GLWidget::~GLWidget() {}

QSize GLWidget::minimumSizeHint() const { return QSize(50, 50); }

QSize GLWidget::sizeHint() const { return QSize(400, 400); }

void GLWidget::setXTranslate(const double &x) {
  xTranslate += x;
  updateGL();
}

void GLWidget::setYTranslate(const double &y) {
  yTranslate += y;
  updateGL();
}

void GLWidget::setXRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != xRotate) {
    xRotate = angle;
    emit xRotationChanged(angle);
    updateGL();
  }
}

void GLWidget::setYRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != yRotate) {
    yRotate = angle;
    emit yRotationChanged(angle);
    updateGL();
  }
}

void GLWidget::setZRotation(int angle) {
  qNormalizeAngle(angle);
  if (angle != zRotate) {
    zRotate = angle;
    emit zRotationChanged(angle);
    updateGL();
  }
}

void GLWidget::setRegionGeometry(const int &width, const int &height,
                                 const int &columnHeight) {
  Region->setGeometry(width, height, columnHeight);
}

void GLWidget::setSceneFactor(const double &sceneFactor) {
  SceneFactor = sceneFactor;
  int side = qMax(width(), height());
  glViewport((width() - side) / 2, (height() - side) / 2, side, side);
  double Scene = SceneFactor * side;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-Scene, Scene, -Scene, Scene, -Scene, Scene);
  glMatrixMode(GL_MODELVIEW);
  updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  lastPos = event->pos();
  // int z = getObjectID(lastPos.x(),lastPos.y());
}

int GLWidget::getObjectID(const int &x, const int &y) {
  GLubyte pixel[3];

  GLdouble ray_x, ray_y, ray_z;
  GLint viewport[4];
  GLdouble proj[16];
  GLdouble modelview[16];

  // we query OpenGL for the necessary matrices etc.
  glGetIntegerv(GL_VIEWPORT, viewport);
  glGetDoublev(GL_PROJECTION_MATRIX, proj);
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

  // assuming you have mouse coordinates as mouseX and mouseY
  // gluUnproject assumes coordinates are measured from bottom of screen
  // so we invert the mouseY you got from glut or SDL or watever
  GLdouble _mouseY = viewport[3] - y;

  // using 1.0 for winZ gives u a ray
  gluUnProject(x, _mouseY, 1.0f, modelview, proj, viewport, &ray_x, &ray_y,
               &ray_z);
  // int r,g,b;
  // glGetIntegerv(GL_RED_BITS,&r);
  // glGetIntegerv(GL_GREEN_BITS,&g);
  // glGetIntegerv(GL_BLUE_BITS,&b);
  glReadPixels(x, y, 1., 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
  qDebug("%d %d %d", pixel[0], pixel[1], pixel[2]);
  /*
int viewport[4] = { 0 };
double  modelview[16] = { 0 },
      projection[16] = { 0 };
glGetIntegerv(GL_VIEWPORT,viewport);
glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
glGetDoublev(GL_PROJECTION_MATRIX,projection);
double sx,sy,sz;
//gluProject(x,y,1.,modelview,projection,viewport,&sx,&sy,&sz);
//qDebug("%f %f %f",sx,sy,sz);
glReadPixels(sx,sy,1,1,GL_RGB,GL_UNSIGNED_BYTE,pixel);
qDebug("%d %d %d",pixel[0],pixel[1],pixel[2]);
*/

  // qDebug("%d,%d,%d",pixel[0],pixel[1],pixel[2]);
  // int ObjectsFound = 0,
  //    ViewportCoords[4] = { 0 };
  // unsigned int SelectBuffer[32] = {0};
  // glSelectBuffer(32,SelectBuffer);
  // glGetIntegerv(GL_VIEWPORT,ViewportCoords);
  // glMatrixMode(GL_PROJECTION);
  // glPushMatrix();
  // glRenderMode(GL_SELECT);
  // glLoadIdentity();
  // gluPickMatrix(x,ViewportCoords[3] - y,2,2,ViewportCoords);
  ////gluPerspective(45.,1.,0.1,150);
  /////gluPerspective(45.0f,(float)g_rRect.right/(float)g_rRect.bottom,0.1f,150.0f);
  // glMatrixMode(GL_MODELVIEW);
  ////paintGL();
  //////RenderScene();
  // ObjectsFound = glRenderMode(GL_RENDER);
  // qDebug("%d",ObjectsFound);
  ////glMatrixMode(GL_PROJECTION);
  // glPopMatrix();
  // glMatrixMode(GL_MODELVIEW);

  ////if(objectsFound > 0){
  ////unsigned int lowestDepth = selectBuffer[1];
  ////int selectedObject = selectBuffer[3];

  ////for(int i = 1;i < objectsFound;i++)
  ////    if(selectBuffer[(i * 4) + 1] < lowestDepth){
  ////        lowestDepth = selectBuffer[(i * 4) + 1];
  ////        selectedObject = selectBuffer[(i * 4) + 3];
  ////        }
  ////    return selectedObject;
  ////    }
  ////    return 0;
  return 0;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - lastPos.x(), dy = event->y() - lastPos.y();
  if (event->buttons() & Qt::LeftButton) {
    setXRotation(xRotate + 8 * dy);
    setYRotation(yRotate + 8 * dx);
  } else if (event->buttons() & Qt::RightButton) {
    setXRotation(xRotate + 8 * dy);
    setZRotation(zRotate + 8 * dx);
  }
  lastPos = event->pos();
}

void GLWidget::initializeGL() {
  qglClearColor(qtPurple);
  glEnable(GL_DEPTH_TEST | GL_CULL_FACE | GL_LIGHTING | GL_LIGHT0 |
           GL_MULTISAMPLE | GL_BLEND);
  glShadeModel(GL_SMOOTH);
  static GLfloat lightPosition[4] = {-500., 500., 500.0, .7};
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(xTranslate, yTranslate, 0.);
  glRotatef(xRotate / 16.0, 1.0, 0.0, 0.0);
  glRotatef(yRotate / 16.0, 0.0, 1.0, 0.0);
  glRotatef(zRotate / 16.0, 0.0, 0.0, 1.0);

  glScalef(ScaleValue, ScaleValue, ScaleValue);
  RegionDraw->draw();
}

void GLWidget::resizeGL(int width, int height) {
  int side = qMax(width, height);

  glViewport((width - side) / 2, (height - side) / 2, side, side);
  double Scene = SceneFactor * side;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-Scene, Scene, -Scene, Scene, -Scene, Scene);
  glMatrixMode(GL_MODELVIEW);
}

void GLWidget::initRegion(const QBitArray &BitArray) {
  Region->init(BitArray.size());
  Region->recognizeAndPredict(&BitArray);
  // Region->init(&BitArray);
}
