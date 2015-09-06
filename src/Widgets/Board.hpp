/**
  * Board.hpp
  * Defines class template Board. This class template handels
  * the visualization of the whole bunch of cells.
  *
  * Author: StormRaiser (Xiang Sitao)
  */

#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QEvent>
#include <QPainter>
#include <QColor>
#include <QString>
#include <QBitmap>
#include <QImage>
#include "Common.h"
#include "Rule.h"
#include "Grid.hpp"
#include "Region.hpp"
#include "Shader.h"
#include "BoardBase.h"
#include "GraphDialog.h"

#define MAX_ZOOM_LEVEL 10
#define MAX_ZOOM_LEVEL_HEX 5

using namespace CACore;

namespace CAGUI{

const int scaleFactorDef[MAX_ZOOM_LEVEL] = {1, 2, 3, 4, 6, 8, 11, 16, 23, 32};
const int hexWDef[MAX_ZOOM_LEVEL_HEX] = {2, 6, 10, 14, 22};
const int hexHDef[MAX_ZOOM_LEVEL_HEX] = {2, 5,  9, 12, 19};

template <typename ValType> class Board: public BoardBase{
private:
    //enum ShapeEnum{SquareShape, HexagonalShape, TriangularShape} shape;

    QColor bkColor, grid1Color, grid2Color;
    int gridInterval, mouseStartX, mouseStartY, mouseStartCX, mouseStartCY;
    int centerX, centerY, zoomLevel, scaleFactor, typeID, currentTime;
    int hexW, hexH;
    int rx, ry, tx1, ty1, tx2, ty2;
    bool partialRender, singleCellRender, clearRender, gridRender, convertRender;
    bool ruleHexFlag, boardHexFlag, hexFlag;
    bool showGrid1, showGrid2, editMode, mousePressed, posButton;
    bool runMode;
    Region<ValType> *rr;
    ValType activeState, rs;
    QBitmap hexMask[MAX_ZOOM_LEVEL_HEX][2];
    //bool smoothZoom;
    //double smoothZoomFactor;
    Rule<ValType> *rule;
    Grid<ValType> *grid;
    Shader<ValType> *shader;
    void pixel2Cell(int px, int py, int &cx, int &cy);
    void cell2Pixel(int cx, int cy, int &px, int &py); // lowerleft
    void pixel2CellHex(int px, int py, int &cx, int &cy);
    void cell2PixelHex(int cx, int cy, int &px, int &py); // upperleft of bounding rectangle
    void renderControl();
    void putCell(int px, int py);
    void zoom(int cx, int cy, int level);
    void updateZoom(){
        if (hexFlag){
            if (zoomLevel < 0)
                zoomLevel = 0;
            if (zoomLevel >= MAX_ZOOM_LEVEL_HEX)
                zoomLevel = MAX_ZOOM_LEVEL_HEX - 1;
            hexW = hexWDef[zoomLevel];
            hexH = hexHDef[zoomLevel];
        }
        else{
            if (zoomLevel < 0)
                zoomLevel = 0;
            if (zoomLevel >= MAX_ZOOM_LEVEL)
                zoomLevel = MAX_ZOOM_LEVEL - 1;
            scaleFactor = scaleFactorDef[zoomLevel];
        }
    }

    static int rdiv(int a, int b){return a > 0 ? a / b : (a - b + 1) / b;};
    static int rmod(int a, int b){return a - b * rdiv(a, b);};

public:
    Board(QWidget *parent = 0);
    ~Board(){if (rule != NULL) delete rule; delete grid;};
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void leaveEvent(QEvent *event);
    void setRule(RuleBase *p_rule){
        if (rule != NULL)
            delete rule;
        rule = (Rule<ValType>*)(p_rule);
        grid->setRule(rule);
        ruleHexFlag = rule->shape() == RuleBase::HexagonalShape;
        hexFlag = ruleHexFlag && boardHexFlag;
        updateZoom();
        repaint();
    };
    void setShader(ShaderBase *p_shader){
        shader = (Shader<ValType>*)(p_shader);
        bkColor = shader->shade(0);
        repaint();
    };
    void clear(){grid->clear(); repaint();};
    void setup(int w, int h){currentTime = 0; grid->setup(w, h); repaint();};
    void toggleEdit(bool t){editMode = t;};
    void evolve();
    void render();
    void setActiveState(void *state){activeState = *((ValType*)(state));};
    long long getTime(){return grid->getTime();};
    long long population(){return grid->population();};
    long long population(int index){
        if (rule->needConversion())
            return grid->population((unsigned char)(rule->nominal2Real(index, currentTime)));
        else
            return grid->population(index);
    };
    void randomize(double *pos);
    void setGraphSetting(GraphSetting *setting);
    void zoomIn(){zoom(width() / 2, height() / 2, 1);};
    void zoomOut(){zoom(width() / 2, height() / 2, -1);};
    Grid<ValType> *getGrid(){return grid;};
};

template <typename ValType> Board<ValType>::Board(QWidget *parent): BoardBase(parent){
    grid = new Grid<ValType>;
    grid->setup(100, 100);
    rule = NULL;
    shader = NULL;
    centerX = 0;
    centerY = 0;
    zoomLevel = 5;
    scaleFactor = 8;
    gridInterval = 16;
    currentTime = 0;
    showGrid1 = true;
    showGrid2 = true;
    editMode = false;
    mousePressed = false;
    partialRender = false;
    singleCellRender = false;
    convertRender = false;
    // clearRender = false;
    bkColor = QColor(0, 0, 0);
    grid1Color = QColor(48, 48, 48);
    grid2Color = QColor(96, 96, 96);
    activeState = 0;
    setMouseTracking(true);
    //
    ruleHexFlag = false;
    boardHexFlag = false;
    hexFlag = false;
    zoomLevel = 3;
    hexW = hexWDef[zoomLevel];
    hexH = hexHDef[zoomLevel];
    //
    hexMask[0][1] = QBitmap::fromImage(QImage(":/mask/hex0solid.png"));
    hexMask[1][0] = QBitmap::fromImage(QImage(":/mask/hex1border.png"));
    hexMask[1][1] = QBitmap::fromImage(QImage(":/mask/hex1solid.png"));
    hexMask[2][0] = QBitmap::fromImage(QImage(":/mask/hex2border.png"));
    hexMask[2][1] = QBitmap::fromImage(QImage(":/mask/hex2solid.png"));
    hexMask[3][0] = QBitmap::fromImage(QImage(":/mask/hex3border.png"));
    hexMask[3][1] = QBitmap::fromImage(QImage(":/mask/hex3solid.png"));
    hexMask[4][0] = QBitmap::fromImage(QImage(":/mask/hex4border.png"));
    hexMask[4][1] = QBitmap::fromImage(QImage(":/mask/hex4solid.png"));
}

template <typename ValType> void Board<ValType>::pixel2Cell(int px, int py, int &cx, int &cy){
    cx = rdiv(px - width() / 2, scaleFactor) + centerX;
    cy = rdiv(height() / 2 - py, scaleFactor) + centerY;
}

template <typename ValType> void Board<ValType>::cell2Pixel(int cx, int cy, int &px, int &py){
    px = (cx - centerX) * scaleFactor + width() / 2;
    py = height() / 2 - (cy - centerY) * scaleFactor;
}

template <typename ValType> void Board<ValType>::pixel2CellHex(int px, int py, int &cx, int &cy){
    int p, q, r;
    p = rdiv(3 * hexW * (height() / 2 - py) + 2 * hexH * (px - width() / 2) + 1, 2 * hexW * hexH);
    q = rdiv(3 * hexW * (height() / 2 - py) - 2 * hexH * (px - width() / 2) - 1, 2 * hexW * hexH);
    r = rdiv(2 * (px - width() / 2), hexW);
    cy = rdiv(p + q + 1, 3);
    cx = rdiv(r + cy, 2) + centerX;
    cy += centerY;
}

template <typename ValType> void Board<ValType>::cell2PixelHex(int cx, int cy, int &px, int &py){
    px = width() / 2 + (cx - centerX) * hexW - (cy - centerY) * hexW / 2;
    py = height() / 2 - (cy - centerY + 1) * hexH + 1;
}

template <typename ValType> void Board<ValType>::putCell(int px, int py){
    int px2, py2;
    if (!hexFlag)
        pixel2Cell(px, py, rx, ry);
    else
        pixel2CellHex(px, py, rx, ry);
    if ((rx >= grid->minX()) && (rx <= grid->maxX()) && (ry >= grid->minY()) && (ry <= grid->maxY())){
        if (posButton)
            rs = rule->nominal2Real(activeState, currentTime);
        else
            rs = rule->nominal2Real(0, currentTime);
        grid->putCell(rx, ry, rs);
        singleCellRender = true;
        if (!hexFlag){
            cell2Pixel(rx, ry + 1, px2, py2);
            if (showGrid1 && (zoomLevel >= 3))
                repaint(px2 + 1, py2 + 1, scaleFactor - 1, scaleFactor - 1);
            else
                repaint(px2, py2 + 1, scaleFactor, scaleFactor);
        }
        else{
            cell2PixelHex(rx, ry, px2, py2);
            repaint(px2 - hexW / 2, py2 - hexH / 2, hexW * 2, hexH * 2);
        }
        singleCellRender = false;
    }
    emit populationChanged(QString("Population = %1").arg(population()));
}

template <typename ValType> void Board<ValType>::renderControl(){
    int x1, y1, x2, y2, lx1, ly1, lx2, ly2, px1, py1, px2, py2, ci, cj;
    partialRender = true;
    pixel2Cell(0, height() - 1, x1, y1);
    pixel2Cell(width() - 1, 0, x2, y2);
    x1 = max(x1, grid->minX());
    x2 = min(x2, grid->maxX());
    y1 = max(y1, grid->minY());
    y2 = min(y2, grid->maxY());
    for (ci = x1 >> REGION_SCALE_FULL; ci <= x2 >> REGION_SCALE_FULL; ci++)
        for (cj = y1 >> REGION_SCALE_FULL; cj <= y2 >> REGION_SCALE_FULL; cj++){
            rr = grid->findRegion(ci, cj);
            lx1 = max(x1, ci << REGION_SCALE_FULL);
            lx2 = min(x2, ((ci + 1) << REGION_SCALE_FULL) - 1);
            ly1 = max(y1, cj << REGION_SCALE_FULL);
            ly2 = min(y2, ((cj + 1) << REGION_SCALE_FULL) - 1);
            if (rr != NULL){
                rr->renderInitialize(lx1 - (ci << REGION_SCALE_FULL), ly1 - (cj << REGION_SCALE_FULL), lx2 - (ci << REGION_SCALE_FULL), ly2 - (cj << REGION_SCALE_FULL));
                while (true){
                    if (!rr->getRenderRegion(tx1, ty1, tx2, ty2))
                        break;
                    rx = tx1 + (ci << REGION_SCALE_FULL);
                    ry = ty1 + (cj << REGION_SCALE_FULL);
                    cell2Pixel(rx, ry, px1, py2);
                    cell2Pixel(tx2 + (ci << REGION_SCALE_FULL) + 1, ty2 + (cj << REGION_SCALE_FULL) + 1, px2, py1);
                    repaint(px1, py1 + 1, px2 - px1, py2 - py1);
                }
            }
            else if (rule->needConversion()){
                cell2Pixel(lx1, ly1, px1, py1);
                cell2Pixel(lx2 + 1, ly2 + 1, px2, py2);
                convertRender = true;
                repaint(px1, py2 + 1, px2 - px1, py1 - py2);
                convertRender = false;
            }
        }
    partialRender = false;
}

template <typename ValType> void Board<ValType>::paintEvent(QPaintEvent *event){
    int x1, y1, x2, y2, lx1, ly1, lx2, ly2, px1, py1, px2, py2, ci, cj, i, j;
    QPainter painter(this);
    painter.fillRect(event->rect(), bkColor);
    if (!hexFlag){
        if (singleCellRender){
            painter.fillRect(event->rect(), shader->shade(rule->real2Nominal(rs, currentTime)));
            return;
        }
        pixel2Cell(event->rect().left(), event->rect().bottom(), x1, y1);
        pixel2Cell(event->rect().right(), event->rect().top(), x2, y2);
        x1 = max(x1, grid->minX());
        x2 = min(x2, grid->maxX());
        y1 = max(y1, grid->minY());
        y2 = min(y2, grid->maxY());
        if (convertRender)
            painter.fillRect(event->rect(), shader->shade(rule->real2Nominal(0, currentTime)));
        else if (partialRender){
            if (!rule->needConversion()){
                for (i = tx1; i <= tx2; i++)
                    for (j = ty1; j <= ty2; j++){
                        cell2Pixel(rx - tx1 + i, ry - ty1 + j + 1, px1, py1);
                        painter.fillRect(px1, py1 + 1, scaleFactor, scaleFactor, shader->shade(rr->getCell(i, j)));
                    }
            }
            else{
                for (i = tx1; i <= tx2; i++)
                    for (j = ty1; j <= ty2; j++){
                        cell2Pixel(rx - tx1 + i, ry - ty1 + j + 1, px1, py1);
                        painter.fillRect(px1, py1 + 1, scaleFactor, scaleFactor, shader->shade(rule->real2Nominal(rr->getCell(i, j), currentTime)));
                    }
            }
        }
        else{
            for (ci = x1 >> REGION_SCALE_FULL; ci <= x2 >> REGION_SCALE_FULL; ci++)
                for (cj = y1 >> REGION_SCALE_FULL; cj <= y2 >> REGION_SCALE_FULL; cj++){
                    lx1 = max(x1, ci << REGION_SCALE_FULL);
                    lx2 = min(x2, ((ci + 1) << REGION_SCALE_FULL) - 1);
                    ly1 = max(y1, cj << REGION_SCALE_FULL);
                    ly2 = min(y2, ((cj + 1) << REGION_SCALE_FULL) - 1);
                    rr = grid->findRegion(ci, cj);
                    if (rr == NULL)
                        if (!rule->needConversion()){
                            cell2Pixel(lx1, ly1, px1, py1);
                            cell2Pixel(lx2 + 1, ly2 + 1, px2, py2);
                            painter.fillRect(px1, py2 + 1, px2 - px1, py2 - py1, shader->shade(0));
                        }
                        else{
                            cell2Pixel(lx1, ly1, px1, py1);
                            cell2Pixel(lx2 + 1, ly2 + 1, px2, py2);
                            painter.fillRect(px1, py2 + 1, px2 - px1, py1 - py2, shader->shade(rule->real2Nominal(0, currentTime)));
                        }
                    else{
                        rr->clearRender();
                        if (!rule->needConversion()){
                            for (i = lx1; i <= lx2; i++)
                                for (j = ly1; j <= ly2; j++){
                                    cell2Pixel(i, j + 1, px1, py1);
                                    painter.fillRect(px1, py1 + 1, scaleFactor, scaleFactor, shader->shade(rr->getCell(i - (ci << REGION_SCALE_FULL), j - (cj << REGION_SCALE_FULL))));
                                }
                        }
                        else{
                            for (i = lx1; i <= lx2; i++)
                                for (j = ly1; j <= ly2; j++){
                                    cell2Pixel(i, j + 1, px1, py1);
                                    painter.fillRect(px1, py1 + 1, scaleFactor, scaleFactor, shader->shade(rule->real2Nominal(rr->getCell(i - (ci << REGION_SCALE_FULL), j - (cj << REGION_SCALE_FULL)), currentTime)));
                                }
                        }
                    }
                }
        }
        if (showGrid1 && (zoomLevel >= 3)){
            //qDebug("#1");
            painter.setPen(grid1Color);
            for (i = x1; i <= x2 + 1; i++){
                cell2Pixel(i, y1, px1, py1);
                cell2Pixel(i, y2 + 1, px2, py2);
                painter.drawLine(px1, py1, px2, py2);
            }
            for (j = y1; j <= y2 + 1; j++){
                cell2Pixel(x1, j, px1, py1);
                cell2Pixel(x2 + 1, j, px2, py2);
                painter.drawLine(px1, py1, px2, py2);
            }
            if (showGrid2){
                painter.setPen(grid2Color);
                if (x1 == grid->minX()){
                    cell2Pixel(x1, y1, px1, py1);
                    cell2Pixel(x1, y2 + 1, px2, py2);
                    painter.drawLine(px1, py1, px2, py2);
                }
                if (x2 == grid->maxX()){
                    cell2Pixel(x2 + 1, y1, px1, py1);
                    cell2Pixel(x2 + 1, y2 + 1, px2, py2);
                    painter.drawLine(px1, py1, px2, py2);
                }
                if (y1 == grid->minY()){
                    cell2Pixel(x1, y1, px1, py1);
                    cell2Pixel(x2 + 1, y1, px2, py2);
                    painter.drawLine(px1, py1, px2, py2);
                }
                if (y2 == grid->maxY()){
                    cell2Pixel(x1, y2 + 1, px1, py1);
                    cell2Pixel(x2 + 1, y2 + 1, px2, py2);
                    painter.drawLine(px1, py1, px2, py2);
                }
                for (i = x1; i <= x2 + 1; i++)
                    if (i % gridInterval == 0){
                        cell2Pixel(i, y1, px1, py1);
                        cell2Pixel(i, y2 + 1, px2, py2);
                        painter.drawLine(px1, py1, px2, py2);
                    }
                for (j = y1; j <= y2 + 1; j++)
                    if (j % gridInterval == 0){
                        cell2Pixel(x1, j, px1, py1);
                        cell2Pixel(x2 + 1, j, px2, py2);
                        painter.drawLine(px1, py1, px2, py2);
                    }
            }
            //qDebug("#2");
        }
    }
    ////////////////////////////////////////////////////////////////
    else{
        pixel2CellHex(event->rect().left(), event->rect().bottom(), x1, y1);
        pixel2CellHex(event->rect().right(), event->rect().top(), x2, y2);
        x1 = max(x1 - 1, grid->minX());
        x2 = min(x2 + 1, grid->maxX());
        y1 = max(y1 - 1, grid->minY());
        y2 = min(y2 + 1, grid->maxY());
        for (ci = x1 >> REGION_SCALE_FULL; ci <= x2 >> REGION_SCALE_FULL; ci++)
            for (cj = y1 >> REGION_SCALE_FULL; cj <= y2 >> REGION_SCALE_FULL; cj++){
                lx1 = max(x1, ci << REGION_SCALE_FULL);
                lx2 = min(x2, ((ci + 1) << REGION_SCALE_FULL) - 1);
                ly1 = max(y1, cj << REGION_SCALE_FULL);
                ly2 = min(y2, ((cj + 1) << REGION_SCALE_FULL) - 1);
                rr = grid->findRegion(ci, cj);
                if (rr == NULL){
                    painter.setPen(shader->shade(rule->real2Nominal(0, currentTime)));
                    for (i = lx1; i <= lx2; i++)
                        for (j = ly1; j <= ly2; j++){
                            cell2PixelHex(i, j, px1, py1);
                            painter.drawPixmap(px1, py1, hexMask[zoomLevel][1]);
                        }
                }
                else{
                    for (i = lx1; i <= lx2; i++)
                        for (j = ly1; j <= ly2; j++){
                            cell2PixelHex(i, j, px1, py1);
                            painter.setPen(shader->shade(rule->real2Nominal(rr->getCell(i - (ci << REGION_SCALE_FULL), j - (cj << REGION_SCALE_FULL)), currentTime)));
                            painter.drawPixmap(px1, py1, hexMask[zoomLevel][1]);
                        }
                }
                if (showGrid1 && (zoomLevel >= 1)){
                    painter.setPen(grid1Color);
                    for (i = lx1; i <= lx2; i++)
                        for (j = ly1; j <= ly2; j++){
                            cell2PixelHex(i, j, px1, py1);
                            painter.drawPixmap(px1, py1, hexMask[zoomLevel][0]);
                        }
                }
            }
    }
}

template <typename ValType> void Board<ValType>::evolve(){
    currentTime++;
    grid->evolve();
}

template <typename ValType> void Board<ValType>::render(){
    singleCellRender = false;
    if (!hexFlag){
        partialRender = true;
        renderControl();
        partialRender = false;
    }
    else
        update();
}

template <typename ValType> void Board<ValType>::mousePressEvent(QMouseEvent *event){
    mousePressed = true;
    if (editMode){
        posButton = event->button() == Qt::LeftButton;
        putCell(event->x(), event->y());
    }
    else{
        mouseStartX = event->x();
        mouseStartY = event->y();
        mouseStartCX = centerX;
        mouseStartCY = centerY;
    }
}

template <typename ValType> void Board<ValType>::mouseMoveEvent(QMouseEvent *event){
    int cx, cy, t;
    if (!hexFlag)
        pixel2Cell(event->x(), event->y(), cx, cy);
    else
        pixel2CellHex(event->x(), event->y(), cx, cy);
    if ((cx >= grid->minX()) && (cx <= grid->maxX()) && (cy >= grid->minY()) && (cy <= grid->maxY()))
        emit coordinateChanged(QString("x = %1, y = %2").arg(cx).arg(cy));
    else
        emit coordinateChanged(QString("x = ---, y = ---"));
    if (mousePressed){
        if (editMode)
            putCell(event->x(), event->y());
        else{
            if (!hexFlag){
                centerX = mouseStartCX - (event->x() - mouseStartX) / scaleFactor;
                centerY = mouseStartCY + (event->y() - mouseStartY) / scaleFactor;
            }
            else{
                t = (event->y() - mouseStartY) / hexH;
                centerY = mouseStartCY + t;
                centerX = mouseStartCX - (event->x() - mouseStartX) / hexW + t / 2;
            }
            update();
        }
    }
}

template <typename ValType> void Board<ValType>::mouseReleaseEvent(QMouseEvent *event){
    mousePressed = false;
}

template <typename ValType> void Board<ValType>::wheelEvent(QWheelEvent *event){
    zoom(event->x(), event->y(), event->angleDelta().y() / 120);
}

template <typename ValType> void Board<ValType>::leaveEvent(QEvent *event){
    emit coordinateChanged(QString("x = ---, y = ---"));
}

template <typename ValType> void Board<ValType>::randomize(double *pos){
    grid->randomize(pos);
    repaint();
    emit populationChanged(QString("Population = %1").arg(population()));
}

template <typename ValType> void Board<ValType>::setGraphSetting(GraphSetting *setting){
    showGrid1 = setting->showGrid1;
    showGrid2 = setting->showGrid2;
    grid1Color = setting->grid1Color;
    grid2Color = setting->grid2Color;
    gridInterval = setting->gridInterval;
    boardHexFlag = setting->boardHexFlag;
    hexFlag = ruleHexFlag && boardHexFlag;
    updateZoom();
    repaint();
}

template <typename ValType> void Board<ValType>::zoom(int cx, int cy, int level){
    int t;
    if (!hexFlag){
        centerX += (cx - width() / 2) / scaleFactor;
        centerY += (height() / 2 - cy) / scaleFactor;
        zoomLevel += level;
        updateZoom();
        centerX -= (cx - width() / 2) / scaleFactor;
        centerY -= (height() / 2 - cy) / scaleFactor;
    }
    else{
        t = (height() / 2 - cy) / hexH;
        centerY += t;
        centerX += (cx - width() / 2) / hexW + t / 2;
        zoomLevel += level;
        updateZoom();
        t = (height() / 2 - cy) / hexH;
        centerY -= t;
        centerX -= (cx - width() / 2) / hexW + t / 2;
    }
    update();
}

}
