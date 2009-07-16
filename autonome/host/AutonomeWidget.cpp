#include "AutonomeWidget.h"

#include <QPainter>

AutonomeWidget::Pad::Pad(size_t ii,size_t jj,const QPointF &base,const qreal &size) : key(UP), led(OFF), i(ii), j(jj),  base(base), size(size) {}

void AutonomeWidget::Pad::paint(QPainter &painter) {
    rect.setRect(base.x() + j*size,base.y() + i*size,size,size);
    painter.drawRect(rect);
}

AutonomeWidget::AutonomeWidget(unsigned int n,unsigned int m,qreal border,qreal min_pad_size,QWidget *parent) : QWidget(parent), n(n), m(m), border(border), min_size(min_pad_size) {
    setMinimumWidth(2*border + m*min_size);
    setMinimumHeight(2*border + n*min_size);

    for (size_t i=0; i<n; i++) for (size_t j=0; j<m; j++) pads.append(new Pad(i,j,base,size));
}

AutonomeWidget::~AutonomeWidget() {
    for (Pads::iterator i=pads.begin(); i!=pads.end(); i++) delete *i;
}

void AutonomeWidget::updateInternal() {
    qreal h = height() - 2*border;
    qreal w = width() - 2*border;
    qreal hsize = h/n;
    qreal wsize = w/m;

    if (hsize < wsize) size = hsize; //horizontal
    else size = wsize; //vertical

    base.setX(border + (w - m*size)/2.);
    base.setY(border + (h - n*size)/2.);
}

void AutonomeWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    updateInternal();

    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::red);
    for (Pads::iterator i=pads.begin(); i!=pads.end(); i++) (*i)->paint(painter);
}


