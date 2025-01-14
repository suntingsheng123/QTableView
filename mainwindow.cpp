#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QLabel>
#include<QStandardItemModel>
#include<QItemSelectionModel>
#include<QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->splitter);
    labelCurFile=new QLabel("当前文件",this);
    labelCurFile->setMinimumWidth(200);
    labelCellPos=new QLabel("当前单元格",this);
    labelCellPos->setMinimumWidth(200);
    labelCellText=new QLabel("单元格内容",this);
    labelCellText->setMinimumWidth(200);
    ui->statusbar->addWidget(labelCurFile);
    ui->statusbar->addWidget(labelCellPos);
    ui->statusbar->addWidget(labelCellText);
    m_model= new QStandardItemModel(2,fixedColumCount,this);
    m_selection=new QItemSelectionModel(m_model,this);

    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionModel(m_selection);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    connect(m_selection,&QItemSelectionModel::currentChanged,
            this,&MainWindow::do_currentChanged);
}
void MainWindow::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if(current.isValid())
    {
        labelCellPos->setText(QString::asprintf("当前单元格：%d行，%d列",
                                                current.row(),current.column()));
        QStandardItem *aItem=m_model->itemFromIndex(current);
        labelCellText->setText("单元格内容"+aItem->text());
        QFont font=aItem->font();
        ui->actTextBold->setChecked(font.bold());
    }
}

void MainWindow::iniModelData(QStringList &aFileContent)
{
    int rowCnt=aFileContent.size();

    m_model->setRowCount(rowCnt-1);
    QString header=aFileContent.at(0);

    QStringList headList=header.split( QRegularExpression(R"(\s+)"),Qt::SkipEmptyParts);
    m_model->setHorizontalHeaderLabels(headList);
    QStandardItem *aItem;
    int j;
    for(int i=1;i<rowCnt;i++)
    {
        QString aLineText=aFileContent.at(i);
        QStringList tempList=aLineText.split( QRegularExpression(R"(\s+)"),Qt::SkipEmptyParts);
        for(j=0;j<fixedColumCount-1;j++)
        {
            aItem=new QStandardItem(tempList.at(j));
            m_model->setItem(i-1,j,aItem);
        }
        aItem=new QStandardItem(headList.at(j));
        aItem->setBackground(QBrush(Qt::yellow));
        if(tempList.at(j)=="0")
            aItem->setCheckState(Qt::Unchecked);
        else
            aItem->setCheckState(Qt::Checked);
        m_model->setItem(i-1,j,aItem);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString curPath=QCoreApplication::applicationDirPath();
    QString aFileName=QFileDialog::getOpenFileName(this,"打开一个文件",curPath,
                                                     "数据文件(*.txt);;所有文件(*.*)");
    if(aFileName.isEmpty())
        return;
    QFile aFile(aFileName);
    if(!aFile.open(QIODevice::ReadOnly|QIODevice::Text)) return ;
    QStringList    aFileContent;
    ui->plainTextEdit->clear();
    QTextStream aStream(&aFile);
    while(!aStream.atEnd())
    {
        QString str=aStream.readLine();
        ui->plainTextEdit->appendPlainText(str);
        aFileContent.append(str);
    }

    aFile.close();
    labelCurFile->setText("当前文件"+aFileName);
    ui->actAppend->setEnabled(true);
    ui->actInsert->setEnabled(true);
    ui->actSave->setEnabled(true);
    ui->actDelete->setEnabled(true);


    iniModelData(aFileContent);

}


void MainWindow::on_actModelData_triggered()
{
    ui->plainTextEdit->clear();
    QStandardItem *aItem;
    QString str;
    for(int i=0;i<m_model->columnCount();i++)
    {
        aItem=m_model->horizontalHeaderItem(i);
        str+=aItem->text();
        str+="\t";
    }
    ui->plainTextEdit->appendPlainText(str);
    for(int i=0;i<m_model->rowCount();i++)
    {
        str="";
        for(int j=0;j<m_model->columnCount()-1;j++)
        {
            aItem=m_model->item(i,j);
            str+=aItem->text();
            str+="\t";

        }
        aItem=m_model->item(i,m_model->columnCount()-1);
        if(aItem->checkState()==Qt::Checked)
        {
            str+="是";
        }
        else
        {
            str+="否";
        }
        ui->plainTextEdit->appendPlainText(str);
    }
}


void MainWindow::on_actAppend_triggered()
{
    QList<QStandardItem*> aItemList;
    QStandardItem *aItem;
    for(int i=0;i<m_model->columnCount()-1;i++)
    {
        aItem=new QStandardItem("0");
        aItemList<<aItem;
    }
    QString str=m_model->headerData(m_model->columnCount()-1,Qt::Horizontal).toString();
    aItem=new QStandardItem(str);
    aItem->setCheckable(true);
    aItem->setBackground(QBrush(Qt::yellow));
    aItemList<<aItem;
    m_model->insertRow(m_model->rowCount(),aItemList);
    m_selection->clearSelection();
    m_selection->setCurrentIndex( m_model->index(m_model->rowCount()-1,0),
                                 QItemSelectionModel::Select);

}


void MainWindow::on_actInsert_triggered()
{
    QList<QStandardItem*> aItemList;
    QStandardItem *aItem;
    for(int i=0;i<m_model->columnCount()-1;i++)
    {
        aItem=new QStandardItem("0");
        aItemList<<aItem;
    }
    QString str=m_model->headerData(m_model->columnCount()-1,Qt::Horizontal).toString();
    aItem=new QStandardItem(str);
    aItem->setCheckable(true);
    aItem->setBackground(QBrush(Qt::yellow));
    aItemList<<aItem;
    QModelIndex curIndex=m_selection->currentIndex();
    m_model->insertRow(curIndex.row(),aItemList);
    m_selection->clearSelection();
    m_selection->setCurrentIndex( curIndex,
                                 QItemSelectionModel::Select);
}


void MainWindow::on_actDelete_triggered()
{
    QModelIndex curIndex=m_selection->currentIndex();
    if(curIndex.row()!=m_model->rowCount()-1)
    {
        m_model->removeRow(curIndex.row());
        m_selection->setCurrentIndex(curIndex,QItemSelectionModel::Select);
    }
    else
    {
        m_model->removeRow(curIndex.row());
    }
}


void MainWindow::on_actAlignLeft_triggered()
{
    if(!m_selection->hasSelection()) return;
    QModelIndexList indexList=m_selection->selectedIndexes();
    for(auto aIndex:indexList)
    {
        m_model->itemFromIndex(aIndex)->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    }


}


void MainWindow::on_actAlignCenter_triggered()
{
    if(!m_selection->hasSelection()) return;
    QModelIndexList indexList=m_selection->selectedIndexes();
    for(auto aIndex:indexList)
    {
        m_model->itemFromIndex(aIndex)->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    }
}


void MainWindow::on_actAlignRight_triggered()
{
    if(!m_selection->hasSelection()) return;
    QModelIndexList indexList=m_selection->selectedIndexes();
    for(auto aIndex:indexList)
    {
        m_model->itemFromIndex(aIndex)->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    }
}


void MainWindow::on_actTextBold_triggered(bool checked)
{
    if(!m_selection->hasSelection()) return;
    QModelIndexList indexList=m_selection->selectedIndexes();
    for(auto aIndex:indexList)
    {
        QFont font=m_model->itemFromIndex(aIndex)->font();
        font.setBold(checked);
        m_model->itemFromIndex(aIndex)->setFont(font);
    }
}

