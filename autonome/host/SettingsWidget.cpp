#include "SettingsWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSettings>

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
    setLayout(new QVBoxLayout(this));

    addGroup("general","General");
    addSetting("general","name","Device Name","autonome");

    addGroup("osc","Open Sound Control");
    addSetting("osc","client_name","Host","localhost");
    addSetting("osc","client_port","Host Port","9999");
    addSetting("osc","server_port","Listen Port","9998");

    dynamic_cast<QBoxLayout*>(layout())->addStretch(1);
    loadSettings();
}

void SettingsWidget::addGroup(const QString &group,const QString &label) {
    if (groups.contains(group)) return;

    QGroupBox *box = new QGroupBox(label,this);
    box->setLayout(new QVBoxLayout(box));
    layout()->addWidget(box);

    groups[group] = box;
}

void SettingsWidget::addSetting(const QString &group,const QString &setting,const QString &label,const QString &value) {
    QString real_setting = group + "/" + setting;
    if (entries.contains(real_setting)) return;

    QGroupBox *box = groups[group];
    Q_ASSERT( box );

    QHBoxLayout *sub_layout = new QHBoxLayout;
    sub_layout->addWidget(new QLabel(label,box),1);
    QLineEdit *line = new QLineEdit(value,box);
    connect(line,SIGNAL( editingFinished() ),this,SLOT( saveSettings() ));
    sub_layout->addWidget(line);
    dynamic_cast<QBoxLayout*>(box->layout())->addLayout(sub_layout);
    
    entries[group + "/" + setting] = line;
}

void SettingsWidget::saveSettings() {
    QSettings settings;
    settings.clear();

    for (Entries::const_iterator i=entries.constBegin(); i!=entries.constEnd(); i++) {
        qDebug("[settings] saving %s %s",qPrintable(i.key()),qPrintable(i.value()->text()));
        settings.setValue(i.key(),i.value()->text());
    }
}

void SettingsWidget::loadSettings() {
    QSettings settings;

    for (Entries::const_iterator i=entries.constBegin(); i!=entries.constEnd(); i++) if (settings.contains(i.key())) {
        qDebug("[settings] loading %s %s",qPrintable(i.key()),qPrintable(settings.value(i.key()).toString()));
        i.value()->setText(settings.value(i.key()).toString());
    }

}
