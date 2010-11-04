#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include "models/profile.h"

#include <QSqlQuery>


/**
 * Classe permettant la gestion des profiles d'utilisateurs
 */
class ProfileManager
{
public:
    /**
     * Récupère le profile d'un utilisateur
     *
     * @param id L'identifiant du profile à utiliser
     *
     * @return Profile Le profile chargé.
     */
    Profile get(int id);

    /**
     * Enregistre ou met à jour le profile d'un utilisateur.
     *
     * \note En cas d'un nouveau profile, l'id est renseigné lors de l'enregistrement
     *
     * @param p Le profile à enregistrer
     */
    void save(Profile &profile);

private:
    /**
     * Crée un profile à partir des résultats d'une requête
     *
     * \note On suppose que tous les champs d'un profile sont
     *       retournés par la requête
     *
     * @param query La requête qui vient d'être exécutée
     *
     * @return Profile Le profile créé
     */
    Profile makeProfile(QSqlQuery &query);

    /**
     * Réalise l'enregistrement d'un nouveau profile
     *
     * \note l'id est renseigné ici.
     *
     * @param profile Le profile à enregistrer
     */
    void insert(Profile &profile);

    /**
     * Réalise la mise à jour d'un profile
     *
     * @param profile Le profile à mettre à jour
     */
    void update(const Profile &profile);

    /**
     * Effectue un bindValue() sur la query avec les champs du
     * profile.
     *
     * @param profile Profile à binder
     * @param query Query à laquelle lier le profile
     */
    void bindProfile(const Profile &profile, QSqlQuery &query);
};

#endif // PROFILEMANAGER_H
