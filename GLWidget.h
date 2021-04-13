#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include "CRegion.h"
#include "CRegionDraw.h"
#include <GL/glu.h>
#include <qgl.h>

class GLWidget : public QGLWidget {
  Q_OBJECT
public:
  GLWidget(QWidget *parent = 0);
  ~GLWidget();

  QSize minimumSizeHint() const;
  QSize sizeHint() const;

  void initRegion(const QBitArray &BitArray);
  inline CRegion *region() const { return Region; }
  inline CRegionDraw *regionDraw() const { return RegionDraw; }
  void setXTranslate(const double &x);
  void setYTranslate(const double &y);
public slots:
  void setXRotation(int angle);
  void setYRotation(int angle);
  void setZRotation(int angle);
  void setRegionGeometry(const int &width, const int &height,
                         const int &columnHeight);
  void setRegionCellRadius(const double &cellRadius) {
    RegionDraw->setCellRadius(cellRadius);
  }
  void setRegionCellSpace(const double &cellSpace) {
    RegionDraw->setCellSpace(cellSpace);
  }
  void setScale(const double &scale) {
    ScaleValue = scale;
    updateGL();
  }
  void setSceneFactor(const double &sceneFactor);

signals:
  void xRotationChanged(int angle);
  void yRotationChanged(int angle);
  void zRotationChanged(int angle);

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  int getObjectID(const int &x, const int &y);

private:
  QColor qtPurple;
  int xRotate, yRotate, zRotate;
  double xTranslate, yTranslate;
  double ScaleValue, SceneFactor;
  QPoint lastPos;
  CRegion *Region;
  CRegionDraw *RegionDraw;
};

#endif
