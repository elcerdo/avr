#include "AutonomeWidget.h"

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>

const static QBrush brushes[4] = { QBrush(Qt::white), QBrush(QColor::fromRgb(230,230,230)), QBrush(QColor::fromRgb(170,255,255)), QBrush(Qt::red) };

AutonomeWidget::Pad::Pad(size_t ii,size_t jj,size_t nn,size_t mm) : key(UP), led(OFF), i(ii), j(jj), n(nn), m(mm), rect((jj-mm/2.)+.05,(ii-nn/2.)+.05,.9,.9) {}

void AutonomeWidget::Pad::paint(QPainter &painter) {
    painter.setBrush(brushes[led+key]);
    painter.drawRoundedRect(rect,.1,.1);
}

AutonomeWidget::AutonomeWidget(QWidget *parent,unsigned int n,unsigned int m,qreal border,qreal min_pad_size) : QWidget(parent), left_pad(NULL), right_pad(NULL), n(n), m(m), border(border), min_size(min_pad_size) {
    setMinimumWidth(2*border + m*min_size);
    setMinimumHeight(2*border + n*min_size);

    for (size_t i=0; i<n; i++) for (size_t j=0; j<m; j++) pads.append(new Pad(i,j,n,m));

    updateInternal();
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

    base.setX( width()/2. );
    base.setY( height()/2. );
}

void AutonomeWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
    updateInternal();
}

void AutonomeWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.translate(base);
    painter.scale(size,size);
    for (Pads::iterator i=pads.begin(); i!=pads.end(); i++) (*i)->paint(painter);
}

void AutonomeWidget::mousePressEvent(QMouseEvent *event) {
    Pad **target_pad = NULL;
    switch (event->button()) {
    case Qt::LeftButton: target_pad = &left_pad; break;
    case Qt::RightButton: target_pad = &right_pad; break;
    default: return; break;
    }

    Q_ASSERT( *target_pad == NULL );

    QPointF click_pos = event->posF();
    click_pos -= base;
    click_pos /= size;

    for (Pads::iterator i=pads.begin(); i!=pads.end(); i++) if ( (*i)->rect.contains(click_pos) ) {
        *target_pad = *i;
        break;
    }

    if ( *target_pad ) {
        (*target_pad)->key = Pad::DOWN;
        emit padPressed((*target_pad)->i, (*target_pad)->j);
        update();
    }
}

void AutonomeWidget::mouseReleaseEvent(QMouseEvent *event) {
    Pad **target_pad = NULL;
    switch (event->button()) {
    case Qt::LeftButton: target_pad = &left_pad; break;
    case Qt::RightButton: target_pad = &right_pad; break;
    default: return; break;
    }

    if (*target_pad == NULL) return;

    (*target_pad)->key = Pad::UP;
    emit padReleased((*target_pad)->i, (*target_pad)->j);
    *target_pad = NULL;
    update();
}
