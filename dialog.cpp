#include "dialog.h"
#include "ui_dialog.h"
#include"employe.h"
#include <QMessageBox>
#include "QApplication"
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QTextDocument>
#include <QAbstractItemModel>
#include <QFileDialog>
#include <QMap>
#include <cstddef> // Nécessaire pour std::byte
#include <cstddef>
#include<QtCore>

#include<QtGui>
#include<QtWidgets>
#include<QtCharts/QtCharts>
#include <algorithm>








Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    emp()





{

    ui->setupUi(this);  // Call this only once
     highlightEventDates();
    // Connect signals and slots
    connect(ui->addEvent, &QPushButton::clicked, this, &Dialog::addEvent);
    connect(ui->calendar, &QCalendarWidget::clicked, this, &Dialog::showEvents);
    connect(ui->clearEvent, &QPushButton::clicked, this, &Dialog::clearEvent);
    connect(ui->showChar, &QPushButton::clicked, this, &Dialog::showSalaryDistribution);
    QVBoxLayout *chartLayout = new QVBoxLayout(ui->chartPlaceholder);
    ui->chartPlaceholder->setLayout(chartLayout);

    connect(ui->seniorityAnalysisButton, &QPushButton::clicked, this, &Dialog::on_seniorityAnalysis_clicked);






    // Initialize the event map
    events = QMap<QDate, QStringList>();


    // table mta3 l affiche
    ui->tableView->setModel(emp.afficher());
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_8_clicked()
{
    // Retrieve ID as a QString to validate it before converting to int
    QString id_str = ui->line_id->text();


    if (id_str.length() != 4 || !id_str.toInt()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID doit être un entier de 4 chiffres.");
        return;
    }


    QDate date = ui->line_date->date();
    QDate currentDate = QDate::currentDate();
    if (date > currentDate) {
        QMessageBox::warning(this, "Erreur de saisie", "La date d'embauche doit être dans le passé ou aujourd'hui.");
        return;
    }


    QString nom = ui->line_nom->text();
    QString prenom = ui->line_prenom->text();


    if (nom.isEmpty() || !nom.at(0).isUpper()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le nom doit commencer par une majuscule.");
        return;
    }


    if (prenom.isEmpty() || !prenom.at(0).isUpper()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le prénom doit commencer par une majuscule.");
        return;
    }


    int id = id_str.toInt();
    QString poste = ui->line_poste->text();
    int salaire = ui->line_salaire->text().toInt();
    employe P(id, nom, prenom, date, poste, salaire);

    bool test = P.ajouter();

    if (test) {

        ui->tableView->setModel(emp.afficher());
        QMessageBox::information(this, "Succès", "Ajout effectué avec succès");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout");
    }
}



void Dialog::on_pushButton_12_clicked()
{

    int id = ui->line_IDS->text().toInt();





        bool test = emp.supprimer(id);
        if (test) {
            //refresh
            ui->tableView->setModel(emp.afficher());
            QMessageBox::information(nullptr, QObject::tr("ok"),
                                     QObject::tr("Suppression effectuee "), QMessageBox::Cancel);
        }
        else
            QMessageBox::critical(nullptr, QObject::tr("NOT ok"),
                                  QObject::tr("Suppression non effectuee "), QMessageBox::Cancel);






}




void Dialog::on_mon_clicked()
{
    int id = ui->line_id_6->text().toInt();
    QString nom = ui->line_nom_6->text();
    QString prenom = ui->line_prenom_6->text();
    QDate date = ui->line_date_6->date();
    QString poste = ui->line_poste_6->text();
    int salaire = ui->line_salaire_6->text().toInt();


    employe emp(id, nom, prenom, date, poste, salaire);
    bool test = emp.modifier(id, nom, prenom, date, poste, salaire);

    // Refresh kl 3ada
    ui->tableView->setModel(emp.afficher());

    QMessageBox msgBox;
    if (test) {
        msgBox.setText("Employee modified successfully!");
    } else {
        msgBox.setText("Failed to modify employee.");
    }

    msgBox.exec();
}


void Dialog::on_exporterr_clicked()
{
    // Get the file path from the user
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le fichier", "", "*.pdf");
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun fichier spécifié pour l'exportation.");
        return;
    }

    // Set up the printer for PDF export
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    // Prepare the QTextDocument to hold the table data
    QTextDocument document;
    QString html;

    // Add a title to the PDF
    html += "<h1>Export de la table des employés</h1><br>";

    // Extract table data from QTableView and convert to HTML
    html += "<table border='1' cellspacing='0' cellpadding='4'>";

    // Get the model from the table view
    QAbstractItemModel *model = ui->tableView->model();
    if (!model) {
        QMessageBox::warning(this, "Erreur", "Impossible de récupérer les données de la table.");
        return;
    }

    // Table headers
    html += "<tr>";
    for (int column = 0; column < model->columnCount(); ++column) {
        html += "<th>" + model->headerData(column, Qt::Horizontal).toString() + "</th>";
    }
    html += "</tr>";

    // Table rows
    for (int row = 0; row < model->rowCount(); ++row) {
        html += "<tr>";
        for (int column = 0; column < model->columnCount(); ++column) {
            QString data = model->data(model->index(row, column)).toString();
            html += "<td>" + data + "</td>";
        }
        html += "</tr>";
    }

    html += "</table>";

    // Set the HTML to the document
    document.setHtml(html);

    // Print the document to the PDF
    document.print(&printer);

    // Confirm export success
    QMessageBox::information(this, "Succès", "Le contenu a été exporté en PDF avec succès.");

}


