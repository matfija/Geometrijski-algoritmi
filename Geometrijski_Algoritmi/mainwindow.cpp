#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      _pAlgoritamBaza(nullptr),
      _imeDatoteke(""),
      _duzinaPauze(DUZINA_PAUZE),
      _broj_nasumicnih_tacaka(BROJ_NASUMICNIH_TACAKA)
{
    ui->setupUi(this);
    ui->tipAlgoritma->insertSeparator(static_cast<int>(TipAlgoritma::SEPARATOR));
    animacijaButtonAktivni(false);
    animacijaParametriButtonAktivni(true);

    _pOblastCrtanjaOpenGL = ui->openGLWidget;

    _pOblastCrtanja = new OblastCrtanja(this);
    QBoxLayout *oblastCrtanjaLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    oblastCrtanjaLayout->addWidget(_pOblastCrtanja);
    ui->tab->setLayout(oblastCrtanjaLayout);

    ui->brojNasumicniTacaka->setPlaceholderText("Uneti broj nasumicnih tacaka, podrazumevana vrednost je 20.");

    connect(_pOblastCrtanja, &OblastCrtanja::polozajKursora,
            this, &MainWindow::on_polozajKursora);
    connect(ui->tabWidget, &QTabWidget::currentChanged,
            this, [this](int) {ui->statusBar->showMessage("");});

    /* Add chart */
    QChart *chart = new QChart();
    _optimalSeries->append(0,0);
    _naiveSeries->append(0,0);

    _optimalSeries->setName("Optimalni algoritam");
    _naiveSeries->setName("Naivni algoritam");

    chart->addSeries(_optimalSeries);
    chart->addSeries(_naiveSeries);

    chart->legend()->show();

    chart->createDefaultAxes();
    chart->setTitle("Poredjenje efikasnosti");

    chart->axes(Qt::Horizontal).back()->setRange(0, X_MAX_VAL);
    chart->axes(Qt::Vertical).back()->setRange(0, Y_MAX_VAL);

    // Same formatting
    chart->setBackgroundVisible(false);
    chart->setPlotAreaBackgroundVisible(true);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QBoxLayout *chartBoxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    chartBoxLayout->addWidget(chartView);

    ui->tab_2->setLayout(chartBoxLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::animacijaButtonAktivni(bool param_aktivnosti)
{
    ui->Ispocetka_dugme->setEnabled(param_aktivnosti);
    ui->Sledeci_dugme->setEnabled(param_aktivnosti);
    ui->Zaustavi_dugme->setEnabled(param_aktivnosti);
    ui->Zapocni_dugme->setEnabled(param_aktivnosti);
    ui->merenjeButton->setEnabled(param_aktivnosti);
}

void MainWindow::animacijaParametriButtonAktivni(bool param_aktivnosti)
{
    ui->datoteka_dugme->setEnabled(!param_aktivnosti);
    ui->Ponisti_dugme->setEnabled(param_aktivnosti);
    ui->tipAlgoritma->setEnabled(param_aktivnosti);
    ui->Nasumicni_dugme->setEnabled(!param_aktivnosti);
}

void MainWindow::on_datoteka_dugme_clicked()
{
    QString imeDatoteke = QFileDialog::getOpenFileName(this,
                              tr("Datoteka sa koordinatama tacaka"), "./ulazni_podaci/", "*.*");
    if (imeDatoteke.isEmpty())
        return;

    _imeDatoteke = imeDatoteke.toStdString();

    napraviNoviAlgoritam();
    ui->Zapocni_dugme->setEnabled(true);
}

void MainWindow::on_Nasumicni_dugme_clicked()
{
    _imeDatoteke = "";
    if (ui->brojNasumicniTacaka->text() != "" )
        _broj_nasumicnih_tacaka = ui->brojNasumicniTacaka->text().toInt();

    napraviNoviAlgoritam();
    ui->Zapocni_dugme->setEnabled(true);
    ui->Ispocetka_dugme->setEnabled(false);
}

void MainWindow::on_Ponisti_dugme_clicked()
{
    ui->tipAlgoritma->setCurrentIndex(0);
    animacijaButtonAktivni(false);
    animacijaParametriButtonAktivni(true);

    _pOblastCrtanja->set_obrisiSve(true);
    _pOblastCrtanja->update();

    _pOblastCrtanjaOpenGL->set_obrisiSve(true);
    _pOblastCrtanjaOpenGL->update();

    ui->brojNasumicniTacaka->clear();
    ui->brojNasumicniTacaka->setPlaceholderText("Uneti broj nasumicnih tacaka, podrazumevana vrednost je 20.");

    _imeDatoteke = "";
    _optimalSeries->clear();
    _naiveSeries->clear();
}

void MainWindow::on_Zapocni_dugme_clicked()
{
    animacijaButtonAktivni(true);
    ui->Zapocni_dugme->setEnabled(false);
    animacijaParametriButtonAktivni(true);
    ui->merenjeButton->setEnabled(false);
    ui->naivniCheck->setEnabled(false);

    if (_pAlgoritamBaza)
        _pAlgoritamBaza->pokreniAnimaciju();
}

void MainWindow::on_Zaustavi_dugme_clicked()
{
    if (_pAlgoritamBaza)
        _pAlgoritamBaza->pauzirajIliNastaviAnimaciju();
}

void MainWindow::on_Sledeci_dugme_clicked()
{
    if (_pAlgoritamBaza)
        _pAlgoritamBaza->sledeciKorakAnimacije();
}

void MainWindow::on_Ispocetka_dugme_clicked()
{
    if (_pAlgoritamBaza)
    {
        _pAlgoritamBaza->zaustaviAnimaciju();
        napraviNoviAlgoritam();
        on_Zapocni_dugme_clicked();
    }
}

void MainWindow::on_polozajKursora(int x, int y)
{
    QString s;
    QTextStream(&s) << "Trenutni polozaj misa: (" << x << ", " << y << ").";
    ui->statusBar->showMessage(s);
}

void MainWindow::on_tipAlgoritma_currentIndexChanged(int index)
{
    TipAlgoritma tipAlgoritma = static_cast<TipAlgoritma>(index);
    if (tipAlgoritma == TipAlgoritma::ALGORITMI_SA_VEZBI ||
        tipAlgoritma == TipAlgoritma::STUDENTSKI_PROJEKTI)
    {
        ui->datoteka_dugme->setEnabled(false);
        ui->Nasumicni_dugme->setEnabled(false);
        animacijaButtonAktivni(false);
    }
    else
    {
        ui->datoteka_dugme->setEnabled(true);
        ui->Nasumicni_dugme->setEnabled(true);
        animacijaButtonAktivni(false);
        ui->merenjeButton->setEnabled(true);

        if (tipAlgoritma == TipAlgoritma::_3D_ISCRTAVANJE ||
            tipAlgoritma == TipAlgoritma::KONVEKSNI_OMOTAC_3D)
        {
            ui->tabWidget->setCurrentIndex(TabIndex::ALGORITAM_3D);
        } else {
            ui->tabWidget->setCurrentIndex(TabIndex::ALGORITAM_2D);
        }
    }
}

/* za Chart, poredjenje. */
void MainWindow::on_merenjeButton_clicked()
{
    ui->merenjeButton->setEnabled(false);
    _optimalSeries->clear();
    _naiveSeries->clear();

    ui->tabWidget->setCurrentIndex(TabIndex::POREDJENJE);
    TipAlgoritma tipAlgoritma = static_cast<TipAlgoritma>(ui->tipAlgoritma->currentIndex());

    _mThread = new TimeMeasurementThread(tipAlgoritma, MIN_DIM, STEP, MAX_DIM);
    connect(_mThread, &TimeMeasurementThread::updateChart, this, &MainWindow::on_lineSeriesChange);
    connect(_mThread, &TimeMeasurementThread::finishChart, this, &MainWindow::on_chartFinished);
    _mThread->start();
}

void MainWindow::on_lineSeriesChange(double dim, double optimal, double naive)
{
    _optimalSeries->append(dim, optimal);
    _naiveSeries->append(dim, naive);
}

void MainWindow::on_chartFinished()
{
    ui->merenjeButton->setEnabled(true);
}

void MainWindow::na_krajuAnimacije()
{
    animacijaParametriButtonAktivni(true);
    animacijaButtonAktivni(false);
    ui->datoteka_dugme->setEnabled(true);
    ui->Nasumicni_dugme->setEnabled(true);
    ui->merenjeButton->setEnabled(true);
    ui->naivniCheck->setEnabled(true);
    ui->Ispocetka_dugme->setEnabled(true);
}

void MainWindow::napraviNoviAlgoritam()
{
    _pOblastCrtanja->set_obrisiSve(false);
    _pOblastCrtanjaOpenGL->set_obrisiSve(false);

    delete _pAlgoritamBaza;
    _pAlgoritamBaza = nullptr;

    const auto naivni = ui->naivniCheck;

    /* Ovde se kreiraju instance algoritama pozivom njihovih konstruktora. Svi
       2D algoritmi crtaju po _pOblastCrtanja, a 3D po _pOblastCrtanjaOpenGL. */
    TipAlgoritma tipAlgoritma = static_cast<TipAlgoritma>(ui->tipAlgoritma->currentIndex());
    switch (tipAlgoritma) {
    case TipAlgoritma::DEMO_ISCRTAVANJA:
        _pAlgoritamBaza = new DemoIscrtavanja(_pOblastCrtanja, _duzinaPauze, naivni,
                                              _imeDatoteke, _broj_nasumicnih_tacaka);
        break;
    case TipAlgoritma::BRISUCA_PRAVA:
        _pAlgoritamBaza = new BrisucaPrava(_pOblastCrtanja, _duzinaPauze, naivni,
                                           _imeDatoteke, _broj_nasumicnih_tacaka);
        break;
    case TipAlgoritma::_3D_ISCRTAVANJE:
        _pAlgoritamBaza = new Discrtavanje(_pOblastCrtanjaOpenGL, _duzinaPauze, naivni,
                                           _imeDatoteke, _broj_nasumicnih_tacaka);
        break;
    case TipAlgoritma::KONVEKSNI_OMOTAC:
        _pAlgoritamBaza = new konveksniomotac(_pOblastCrtanja, _duzinaPauze, naivni,
                                              _imeDatoteke, _broj_nasumicnih_tacaka);
        break;
    case TipAlgoritma::KONVEKSNI_OMOTAC_3D:
        _pAlgoritamBaza = new KonveksniOmotac3D(_pOblastCrtanjaOpenGL, _duzinaPauze, naivni,
                                                _imeDatoteke, _broj_nasumicnih_tacaka);
        break;
    case TipAlgoritma::PRESECI_DUZI:
        _pAlgoritamBaza = new PreseciDuzi(_pOblastCrtanja, _duzinaPauze, naivni,
                                          _imeDatoteke, _broj_nasumicnih_tacaka);
        break;
    case TipAlgoritma::PRESEK_PRAVOUGAONIKA:
        _pAlgoritamBaza = new PresekPravougaonika(_pOblastCrtanja, _duzinaPauze, naivni,
                                                  _imeDatoteke, _broj_nasumicnih_tacaka);
        break;
    default: /* ako nije algoritam uopste */
        break;
    }

    if (_pAlgoritamBaza)
    {
        _pOblastCrtanja->postaviAlgoritamKojiSeIzvrsava(_pAlgoritamBaza);
        _pOblastCrtanja->update();

        _pOblastCrtanjaOpenGL->postaviAlgoritamKojiSeIzvrsava(_pAlgoritamBaza);
        _pOblastCrtanjaOpenGL->update();

        connect(_pAlgoritamBaza, &AlgoritamBaza::animacijaZavrsila, this, &MainWindow::na_krajuAnimacije);
    }
}
