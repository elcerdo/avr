#ifndef __OSCWIDGET_H__
#define __OSCWIDGET_H__

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <lo/lo.h>

class OSCWidget : public QWidget {
Q_OBJECT
public:
    OSCWidget(QWidget *parent = NULL,bool autostart = true);
    ~OSCWidget();
public slots:
    void setRunning(bool);
    void padPressed(int i,int j,bool state);
signals:
    void setLed(int i,int j,bool state);
    void setLedColumn(int j,int value);
    void setLedRow(int i,int value);
    void clearLed();
protected:
    void startServer();
    void stopServer();
private:
    static void server_err_callback(int num,const char *msg,const char *where);
    static int server_all_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *user_data);
    static int server_led_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *user_data);
    static int server_col_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *user_data);
    static int server_row_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *user_data);
    static int server_clear_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *user_data);
    QPushButton *run_button;
    QLabel *server_state;
    QLabel *client_state;
    QString prefix;

    void *client;
    void *server;

    enum State { STOPPED, RUNNING };
    State state;
};

#endif
