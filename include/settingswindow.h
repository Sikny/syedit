#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QTabWidget>
#include <QComboBox>
#include <QBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QListWidget>
#include <QDirIterator>
#include <QMainWindow>

#include "settings.h"

class SettingsWindow : public QWidget
{
Q_OBJECT
public:
    SettingsWindow(QMainWindow* mainWin);
    void buildTabsWidgets();

signals:
    void settingsModified();

public slots:
    void handleNavigation(QListWidgetItem* item);
    void applySettings();

private:
    QTabWidget* tabs;

    QComboBox* themeChoice;

    QMainWindow* mainWindow;
};

#endif // SETTINGS_H
