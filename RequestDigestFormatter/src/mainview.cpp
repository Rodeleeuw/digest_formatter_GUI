#include "mainview.h"
#include "ui_mainview.h"
#include "pch.hpp"
#include "customdatatypes.hpp"
#include "ExcelReader.hpp"


/* Constructor */
MainView::MainView(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainView)
{
    ui->setupUi(this);
    // disable the stop button when the application start
    //ui->btnStopServer->setEnabled(false);
    // start the ExchangeData slot when the signal newConnection is emmitted
    //connect(m_server, &QTcpServer::newConnection, this, &MainView::ExchangeData);

}

/* Destructor */
MainView::~MainView()
{
    delete ui;
}


/* ----------------------- class functions --------------------- */

/* This method fills the list of the vacancies read
 * from the diggest.txt */
void MainView::PopulateVacanciesList(std::string digest_file)
{
    // open and parse the request file
    ExcelReader reader( digest_file );
    reader.FillVacancies(m_vacancies);
    // Fill the list
    std::string indexed_name;
    for( auto& vacancy : m_vacancies )
    {
        indexed_name = std::to_string( vacancy.index ) + ". " + vacancy.name;
        // You need to convert the std string to QString first
        ui->listWidget->addItem( QString::fromStdString( indexed_name ) );
    }

    // Select the first item in the list
    ui->listWidget->setCurrentRow(0);
}


/* ------------------------- Call backs -------------------------- */
void MainView::on_btnOpenFile_clicked()
{
    // Filter for the file selection
    QString filter = "All files (*.*) ;; Excel files (*.xlsx)";
   // when clicked open a new window to select a file, the path is set to the home path
    QString digest_file = QFileDialog::getOpenFileName( this, "Open the reques file", QDir::homePath(), filter );
    // Fill the vacancies, convert to std string first
    PopulateVacanciesList( digest_file.toStdString() );
    // Display a message box
    //QMessageBox::information( this, "Some message", digest_file );
}


void MainView::on_btnCloseApp_clicked()
{
   // close the application
    exit(0);
}

void MainView::on_listWidget_itemSelectionChanged()
{
   int index = 0;
   // Get the index of the current row
   index = ui->listWidget->currentRow();
   // Create the message to display
   QString vacancy_description = QString::fromStdString( m_vacancies[index].description );
    // Update the Description fields
   ui->textBrowser->setText( vacancy_description );
   ui->label_company->setText( QString::fromStdString( "Company: " + m_vacancies[ index ].company ) );
   ui->label_owner->setText( QString::fromStdString( "Contact: " + m_vacancies[ index ].owner ) );
}
