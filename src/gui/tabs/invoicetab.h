#ifndef INVOICETAB_H
#define INVOICETAB_H

#include "qfacturecore.h"
#include "gui/widgets/sqltable.h"

#include <QListWidget>
#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QCompleter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QTableWidget>

#include "models/invoice.h"


class InvoiceTab : public QWidget
{
    Q_OBJECT
public:
    explicit InvoiceTab(Invoice invoice, QfactureCore *core, QWidget *parent = 0);
    ~InvoiceTab();

public slots:
    /**
     * (Dés)active l'onglet
     *
     * @param state État d'activation de l'onglet
     */
    void setEnabled(bool state=true);

private:
    /**
     * Construit la GUI de l'onglet
     */
    void buildLayout();

    /**
     * Construit la partie haute de la GUI de l'onglet
     */
    void buildHeader();

    /**
     * Construit le corps de la GUI de l'onglet
     */
    void buildBody();

    void buildClientBox();
    void buildCommentBox();
    void buildDetailsBox();


    QVBoxLayout *l_layout;
    QHBoxLayout *l_header;
    QVBoxLayout *l_header_left;
    QVBoxLayout *l_header_right;
    QHBoxLayout *l_body;

    QVBoxLayout *l_client;
    QLineEdit *le_client;
    QLineEdit *le_selected_client;
    QListWidget *li_clients;
    QGroupBox *gbox_client;

    QHBoxLayout *l_comment;
    QLineEdit *le_comment;
    QGroupBox *gbox_comment;

    QFormLayout *form_details;
    QHBoxLayout *l_details_button;
    QGroupBox *gbox_details;
    QLineEdit *le_facture_no;
    QDateEdit *le_facture_date;
    QComboBox *le_facture_type;
    QLineEdit *le_facture_montant;
    QComboBox *le_facture_reglement;
    QPushButton *btn_creer;
    QPushButton *btn_sauver;
    QPushButton *btn_print;
    QPushButton *btn_del;

    /**
     * Pointeur vers le contrôleur principal
     */
    QfactureCore *core;

    /**
     * Facture représentée dans l'onglet
     */
    Invoice invoice;
};

#endif // INVOICETAB_H
