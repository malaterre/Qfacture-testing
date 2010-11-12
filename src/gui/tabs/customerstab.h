#ifndef CUSTOMERSTAB_H
#define CUSTOMERSTAB_H

#include <qfacturecore.h>
#include "gui/widgets/sqltable.h"

#include <QWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>


class CustomersTab : public QWidget
{
    Q_OBJECT
public:
    explicit CustomersTab(QfactureCore *core, QWidget *parent = 0);
    ~CustomersTab();

public slots:
    /**
     * Appelé lorsque l'état de connexion à la DB change. Elle
     * s'occupe de (dés)activer certains widgets
     */
    void onDBConnectionStateChanged();

    /**
     * Recharge la liste des clients dans le tableau à partir de la
     * DB
     */
    void loadCustomers();

private:
    /**
     * Construit la GUI de l'onglet
     */
    void buildLayout();

    /**
     * Cette méthode, appelée par le constructeur, s'occupe de connecter les
     * principaux évènements.
     */
    void createActions();


    /**
     * Layout principal du widget
     */
    QVBoxLayout *layout;

    /**
     * Layout contenant le form d'édition d'un client + les boutons
     * actions
     */
    QHBoxLayout *edit_customer_layout;

    /**
     * Layout contenant le tableau des clients
     */
    QHBoxLayout *customers_layout;

    /**
     * Groupbox servant de conteneur pour la liste des clients
     */
    QGroupBox *gbox_customers;

    /**
     * Groupbox servant de conteneur pour le formulaire de création
     * d'un client
     */
    QGroupBox *gbox_customers_form;

    /**
     * Groupbox servant de conteneur pour la liste des actions
     */
    QGroupBox *gbox_actions;

    /**
     * Tableau des clients
     */
    SQLTable *customers_table;

    /**
     * Pointeur vers le contrôleur principal
     */
    QfactureCore *core;
};

#endif // CUSTOMERSTAB_H
