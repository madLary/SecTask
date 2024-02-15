#include <QCoreApplication>
#include "main.h"

//void insert_Employee(QSqlDatabase &db);
void RunServer();

class EmployeeDbImpl final : public EmployeeDb::Service
{
public:

  explicit EmployeeDbImpl()
    {

        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("./../../db/Employees.db");
        if (db.open())
        {
            qDebug() << "Db is loaded succufully: " << db.databaseName();
        }
        else
        {
            qDebug() << "Error while loading db: " << db.lastError().databaseText();
        }
    }
    ~EmployeeDbImpl()
    {
        db.close();
    }

    Status InsertEmployee (ServerContext* context, const Employee* employee, StatusReply* status_reply) override
    {
        mu_.lock();

        QString name, post;
        int lead;

        std::ostringstream buf;
        name = QString::fromStdString(employee->name());
        post = QString::fromStdString(employee->post());
        lead = employee->lead();
        if(!id_chek(lead))
        {
            buf << "Lead ID is not correct, it was set to 0 by default." << std::endl;
            lead = 0;
        }

        QSqlQuery query(db);
        query.prepare("INSERT INTO Employees(Name,Post,Director) VALUES(:Name,:Post,:Director)");
        //query.bindValue(":ID", 0);
        query.bindValue(":Name", name);
        query.bindValue(":Post", post);
        query.bindValue(":Director", lead);
        //query.exec();
        if(!query.exec())
        {
            buf << "Error: " << (query.lastError().databaseText()).toStdString();
            status_reply->set_reply(buf.str());
        }
        else
        {
            buf << "Employee was successfully added with ID: " << (query.lastInsertId()).toString().toStdString();
            status_reply->set_reply(buf.str());
        }

        mu_.unlock();

        return Status::OK;
    }

    Status GetList (ServerContext* context, const employee::GetListRequest* getListRequest, ServerWriter<EmployeeReply>* writer) override
    {
        mu_.lock();

        QSqlQuery get_list_query(db);
        QSqlQuery get_list_search_query(db);
        int index;
        get_list_query.exec("SELECT * FROM Employees");
        EmployeeReply em;

//        if (!get_list_query.isSelect())
//        {
//            mu_.unlock();
//            return Status::CANCELLED;
//        }


        while (get_list_query.next())
        {
            em.set_id(get_list_query.value(0).toInt());
            em.set_name((get_list_query.value(1).toString()).toStdString());
            em.set_post((get_list_query.value(2).toString()).toStdString());
            index = get_list_query.value(3).toInt();
            if(index != 0)
            {
                if(!id_chek(index))
                {
                    em.set_lead("Incorrect lead ID in database");
                }
                else
                {
                    get_list_search_query.prepare("SELECT Name FROM Employees WHERE ID = :ID");
                    get_list_search_query.bindValue(":ID", index);
                    get_list_search_query.exec();
                    get_list_search_query.first();
                    em.set_lead(get_list_search_query.value(0).toString().toStdString());
                }

            }
            else
            {
                em.set_lead("none");
            }

            writer->Write(em);
        }

        mu_.unlock();
        return Status::OK;
    }

    Status GetPostAndLead (ServerContext* context, const Id* id, PostAndLead* post_and_lead) override
    {
        mu_.lock();

        QSqlQuery query(db);
        int index = id->id();
        if(!id_chek(index))
        {
            mu_.unlock();
            post_and_lead->set_post("Incorrect input ID");
            post_and_lead->set_lead("Incorrect input ID");
            return Status::OK;
        }

        //query.exec("SELECT Post FROM Employees");
        query.prepare("SELECT Post, Director FROM Employees WHERE ID = :ID");
        query.bindValue(":ID", index);
        query.exec();
        query.first();
        post_and_lead->set_post((query.value(0).toString()).toStdString());
        index = query.value(1).toInt();
        if(index == 0)
        {
            post_and_lead->set_lead("none");
        }
        else
        {
            query.prepare("SELECT Name FROM Employees WHERE ID = :ID");
            query.bindValue(":ID", index);
            query.exec();
            query.first();
            post_and_lead->set_lead((query.value(0).toString()).toStdString());
        }

        mu_.unlock();
        return Status::OK;
    }

    Status SetEmployeePost (ServerContext* context, const Post_Setter* post_setter, StatusReply* status_reply) override
    {
        mu_.lock();

        QSqlQuery query(db);
        std::ostringstream buf;
        int id = post_setter->id();
        QString post = QString::fromStdString(post_setter->data());
        if(!id_chek(id))
        {
            mu_.unlock();
            buf << "Incorrect ID";
            status_reply->set_reply(buf.str());
            return Status::OK;
        }

        query.prepare("UPDATE Employees SET Post = :POST WHERE ID = :ID");
        query.bindValue(":POST", post);
        query.bindValue(":ID", id);
        query.exec();
        buf << "SetEmployeePost rpc succeeded.";
        status_reply->set_reply(buf.str());

        mu_.unlock();
        return Status::OK;
    }

