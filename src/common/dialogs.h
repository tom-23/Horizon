#ifndef DIALOGS_H
#define DIALOGS_H
#include "string"
#include "iostream"
#include "gui/loadingdialogwidget.h"
#include "gui/aboutdialogwidget.h"
#include "gui/messagedialogwidget.h"
#include "common/thememanager.h"

#include <QWidget>
#include <QDebug>
#include <QGraphicsEffect>
#include <QBoxLayout>
#include <QLabel>

namespace dialogs {

    namespace  {
        QWidget *dialogParent;
        ThemeManager *dialogThemeManager;
        LoadingDialogWidget *loadingDialogWidget;
        MessageDialogWidget *messageDialogWidget;

    }

    void setParent(QWidget *parent);

    void setThemeManager(ThemeManager *themeManager);
    ThemeManager* getThemeManager();

    class ProgressDialog {
    public:
        static void updateValue(int value);
        static int getValue();
        static int getMax();
        static bool isVisible();
        static void show(int min, int max, std::string message);
        static void close();
    };

    class AboutDialog {
        public:
            AboutDialog();
        private:
           AboutDialogWidget *ab;
    };

    class MessageDialog {
    public:
        enum buttons {okOnly, okCancel, yesNo, yesNoCancel};
        enum icons {caution, info, no};
        static int show(std::string title, std::string message, icons icon, buttons buttonType);

    };


}

#endif // DIALOGS_H
