#include "OSCWidget.h"

#include <QVBoxLayout>
#include <QSettings>

static const QString status_strings[3] = { QObject::tr("%1 status"), QObject::tr("%1 started at %2"), QObject::tr("%1 stopped") };
static const QString status_names[2] = { QObject::tr("Server"), QObject::tr("Client") };
static OSCWidget *instance = NULL;

OSCWidget::OSCWidget(QWidget *parent,bool autostart) : QWidget(parent), client(NULL), server(NULL), state(STOPPED) {
    Q_ASSERT( instance == NULL ); //only one instance
    instance = this;

    setLayout(new QVBoxLayout(this));

    run_button = new QPushButton("Start Server",this);
    run_button->setCheckable(true);
    layout()->addWidget(run_button);
    connect(run_button,SIGNAL( clicked(bool) ),this,SLOT( setRunning(bool) ));

    server_state = new QLabel(status_strings[0].arg(status_names[0]),this);
    layout()->addWidget(server_state);

    client_state = new QLabel(status_strings[0].arg(status_names[1]),this);
    layout()->addWidget(client_state);

    dynamic_cast<QBoxLayout*>(layout())->addStretch(1);
    if (autostart) {
        run_button->setChecked(true);
        setRunning(true);
    }
}

OSCWidget::~OSCWidget() {
    stopServer();
    instance = NULL;
}

void OSCWidget::padPressed(int i,int j,bool on) {
    if (state != RUNNING) return;
    Q_ASSERT( client != NULL );

    lo_send(client,qPrintable(prefix + "press"),"iii",i,j,on);
}


void OSCWidget::stopServer() {
    if (state != RUNNING) return;
    qDebug("[osc] stopping server");
    Q_ASSERT( server != NULL );
    Q_ASSERT( client != NULL );

    lo_server_thread_stop(server);

    lo_server_thread_free(server); server = NULL;
    lo_address_free(client); client = NULL;

    state = STOPPED;
    server_state->setText(status_strings[2].arg(status_names[0]));
    client_state->setText(status_strings[2].arg(status_names[1]));
}

void OSCWidget::startServer() {
    if (state != STOPPED) return;
    qDebug("[osc] starting server");
    Q_ASSERT( server == NULL );
    Q_ASSERT( client == NULL );

    QSettings settings;
    prefix = "/" + settings.value("general/name").toString() + "/";
    server = lo_server_thread_new(qPrintable(settings.value("osc/server_port").toString()),server_err_callback);
    lo_server_thread_add_method(server,qPrintable(prefix + "led"),"iii",server_led_callback,NULL);
    lo_server_thread_add_method(server,NULL,NULL,server_all_callback,NULL);
    client = lo_address_new(qPrintable(settings.value("osc/client_name").toString()),qPrintable(settings.value("osc/client_port").toString()));

    lo_server_thread_start(server);

    state = RUNNING;
    server_state->setText(status_strings[1].arg(status_names[0],lo_server_thread_get_url(server)));
    client_state->setText(status_strings[1].arg(status_names[1],lo_address_get_url(client)));
}

void OSCWidget::server_err_callback(int num,const char *msg,const char *where) {
    Q_ASSERT( instance );
    qDebug("[osc] error %d %s %s",num,msg,where);
}

int OSCWidget::server_all_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *user_data) {
    Q_ASSERT( instance );
    Q_UNUSED( argv );
    Q_UNUSED( user_data );
    qWarning("[osc] got unhandled %s from %s with %d param(s) of type(s) %s",path,lo_address_get_url(lo_message_get_source(msg)),argc,types);
    return 1;
}

int OSCWidget::server_led_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg, void *user_data) {
    Q_ASSERT( instance );
    Q_UNUSED( user_data );
    Q_UNUSED( path );
    Q_UNUSED( argc );
    Q_UNUSED( types );
    Q_UNUSED( msg );
    emit instance->setLed(argv[0]->i,argv[1]->i,argv[2]->i);
    return 0;
}

void OSCWidget::setRunning(bool run) {
    if (run) startServer();
    else stopServer();
}

