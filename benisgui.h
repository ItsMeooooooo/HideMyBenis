#ifndef BENISGUI_H
#define BENISGUI_H

#include <QMainWindow>

namespace Ui {
class BenisGUI;
}

class BenisGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit BenisGUI(QWidget *parent = 0);
    ~BenisGUI();

private slots:

    void on_browseInputImage_clicked();

    void on_browseInputFile_clicked();

    void on_browseOutputImage_clicked();

    void on_browseOutputFile_clicked();

    void on_hideButton_clicked();

    void on_extractButton_clicked();

private:
    Ui::BenisGUI *ui;
};

#endif // BENISGUI_H