void Dialog::on_trier_clicked()
{
    // Get the selected sorting option from the combo box
    QString sortOption = ui->comboBox->currentText(); // Assuming the combo box is named comboBox_sort

    // Create a new model to display sorted data
    QSqlQueryModel *model = new QSqlQueryModel();

    // Determine the query based on the selected sorting option
    QString query;
    if (sortOption == "salaire") {
        query = "SELECT * FROM EMPLOYES ORDER BY SALAIRE DESC";
    } else if (sortOption == "date") {
        query = "SELECT * FROM EMPLOYES ORDER BY DATE_EMBAUCHE DESC";
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une option de tri valide.");
        return;
    }

    // Set the query to the model
    model->setQuery(query);

    // Check if the query executed successfully
    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Erreur", "Échec du tri des employés.");
        return;
    }

    // Set the sorted model to the table view
    ui->tableView->setModel(model);

    // Optional: Set headers if needed
    model->setHeaderData(0, Qt::Horizontal, "ID Employé");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Date Embauche");
    model->setHeaderData(4, Qt::Horizontal, "Poste");
    model->setHeaderData(5, Qt::Horizontal, "Salaire");

    // Inform the user that sorting was successful
    QMessageBox::information(this, "Succès", "Les employés ont été triés par " + sortOption + ".");

}


void Dialog::on_rechercher_clicked()
{
    // Get the text entered in the search line edit
    QString searchText = ui->line_recherche->text();

    QSqlQueryModel *model = new QSqlQueryModel();

    // If the search field is empty, load the full table
    if (searchText.isEmpty()) {
        model->setQuery("SELECT * FROM EMPLOYES");
    } else {
        // Prepare the query to filter employees by ID starting with the entered text
        QSqlQuery query;

        // Use LIKE directly with the numeric field
        QString queryStr = "SELECT * FROM EMPLOYES WHERE ID_EMPLOYE LIKE :searchText";
        query.prepare(queryStr);
        query.bindValue(":searchText", searchText + "%");  // Matches IDs starting with searchText

        // Execute the query
        if (!query.exec()) {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la recherche: " + query.lastError().text());
            return;
        }

        // Use the query result for the model
        model->setQuery(query);
    }

    // Set the model to the table view
    ui->tableView->setModel(model);

    // Optional: Customize headers
    model->setHeaderData(0, Qt::Horizontal, "ID Employé");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Date Embauche");
    model->setHeaderData(4, Qt::Horizontal, "Poste");
    model->setHeaderData(5, Qt::Horizontal, "Salaire");

    // Inform the user
    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Aucun résultat", "Aucun employé trouvé avec cette recherche.");
    }
}

