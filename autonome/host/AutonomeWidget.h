#ifndef __AUTONOMEWIDGET_H__
#define __AUTONOMEWIDGET_H__

#include <QWidget>
#include <QPaintEvent>
#include <QVector>

class AutonomeWidget : public QWidget {
Q_OBJECT
public:
    AutonomeWidget(unsigned int n=8,unsigned int m=8,qreal border=10.,qreal min_pad_size=5.,QWidget *parent=0);
    ~AutonomeWidget();
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void updateInternal(void);
private:
    class Pad {
    public:
        enum KeyStatus { UP, DOWN };
        enum LedStatus { ON, OFF };

        Pad(size_t i,size_t j,const QPointF &base,const qreal &size);
        void paint(QPainter &painter);

        KeyStatus key;
        LedStatus led;
    protected:
        size_t i,j;

        QRectF rect;
        const QPointF &base;
        const qreal &size;
    };

    typedef QVector<Pad*> Pads;
    Pads pads;

    size_t n,m;
    QPointF base;
    qreal border,size,min_size;
};


#endif
