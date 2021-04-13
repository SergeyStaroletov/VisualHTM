#include "CRegionDraw.h"

CRegionDraw::CRegionDraw(const int &width, const int &height,
                         const int &columnHeight)
    : Width(width), Height(height), ColumnHeight(columnHeight),
      CellRadius(CELL_RADIUS_DEF_VALUE), CellSpace(CELL_SPACE_DEF_VALUE) {
  ActiveCellColor = DefaultValues.ActiveCellColor;
  InactiveCellColor = DefaultValues.InactiveCellColor;
  Columns.resize(Width * Height);
  int ColumnIndex, CellIndex;
  for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex) {
    Columns[ColumnIndex].ColumnCells.resize(columnHeight);
    Columns[ColumnIndex].IsActive = false;
    Columns[ColumnIndex].ColumnColor = InactiveCellColor;
    for (CellIndex = 0; CellIndex < ColumnHeight; ++CellIndex)
      Columns[ColumnIndex].ColumnCells[CellIndex] = gluNewQuadric();
  }
}

void CRegionDraw::acceptGeometryChanges(const int &width, const int &height,
                                        const int &columnHeight) {
  int TempWidth, TempHeight, TempColumnHeight, ColumnIndex, CellIndex;
  TempWidth = width > 0 ? width : 1;
  TempHeight = height > 0 ? height : 1;
  TempColumnHeight = columnHeight > 0 ? columnHeight : 1;

  if (TempWidth == Width && TempHeight == Height &&
      TempColumnHeight == ColumnHeight)
    return;

  for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex) {
    Columns[ColumnIndex].ColumnCells.resize(ColumnHeight);
    for (CellIndex = 0; CellIndex < ColumnHeight; ++CellIndex)
      gluDeleteQuadric(Columns[ColumnIndex].ColumnCells[CellIndex]);
  }

  if (Width != TempWidth || Height != TempHeight) {
    Width = TempWidth;
    Height = TempHeight;
    ColumnHeight = TempColumnHeight;
    Columns.resize(Width * Height);
    for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex) {
      Columns[ColumnIndex].ColumnCells.resize(ColumnHeight);
      Columns[ColumnIndex].IsActive = false;
      Columns[ColumnIndex].ColumnColor = InactiveCellColor;
      for (CellIndex = 0; CellIndex < ColumnHeight; ++CellIndex)
        Columns[ColumnIndex].ColumnCells[CellIndex] = gluNewQuadric();
    }
  } else {
    ColumnHeight = TempColumnHeight;
    for (ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex) {
      Columns[ColumnIndex].ColumnCells.resize(ColumnHeight);
      for (CellIndex = 0; CellIndex < ColumnHeight; ++CellIndex)
        Columns[ColumnIndex].ColumnCells[CellIndex] = gluNewQuadric();
    }
  }
}

void CRegionDraw::getTopLeftNearPoint(double &X, double &Y, double &Z) {
  if (Width % 2 == 0)
    X = -((CellRadius * 2. * (double)(Width >> 1) +
           CellSpace * (double)((Width >> 1) - 1)));
  else
    X = -((CellRadius * 2. + CellSpace) * (double)((Width - 1) >> 1) +
          CellRadius);

  if (Height % 2 == 0)
    Z = (CellRadius * 2. * (double)(Height >> 1) +
         CellSpace * (double)((Height >> 1) - 1));
  else
    Z = (CellRadius * 2. + CellSpace) * (double)((Height - 1) >> 1) +
        CellRadius;

  if (ColumnHeight % 2 == 0)
    Y = (CellRadius * 2. * (double)(ColumnHeight >> 1) +
         CellSpace * (double)((ColumnHeight >> 1) - 1));
  else
    Y = (CellRadius * 2. * (double)(ColumnHeight >> 1) +
         CellSpace * (double)((ColumnHeight >> 1) - 1));
}

void CRegionDraw::draw() {
  int WidthIndex, HeightIndex, CellIndex;
  double LeftX, TopY, NearZ, X, Y;
  getTopLeftNearPoint(LeftX, TopY, NearZ);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(LeftX, TopY, NearZ);
  for (HeightIndex = 0; HeightIndex < Height; ++HeightIndex) {
    X = 0.;
    for (WidthIndex = 0; WidthIndex < Width; ++WidthIndex) {
      Y = 0.;
      glColor3f(Columns[HeightIndex * Width + WidthIndex].ColumnColor.redF(),
                Columns[HeightIndex * Width + WidthIndex].ColumnColor.greenF(),
                Columns[HeightIndex * Width + WidthIndex].ColumnColor.blueF());
      for (CellIndex = 0; CellIndex < ColumnHeight; ++CellIndex) {
        //      gluQuadricOrientation(Columns[0].ColumnCells[CellIndex],GLU_OUTSIDE);
        //      gluQuadricNormals(Columns[0].ColumnCells[CellIndex],GL_SMOOTH);
        //          glPushName(); HERE!!!
        gluQuadricDrawStyle(
            Columns[HeightIndex * Width + WidthIndex].ColumnCells[CellIndex],
            GLU_LINE);
        //      gluQuadricDrawStyle(Columns[HeightIndex * Width +
        //      WidthIndex].ColumnCells[CellIndex],GLU_FILL);
        gluSphere(
            Columns[HeightIndex * Width + WidthIndex].ColumnCells[CellIndex],
            CellRadius, 20, 20);
        //          glPopName();
        glTranslatef(0., -(CellRadius * 2. + CellSpace), 0.);
        Y += CellRadius * 2. + CellSpace;
      }
      glTranslatef(CellRadius * 2. + CellSpace, Y, 0.);
      X += CellRadius * 2. + CellSpace;
    }
    glTranslatef(-X, 0., CellRadius * 2. + CellSpace);
  }
  glPopMatrix();
}

void CRegionDraw::redrawColumn(const int &columnIndex, const bool &isActive) {
  Columns[columnIndex].IsActive = isActive;
  if (isActive)
    Columns[columnIndex].ColumnColor = ActiveCellColor;
  else
    Columns[columnIndex].ColumnColor = InactiveCellColor;
}

void CRegionDraw::setActiveCellColor(const QColor &color) {
  ActiveCellColor = color;
  for (int ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex)
    if (Columns[ColumnIndex].IsActive)
      Columns[ColumnIndex].ColumnColor = color;
}

void CRegionDraw::setInactiveCellColor(const QColor &color) {
  InactiveCellColor = color;
  for (int ColumnIndex = 0; ColumnIndex < Width * Height; ++ColumnIndex)
    if (!Columns[ColumnIndex].IsActive)
      Columns[ColumnIndex].ColumnColor = color;
}
