#ifndef __AUTONOMEWIDGET_H__
#define __AUTONOMEWIDGET_H__

#include <QWidget>
#include <QPaintEvent>
#include <QVector>

class AutonomeWidget : public QWidget {
Q_OBJECT
public:
    AutonomeWidget(QWidget *parent=NULL,unsigned int n=8,unsigned int m=8,qreal border=10.,qreal min_pad_size=15.);
    ~AutonomeWidget();
public slots:
    void pressPad(int i,int j,bool state);
    void setLed(int i,int j,bool state);
    void setLayer(int k);
    void setLedColumn(int j,int value);
    void setLedRow(int i,int value);
    void clearLed();
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
signals:
    void padPressed(int i,int j,bool state);
    void ledsSetted(const unsigned char *);
    void layerSetted(int k);
private:
    virtual void updateInternal(void);
    virtual void updateLedState(void);

    struct Pad {
        enum KeyStatus { UP=0 , DOWN=1 };
        enum LedStatus { ON=2 , OFF=0 };

        Pad(size_t i,size_t j,size_t n,size_t m);
        void paint(QPainter &painter);

        KeyStatus key;
        LedStatus leds[4];
        const size_t i,j;
        const size_t n,m;
        const QRectF rect;
    };

    typedef QVector<Pad*> Pads;
    int current_layer;
    Pads pads;
    Pad *left_pad;
    Pad *right_pad;

    size_t n,m;
    QPointF base;
    qreal border,size,min_size;
    unsigned char *led_state;
};


#endif
