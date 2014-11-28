#include "drawwidget.h"
#include <QtGui>


DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbling = false;
    penWidth = 2;
    color = qRgba(0,0,0,10);
}

void DrawWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
    }
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());
}

void DrawWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
    }
}

void DrawWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    const QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect.topLeft(), image, dirtyRect);
}

void DrawWidget::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(color, penWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (penWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void DrawWidget::resizeEvent(QResizeEvent *event)
 {
     if (width() > image.width() || height() > image.height()) {
         int newWidth = qMax(width() + 128, image.width());
         int newHeight = qMax(height() + 128, image.height());
         resizeImage(&image, QSize(newWidth, newHeight));
         update();
     }
     QWidget::resizeEvent(event);
 }

void DrawWidget::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_ARGB32);
    newImage.fill(qRgba(0, 0, 0, 0));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
