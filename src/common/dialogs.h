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
    }

    void setParent(QWidget *parent);
    void setThemeManager(ThemeManager *themeManager);


    class ProgressDialog {
        public:
            ProgressDialog(int min, int max, std::string message);
            void updateValue(int value);
            bool isVisible();
            void close();
        private:
            LoadingDialogWidget *ld;
    };

    class AboutDialog {
        public:
            AboutDialog();
        private:
           AboutDialogWidget *ab;
    };


}

#endif // DIALOGS_H
