#ifndef CREGIONDRAW_H
#define CREGIONDRAW_H

#include <GL/glu.h>
#include <QColor>
#include <QVector>
#include <QtOpenGL>

#include "Defines.h"

class CRegionDraw : public QObject {
  Q_OBJECT
public:
  struct CColumnDraw {
    bool IsActive;
    QVector<GLUquadric *> ColumnCells;
    QColor ColumnColor;
  };

  inline void setWidth(const int &width) {
    acceptGeometryChanges(width, Height, ColumnHeight);
  }
  inline void setHeight(const int &height) {
    acceptGeometryChanges(Width, height, ColumnHeight);
  }
  inline void setColumnHeight(const int &columnHeight) {
    acceptGeometryChanges(Width, Height, columnHeight);
  }
  inline void setGeometry(const int &width, const int &height,
                          const int &columnHeight) {
    acceptGeometryChanges(width, height, columnHeight);
  }

  CRegionDraw(const int &width, const int &height, const int &columnHeight);

  inline void setCellRadius(const double &radius) { CellRadius = radius; }
  inline void setCellSpace(const double &space) { CellSpace = space; }
  void getTopLeftNearPoint(double &X, double &Y, double &Z);
  void draw();
public slots:
  void setRegionGeometry(const int &width, const int &height,
                         const int &columnHeight) {
    setGeometry(width, height, columnHeight);
  }
  void redrawColumn(const int &columnIndex, const bool &isActive);
  void setActiveCellColor(const QColor &color);
  void setInactiveCellColor(const QColor &color);

protected:
  void acceptGeometryChanges(const int &width, const int &height,
                             const int &columnHeight);

protected:
  int Width, Height, ColumnHeight;
  QVector<CColumnDraw> Columns;
  double CellRadius, CellSpace;
  QColor ActiveCellColor, InactiveCellColor;
};

#endif