    Status SetEmployeeLead (ServerContext* context, const Lead_Setter* lead_setter, StatusReply* status_reply) override
    {
        mu_.lock();

        QSqlQuery query(db);
        std::ostringstream buf;
        int id = lead_setter->id();
        int lead_id = lead_setter->data();
        int id_verify = lead_id;
        if(!id_chek(id))
        {
            buf << "Incorrect ID";
            status_reply->set_reply(buf.str());

            mu_.unlock();
            return Status::OK;
        }

        if(lead_id == 0)
        {
            query.prepare("UPDATE Employees SET Director = :LEAD WHERE ID = :ID");
            query.bindValue(":LEAD", lead_id);
            query.bindValue(":ID", id);
            query.exec();
            buf << "SetEmployeeLead rpc succeeded.";
            status_reply->set_reply(buf.str());

            mu_.unlock();

            return Status::OK;
        }

        if(!id_chek(lead_id))
        {
            buf << "Incorrect lead ID";
            status_reply->set_reply(buf.str());

            mu_.unlock();
            return Status::OK;
        }


//        query.prepare("SELECT Director FROM Employees WHERE ID = :ID");
//        query.bindValue(":ID", lead_id);
//        query.exec();
//        query.first();
//        if(id == query.value(0).toInt())
//        {
//            buf << "Employees cannot be each other`s directors";
//            status_reply->set_reply(buf.str());
//            return Status::OK;
//        }

        while(id_verify != 0)
        {
            query.prepare("SELECT Director FROM Employees WHERE ID = :ID");
            query.bindValue(":ID", id_verify);
            query.exec();
            query.first();
            id_verify = query.value(0).toInt();
            if(id == id_verify)
            {
                buf << "Employees cannot be each other`s directors";
                status_reply->set_reply(buf.str());

                mu_.unlock();
                return Status::OK;
            }
        }

        query.prepare("UPDATE Employees SET Director = :LEAD WHERE ID = :ID");
        query.bindValue(":LEAD", lead_id);
        query.bindValue(":ID", id);
        query.exec();
        buf << "SetEmployeeLead rpc succeeded.";
        status_reply->set_reply(buf.str());

        mu_.unlock();

        return Status::OK;
    }

    Status DeleteEmployee (ServerContext* context, const Id* id, StatusReply* status_reply) override
    {
        mu_.lock();

        int lead;
        QSqlQuery query(db);
        QSqlQuery sec_query(db);
        std::ostringstream buf;
        QString name = 0;
        if(!id_chek(id->id()))
        {
            buf << "Incorrect ID";
            status_reply->set_reply(buf.str());
            mu_.unlock();
            return Status::OK;
        }

        query.prepare("SELECT Director FROM Employees WHERE ID = :ID");
        query.bindValue(":ID", id->id());
        query.exec();
        query.first();
        lead = query.value(0).toInt();

        query.prepare("SELECT Name FROM Employees WHERE ID = :ID");
        query.bindValue(":ID", id->id());
        query.exec();
        query.first();
        name = query.value(0).toString();

        query.prepare("SELECT ID, Name FROM Employees WHERE Director = :LEAD");
        query.bindValue(":LEAD", id->id());
        query.exec();
        while (query.next())
        {
            buf << "Lead ID of employee with ID: "<< query.value(0).toString().toStdString()
                << " and name: " << query.value(1).toString().toStdString()
                << " was set to " << lead << ", due lead deletion" << std::endl;
        }

        sec_query.prepare("UPDATE Employees SET Director = :DIR WHERE Director = :LEAD");
        sec_query.bindValue(":DIR", lead);
        sec_query.bindValue(":LEAD", id->id());
        sec_query.exec();

        query.prepare("DELETE FROM Employees WHERE ID = :ID");
        query.bindValue(":ID", id->id());
        query.exec();
        buf << "Record with ID: " << id->id() << " and name: " << name.toStdString() <<" was successfully deleted. ";
        status_reply->set_reply(buf.str());

        mu_.unlock();

        return Status::OK;
    }




private:
    QSqlDatabase db;
    std::mutex mu_;

    bool id_chek(int id)
    {
        QSqlQuery local_query(db);
        local_query.prepare("SELECT Post, Director FROM Employees WHERE ID = :ID");
        local_query.bindValue(":ID", id);
        local_query.exec();
        if(local_query.first())
        {
            return true;
        }
        return false;
    }

};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    RunServer();

    return a.exec();
}

void RunServer()
{
  std::string server_address("0.0.0.0:50051");
  EmployeeDbImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

//void insert_Employee(QSqlDatabase &db)
//{
//    db.open();
//    QSqlDatabase::database().transaction();
//    QSqlQuery query(db);
//    query.prepare("INSERT INTO Employees(Name,Post,Director) VALUES(:Name,:Post,:Director)");
//    //query.bindValue(":ID", 0);
//    query.bindValue(":Name", "Ivan");
//    query.bindValue(":Post", "Worker");
//    query.bindValue(":Director", 0);
//    query.exec();
//    QSqlDatabase::database().commit();
//    db.close();
//}