///////////////////////////////////////////////////////////
void Dialog::addEvent()
{
    QString eventText = ui->line_event->text();
    QDate selectedDate = ui->calendar->selectedDate();

    if (eventText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un événement.");
        return;
    }

    // Add event to the database
    QSqlQuery query;
    query.prepare("INSERT INTO events (event_date, description) VALUES (:date, :description)");
    query.bindValue(":date", selectedDate);
    query.bindValue(":description", eventText);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'événement: " + query.lastError().text());
        return;
    }

    // Highlight the date on the calendar
    QTextCharFormat format;
    format.setBackground(Qt::yellow);
    ui->calendar->setDateTextFormat(selectedDate, format);

    // Clear the input field
    ui->line_event->clear();

    // Inform the user
    QMessageBox::information(this, "Succès", "Événement ajouté avec succès.");

    // Refresh events display
    showEvents(selectedDate);
}

// Show events for the selected date
void Dialog::showEvents(const QDate &date)
{
    // Clear the event display
    ui->textEdit_events->clear();

    // Query the database for events on the selected date
    QSqlQuery query;
    query.prepare("SELECT description FROM events WHERE event_date = :date");
    query.bindValue(":date", date);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les événements: " + query.lastError().text());
        return;
    }

    // Display the events
    QStringList eventList;
    while (query.next()) {
        eventList.append(query.value(0).toString());
    }

    if (!eventList.isEmpty()) {
        for (const QString &event : eventList) {
            ui->textEdit_events->append(event);
        }
    } else {
        ui->textEdit_events->setText("Aucun événement pour ce jour.");
    }
}


