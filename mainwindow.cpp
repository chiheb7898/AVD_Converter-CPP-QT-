#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C://",
                "XLSX File(*.xlsx)"

                );
    if (filename!="")
    {
    QMessageBox::information(this,tr("File Name"),filename);
    ui->progressBar->show();
    ui->progressBar->setValue(0);

    auto excel     = new QAxObject("Excel.Application");
        auto workbooks = excel->querySubObject("Workbooks");
        ui->progressBar->setValue(20);
        auto workbook  = workbooks->querySubObject("Open(const QString&)",filename);
        ui->progressBar->setValue(30);
        auto sheets    = workbook->querySubObject("Worksheets");
        auto sheet     = sheets->querySubObject("Item(int)", 1);
        ui->progressBar->setValue(50);

        QAxObject * range = sheet->querySubObject("UsedRange");
        QAxObject * rows = range->querySubObject( "Rows" );
        ui->progressBar->setValue(100);
        int rowCount = rows->dynamicCall( "Count()" ).toInt();
        QStringListModel *model = new QStringListModel();
           QStringList list;
        for (int r = 2; (r <=rowCount); ++r)
        {
            //auto cCell = sheet->querySubObject("Cells(int,int)",r,1);
            //qDebug() << cCell->dynamicCall("Value()").toString();
            list <<(sheet->querySubObject("Cells(int,int)",r,1)->dynamicCall("Value()").toString())+";"+(sheet->querySubObject("Cells(int,int)",r,2)->dynamicCall("Value()").toString())+";"+(sheet->querySubObject("Cells(int,int)",r,3)->dynamicCall("Value()").toString())+";"+"Fixed"+";"+(sheet->querySubObject("Cells(int,int)",r,5)->dynamicCall("Value()").toString().mid(0,sheet->querySubObject("Cells(int,int)",r,5)->dynamicCall("Value()").toString().size()-4).remove('.'));
            model->setStringList(list);
        }
        ui->progressBar->hide();
        l=list;
       ui->tableView->setModel(model);
       ui->tableView->resizeColumnToContents(0);
    }
    else
        QMessageBox::critical(this,tr("Error"),"     Try Again        ");
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getSaveFileName( this,
                                                     tr("Open File"),
                                                     "C://",
                                                     "Licence File(*.lic)" );
    QFile f(filename);

    if (f.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream s(&f);
        for (int i = 0; i < l.size(); ++i)
          s << l.at(i) << '\n';
       QMessageBox::information(this,tr("File Saved"),filename);

      } else {
       QMessageBox::critical(this,tr("Error"),"Something went wrong!");
      }
      f.close();

    f.close();
}
