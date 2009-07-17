#ifndef __SETTINGSWIDGET_H__
#define __SETTINGSWIDGET_H__

#include <QWidget>
#include <QMap>
#include <QGroupBox>
#include <QLineEdit>

class SettingsWidget : public QWidget {
Q_OBJECT
public:
    SettingsWidget(QWidget *parent = NULL);
public slots:
    void saveSettings();
    void loadSettings();
private:
    typedef QMap<QString,QGroupBox*> Groups;
    typedef QMap<QString,QLineEdit*> Entries;

    void addGroup(const QString &group,const QString &label);
    void addSetting(const QString &group,const QString &setting,const QString &label,const QString &value); 

    Groups groups;
    Entries entries;
};

#endif
