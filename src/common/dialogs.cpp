#include "dialogs.h"

namespace dialogs {

void setParent(QWidget *parent) {

    dialogParent = parent;
}

void setThemeManager(ThemeManager *themeManager) {
    dialogThemeManager = themeManager;
}

    ProgressDialog::ProgressDialog(int min, int max, std::string message) {
        ld = new LoadingDialogWidget(dialogParent, min, max, QString::fromUtf8(message.c_str()));
        ld->show();
    }

    void ProgressDialog::updateValue(int value) {
        if (ld != nullptr) {
            ld->updateProgress(value);
        }
    }

    void ProgressDialog::close() {
        if (ld != nullptr) {
            ld->close();
        }
    }

    bool ProgressDialog::isVisible() {
        return ld->isVisible();
    }

    AboutDialog::AboutDialog() {
        ab = new AboutDialogWidget(dialogParent, dialogThemeManager->colorizeSVG(":/svg/svg/logo_base.svg"));
        ab->show();
    }

}