void Dialog::clearEvent()
{
    QDate selectedDate = ui->calendar->selectedDate();

    // Query to delete events for the selected date
    QSqlQuery query;
    query.prepare("DELETE FROM events WHERE event_date = :date");
    query.bindValue(":date", selectedDate);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Événements supprimés pour cette date.");

        // Clear the text edit
        ui->textEdit_events->clear();
        ui->textEdit_events->setText("Aucun événement pour ce jour.");
        // Reset the calendar day format to default
        QTextCharFormat defaultFormat;
        ui->calendar->setDateTextFormat(selectedDate, defaultFormat);
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de supprimer les événements: " + query.lastError().text());
    }
}
void Dialog::highlightEventDates()
{
    QSqlQuery query("SELECT DISTINCT event_date FROM events");

    QTextCharFormat format;
    format.setBackground(Qt::yellow);

    while (query.next()) {
        QDate date = query.value(0).toDate();
        ui->calendar->setDateTextFormat(date, format);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Dialog::showSalaryDistribution()
{
    // Query to count employees in each salary range
    QSqlQuery query;
    query.prepare(
        "SELECT "
        "SUM(CASE WHEN SALAIRE < 500 THEN 1 ELSE 0 END) AS \"<500\", "
        "SUM(CASE WHEN SALAIRE >= 500 AND SALAIRE < 1000 THEN 1 ELSE 0 END) AS \"500-1000\", "
        "SUM(CASE WHEN SALAIRE >= 1000 AND SALAIRE < 1500 THEN 1 ELSE 0 END) AS \"1000-1500\", "
        "SUM(CASE WHEN SALAIRE >= 1500 AND SALAIRE < 2000 THEN 1 ELSE 0 END) AS \"1500-2000\", "
        "SUM(CASE WHEN SALAIRE >= 2000 AND SALAIRE < 2500 THEN 1 ELSE 0 END) AS \"2000-2500\", "
        "SUM(CASE WHEN SALAIRE >= 2500 THEN 1 ELSE 0 END) AS \">=2500\" "
        "FROM EMPLOYES"
        );

    // Execute the query
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des données: " + query.lastError().text());
        return;
    }

    // Ensure data retrieval succeeded
    if (!query.next()) {
        QMessageBox::critical(this, "Erreur", "Aucune donnée trouvée.");
        return;
    }

    // Retrieve the counts
    int under500 = query.value(0).toInt();
    int between500And1000 = query.value(1).toInt();
    int between1000And1500 = query.value(2).toInt();
    int between1500And2000 = query.value(3).toInt();
    int between2000And2500 = query.value(4).toInt();
    int over2500 = query.value(5).toInt();

    // Create the pie chart series
    QPieSeries *series = new QPieSeries();
    series->append("<500", under500);
    series->append("500-1000", between500And1000);
    series->append("1000-1500", between1000And1500);
    series->append("1500-2000", between1500And2000);
    series->append("2000-2500", between2000And2500);
    series->append(">2500", over2500);

    // Customize slices
    for (auto slice : series->slices()) {
        slice->setLabel(QString("%1 (%2%)")
                            .arg(slice->label())
                            .arg(100 * slice->percentage(), 0, 'f', 1));
        slice->setExploded(slice->value() == std::max({under500, between500And1000, between1000And1500, between1500And2000, between2000And2500, over2500}));
    }

    // Create the chart and set the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des salaires des employés");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Customize chart legend
    chart->legend()->setAlignment(Qt::AlignRight);

    // Create the chart view and add it to the layout
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Get the layout and add the chart view
    QVBoxLayout *chartLayout = qobject_cast<QVBoxLayout *>(ui->chartPlaceholder->layout());
    if (chartLayout) {
        // Clear the previous chart if any
        QLayoutItem *item;
        while ((item = chartLayout->takeAt(0)) != nullptr) {
            delete item->widget();  // Delete the widget
            delete item;  // Delete the layout item
        }

        // Add the new chart
        chartLayout->addWidget(chartView);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Dialog::on_seniorityAnalysis_clicked() {
    // SQL query to get employee hire dates
    QSqlQuery query;
    query.prepare("SELECT DATE_EMBAUCHE FROM EMPLOYES");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des dates d'embauche: " + query.lastError().text());
        return;
    }

    // Initialize counters for the seniority categories
    int juniorCount = 0, midLevelCount = 0, seniorCount = 0;

    while (query.next()) {
        QDate hireDate = query.value("DATE_EMBAUCHE").toDate();

        // If hire date is valid and not null
        if (hireDate.isValid()) {
            int yearsOfService = hireDate.daysTo(QDate::currentDate()) / 365;  // Calculate years of service

            // Categorize employees based on their years of service
            if (yearsOfService <= 3) {
                juniorCount++;
            } else if (yearsOfService >= 4 && yearsOfService <= 7) {
                midLevelCount++;
            } else {
                seniorCount++;
            }
        }
    }

    // Create a pie chart series for seniority levels
    QPieSeries *series = new QPieSeries();
    series->append("Junior", juniorCount);
    series->append("Mid-Level", midLevelCount);
    series->append("Senior", seniorCount);

    // Customize slices
    for (auto slice : series->slices()) {
        slice->setLabel(QString("%1 (%2%)")
                            .arg(slice->label())
                            .arg(100 * slice->percentage(), 0, 'f', 1));
        slice->setExploded(slice->value() == std::max({juniorCount, midLevelCount, seniorCount}));
    }

    // Create the chart and set the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des employés par niveau de séniorité");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Customize chart legend
    chart->legend()->setAlignment(Qt::AlignRight);

    // Create the chart view and add it to the layout
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Get the layout and add the chart view
    QVBoxLayout *chartLayout = qobject_cast<QVBoxLayout *>(ui->chartPlaceholder->layout());
    if (chartLayout) {
        qDebug() << "Chart layout: " << chartLayout;
        // Clear previous chart if any
        QLayoutItem *item;
        while ((item = chartLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        // Add the new chart
        chartLayout->addWidget(chartView);
    }
}


