#include "customermanager.h"
#include "controllers/dbcontroller.h"

#include "QVariant"


Customer CustomerManager::get(int id, int uid)
{
    QSqlQuery query;
    QString sql;
    Customer c;

    sql = "SELECT cID, name, address, complement, zip, city, phone, mail "
          "FROM client WHERE cID = :customer_id";

    if(uid != -1)
        sql += " AND u_ID = :uid";

    query.prepare(sql);

    query.bindValue(":customer_id", QVariant(id));
    if(uid != -1)
        query.bindValue(":uid", QVariant(uid));

    if(!DBController::getInstance()->exec(query))
        return c;

    // pas de client avec l'ID demandé, on ne remonte pas d'erreur
    // mais juste un client vide
    if(query.next())
        c = makeCustomer(query);

    query.finish();

    return c;
}

bool CustomerManager::save(Customer &customer, int uid)
{
    return (customer.getId() == 0) ? insert(customer, uid) : update(customer, uid);
}

bool CustomerManager::erase(int id, int uid)
{
    QSqlQuery query;
    QString sql;

    if(id <= 0)
        return false;

    sql = "DELETE FROM client WHERE cID = :c_id AND u_ID = :uid";

    query.prepare(sql);

    query.bindValue(":c_id", QVariant(id));
    query.bindValue(":uid", QVariant(uid));

    if(DBController::getInstance()->exec(query))
    {
        query.finish();

        return true;
    }

    return false;
}

bool CustomerManager::insert(Customer &customer, int uid)
{
    QSqlQuery query;

    query.prepare(
            "INSERT INTO client "
                "(u_ID, name, address, complement, zip, city, phone, Mail) "
            "VALUES "
                "(:uid, :name, :address, :address2, :zip, :city, :phone, :mail)"
    );

    bindCustomer(customer, query, uid);

    if(DBController::getInstance()->exec(query))
    {
        customer.setId(query.lastInsertId().toInt());

        query.finish();

        return true;
    }

    return false;
}

bool CustomerManager::update(const Customer &customer, int uid)
{
    QSqlQuery query;
    QString sql;

    sql = "UPDATE client "
          "SET name = :name, address = :address, complement = :complement, "
              "zip = :zip, city = :city, phone = :phone, "
              "mail = :mail "
          "WHERE cID = :c_id";

    if(uid != -1)
        sql += " AND u_ID = :uid";

    query.prepare(sql);

    bindCustomer(customer, query, uid);

    if(DBController::getInstance()->exec(query))
    {
        query.finish();

        return true;
    }

    return false;
}

void CustomerManager::bindCustomer(const Customer &customer, QSqlQuery &query, int uid)
{
    query.bindValue(":c_id", customer.getId());
    query.bindValue(":name", customer.getName());
    query.bindValue(":address", customer.getAddress());
    query.bindValue(":complement", customer.getAddressComplement());
    query.bindValue(":zip", customer.getZipCode());
    query.bindValue(":city", customer.getCity());
    query.bindValue(":phone", customer.getPhone());
    query.bindValue(":mail", customer.getMail());

    if(uid != -1)
        query.bindValue(":uid", QVariant(uid));
}

Customer CustomerManager::makeCustomer(QSqlQuery &query)
{
    Customer customer;

    customer.setId(query.value(0).toInt());
    customer.setName(query.value(1).toString());
    customer.setAddress(query.value(2).toString());
    customer.setAddressComplement(query.value(3).toString());
    customer.setZipCode(query.value(4).toString());
    customer.setCity(query.value(5).toString());
    customer.setPhone(query.value(6).toString());
    customer.setMail(query.value(7).toString());

    return customer;
}
