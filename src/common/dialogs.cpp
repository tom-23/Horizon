#include "dialogs.h"

namespace dialogs {

void setParent(QWidget *parent) {

    dialogParent = parent;
}

void setThemeManager(ThemeManager *themeManager) {
    dialogThemeManager = themeManager;
}

ThemeManager *getThemeManager() {
    return dialogThemeManager;
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

    int ProgressDialog::getValue() {
        return loadingDialogWidget->getProgress();
    }

    int ProgressDialog::getMax() {
        return loadingDialogWidget->getMaxProgress();
    }




    AboutDialog::AboutDialog() {
        ab = new AboutDialogWidget(dialogParent, dialogThemeManager->colorizeSVG(":/svg/svg/logo_base.svg"));
        ab->show();
    }

    int MessageDialog::show(std::string title, std::string message, icons icon, buttons buttonType) {
        bool okayOnly = false;
        bool okayCancel = false;
        bool yesNo = false;
        bool yesNoCancel = false;

        QString iconSVG;

        switch (buttonType) {
         case MessageDialog::buttons::okOnly:
            okayOnly = true;
            break;
        case MessageDialog::buttons::okCancel:
           okayCancel = true;
           break;
        case MessageDialog::buttons::yesNo:
           yesNo = true;
           break;
        case MessageDialog::buttons::yesNoCancel:
           yesNoCancel = true;
           break;

        }

        switch (icon) {
        case MessageDialog::icons::caution:
         iconSVG = dialogThemeManager->colorizeSVG(":/svg/svg/caution.svg");
         break;
        case MessageDialog::icons::info:
         iconSVG = dialogThemeManager->colorizeSVG(":/svg/svg/info.svg");
         break;
        case MessageDialog::icons::no:
         iconSVG = dialogThemeManager->colorizeSVG(":/svg/svg/no.svg");
         break;
        }


        messageDialogWidget = new MessageDialogWidget(dialogParent, QString::fromStdString(title), QString::fromStdString(message), iconSVG, okayOnly, okayCancel, yesNo, yesNoCancel);
        return messageDialogWidget->exec();
        };



    }


