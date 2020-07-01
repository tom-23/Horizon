#include "dialogs.h"

namespace dialogs {

void setParent(QWidget *parent) {

    dialogParent = parent;
}

void setThemeManager(ThemeManager *themeManager) {
    dialogThemeManager = themeManager;
}


    void ProgressDialog::show(int min, int max, std::string message) {
        loadingDialogWidget = new LoadingDialogWidget(dialogParent, min, max, QString::fromUtf8(message.c_str()));
        loadingDialogWidget->show();
    }

    void ProgressDialog::updateValue(int value) {
        if (loadingDialogWidget != nullptr) {
            loadingDialogWidget->updateProgress(value);
        }
    }

    bool ProgressDialog::isVisible() {
        return loadingDialogWidget->isVisible();
    }

    void ProgressDialog::close() {
        loadingDialogWidget->close();
    }




    AboutDialog::AboutDialog() {
        ab = new AboutDialogWidget(dialogParent, dialogThemeManager->colorizeSVG(":/svg/svg/logo_base.svg"));
        ab->show();
    }

}
