#ifndef DIALOGS_H
#define DIALOGS_H
#include "string"
#include "iostream"
#include "gui/loadingdialogwidget.h"
#include "gui/aboutdialogwidget.h"
#include "common/thememanager.h"

#include <QWidget>

namespace dialogs {

    namespace  {
        QWidget *dialogParent;
        ThemeManager *dialogThemeManager;
        LoadingDialogWidget *loadingDialogWidget;

    }

    void setParent(QWidget *parent);

    void setThemeManager(ThemeManager *themeManager);


    class ProgressDialog {
    public:
        static void updateValue(int value);
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


}

#endif // DIALOGS_H
