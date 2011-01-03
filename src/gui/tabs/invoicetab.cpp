#include "invoicetab.h"
#include "controllers/dbcontroller.h"

#include <QMessageBox>


InvoiceTab::InvoiceTab(Invoice invoice, QfactureCore *core, QWidget *parent) :
        QWidget(parent)
{
    this->core = core;
    this->invoice = invoice;

    buildLayout();
    createActions();

    onInvoiceStateChanged();
}

InvoiceTab::~InvoiceTab()
{
    delete li_clients;
    delete le_client;
    delete le_selected_client;
    delete le_comment;
    delete le_facture_no;
    delete le_facture_date;
    delete le_facture_type;
    delete le_facture_montant;
    delete le_facture_reglement;
    delete btn_sauver;
    delete btn_print;
    delete btn_del;
    delete t_available_products;
    delete t_selected_products;

    delete l_client;
    delete l_comment;
    delete form_details;
    delete l_details_button;
    delete l_products;

    delete l_header_left;
    delete l_header_right;
    delete l_header;
    delete l_body;
    delete l_layout;

    delete gbox_products;
    delete gbox_client;
    delete gbox_comment;
    delete gbox_details;
}

void InvoiceTab::createActions()
{
    // activation de l'onglet dès que la connexion à la DB est établie
    connect(core, SIGNAL(DBConnected()), this, SLOT(onDBStateChanged()));

    // activation de l'onglet dès que la connexion à la DB est établie
    connect(core, SIGNAL(DBDisconnected()), this, SLOT(onDBStateChanged()));

    connect(t_available_products, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this, SLOT(onAvailableProductDoubleClicked(QTableWidgetItem*)));
}

void InvoiceTab::onDBStateChanged()
{
    bool connected = core->isDBConnected();

    setEnabled(connected);
}

void InvoiceTab::onInvoiceStateChanged()
{
    bool saved = invoice.getId() != 0;

    btn_print->setEnabled(saved);
    btn_del->setEnabled(saved);

    if(saved)
        displayInvoiceData();
}

void InvoiceTab::onAvailableProductDoubleClicked(QTableWidgetItem *item)
{
    QMessageBox::critical(this, trUtf8("Erreur !"), item->data(Qt::UserRole).toString());
}

void InvoiceTab::displayInvoiceData()
{
    le_facture_no->setText(QVariant(invoice.getId()).toString());
    le_facture_montant->setText(QVariant(invoice.getAmount()).toString()+trUtf8(" €"));
    le_facture_date->setDate(invoice.getDate());
    le_comment->setText(invoice.getDescription());
    le_selected_client->setText(invoice.getCustomer().getName());
}

void InvoiceTab::buildLayout()
{
    l_layout = new QVBoxLayout();

    buildHeader();
    buildBody();

    setLayout(l_layout);
}

void InvoiceTab::buildHeader()
{
    l_header = new QHBoxLayout();
    l_header_left = new QVBoxLayout();
    l_header_right = new QVBoxLayout();

    buildClientBox();
    buildCommentBox();
    buildDetailsBox();

    l_header->addLayout(l_header_left);
    l_header->addLayout(l_header_right);

    l_layout->addLayout(l_header);
}

void InvoiceTab::buildBody()
{
    l_body = new QVBoxLayout();

    buildProductsBox();

    l_layout->addLayout(l_body);
}

void InvoiceTab::buildProductsBox()
{
    QStringList columns_labels;
    columns_labels << trUtf8("Désignation") << trUtf8("Prix unitaire")
                   << trUtf8("Quantité") << trUtf8("Remise") << trUtf8("Total");

    createAvailableProductsList();

    l_products = new QVBoxLayout();
    gbox_products = new QGroupBox(trUtf8("Liste des prestations"));
    t_selected_products = new QTableWidget(2, 5);

    t_selected_products->setHorizontalHeaderLabels(columns_labels);

    l_products->addWidget(t_available_products->getWidget());
    l_products->addWidget(t_selected_products);

    gbox_products->setLayout(l_products);

    l_body->addWidget(gbox_products);
}

void InvoiceTab::createAvailableProductsList()
{
    // définition des colonnes du tableau
    QStringList not_wanted = QStringList() << "aID";
    QStringList columns = core->getDBColumns("article", not_wanted);
    QStringList labels = core->getDBLabels("article", not_wanted);

    // création du tableau de produits
    t_available_products = new SQLTable("article", core->getCurrentProfile().getId());
    t_available_products->setColumns(columns, labels);
}

void InvoiceTab::buildClientBox()
{
    l_client = new QVBoxLayout();

    gbox_client = new QGroupBox(trUtf8("Client"));
    le_client = new QLineEdit();
    le_selected_client = new QLineEdit();
    li_clients = new QListWidget();

    le_client->setPlaceholderText(trUtf8("Rechercher un client ..."));
    le_selected_client->setEnabled(false);

    l_client->addWidget(le_client);
    l_client->addWidget(li_clients);
    l_client->addWidget(le_selected_client);

    gbox_client->setLayout(l_client);

    l_header_left->addWidget(gbox_client);
}

void InvoiceTab::buildCommentBox()
{
    l_comment = new QHBoxLayout();

    gbox_comment = new QGroupBox(trUtf8("Commentaire"));
    le_comment = new QLineEdit();

    le_comment->setPlaceholderText(trUtf8("Description de la facture ..."));

    l_comment->addWidget(le_comment);

    gbox_comment->setLayout(l_comment);

    l_header_left->addWidget(gbox_comment);
}

void InvoiceTab::buildDetailsBox()
{
    // formulaire
    form_details = new QFormLayout();

    gbox_details = new QGroupBox(trUtf8("Détails de la facture"));
    le_facture_no = new QLineEdit();
    le_facture_date = new QDateEdit();
    le_facture_type = new QComboBox();
    le_facture_montant = new QLineEdit();
    le_facture_reglement = new QComboBox();

    le_facture_no->setEnabled(false);
    le_facture_montant->setEnabled(false);

    form_details->addRow(trUtf8("Numéro de la facture"), le_facture_no);
    form_details->addRow(trUtf8("Date de facturation"), le_facture_date);
    form_details->addRow(trUtf8("Type de document"), le_facture_type);
    form_details->addRow(trUtf8("Montant total"), le_facture_montant);
    form_details->addRow(trUtf8("Règlement"), le_facture_reglement);

    gbox_details->setLayout(form_details);

    // boutons du form
    l_details_button = new QHBoxLayout();
    btn_sauver = new QPushButton(trUtf8("Sauver"));
    btn_print = new QPushButton(trUtf8("Imprimer"));
    btn_del = new QPushButton(trUtf8("Supprimer"));

    l_details_button->addWidget(btn_sauver);
    l_details_button->addWidget(btn_print);
    l_details_button->addWidget(btn_del);

    l_header_right->addWidget(gbox_details);
    l_header_right->addLayout(l_details_button);
    l_header_right->addStretch(); // pour que les boutons soient collés au form
}

void InvoiceTab::setEnabled(bool state)
{
    gbox_client->setEnabled(state);
    gbox_comment->setEnabled(state);
    gbox_details->setEnabled(state);
    gbox_products->setEnabled(state);

    btn_sauver->setEnabled(state);
    btn_print->setEnabled(state);
    btn_del->setEnabled(state);
}
