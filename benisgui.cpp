#include "benisgui.h"
#include "ui_benisgui.h"
#include <QFileDialog>
#include <QMessageBox>
#include "image.h"
#include "benis_crypt.h"
#include <fstream>

using namespace HideMyBenis;

BenisGUI::BenisGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BenisGUI)
{
    ui->setupUi(this);
    ui->passwordInput->setPlaceholderText("Password");
    ui->passwordInput->setEchoMode(QLineEdit::Password);
    ui->passwordInput->setText("");

    ui->inputImage->setPlaceholderText("Select input image...");
    ui->inputImage->setText("");

    ui->inputFile->setPlaceholderText("Select input file...");
    ui->inputFile->setText("");

    ui->outputFile->setPlaceholderText("Select output file...");
    ui->outputFile->setText("");

    ui->outputImage->setPlaceholderText("Select output image...");
    ui->outputImage->setText("");
}

BenisGUI::~BenisGUI()
{
    delete ui;
}

void BenisGUI::on_browseInputImage_clicked()
{
    QString imageName = QFileDialog::getOpenFileName(this, tr("Open Image"), QString(),
                tr("Image Files (*.png *.jpg *.jpeg)"));
    ui->inputImage->setText(imageName);
}

void BenisGUI::on_browseInputFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString());
    ui->inputFile->setText(fileName);
}

void BenisGUI::on_browseOutputImage_clicked()
{
    QString outputName = QFileDialog::getSaveFileName(this, tr("Save Image"), QString(),
                tr("Image files (*.png)"));
    ui->outputImage->setText(outputName);
}

void BenisGUI::on_browseOutputFile_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString());
    ui->outputFile->setText(fileName);
}

void BenisGUI::on_hideButton_clicked()
{
    try
    {
        Image img(ui->inputImage->text().toStdString());
        std::ifstream input(ui->inputFile->text().toStdString(), std::ios::binary);
        if(!input)
        {
            throw std::runtime_error("ERROR: " + ui->inputFile->text().toStdString() + "\nCould not open input file!");
        }
        encrypt(img, input, ui->passwordInput->text().toStdString(), ui->fastMode->isChecked());
        img.writeToFile(ui->outputImage->text().toStdString());
        QMessageBox::information(this, tr("Success"), tr("Successfully hid and encrypted your data."));
    }
    catch(std::runtime_error& e) { QMessageBox::critical(this, tr("Error"), tr(e.what())); }
}

void BenisGUI::on_extractButton_clicked()
{
    try
    {
        Image img(ui->inputImage->text().toStdString());
        std::ofstream output(ui->outputFile->text().toStdString(), std::ios::binary);
        if (!output)
        {
            throw std::runtime_error("ERROR: " + ui->outputFile->text().toStdString() + "\nCould not save output file!");
        }
        decrypt(img, ui->passwordInput->text().toStdString(), output, ui->fastMode->isChecked());
        QMessageBox::information(this, tr("Success"), tr("Successfully extracted and decrypted your data."));
    }
    catch(std::runtime_error& e) { QMessageBox::critical(this, tr("Error"), tr(e.what())); }
}
