#ifndef MAIN_H
#define MAIN_H

#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <iostream>


//#include "employee.h"
//#include "server.h"

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "employee.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
//using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using employee::EmployeeDb;
using employee::Employee;
using employee::EmployeeReply;
using employee::StatusReply;
using employee::GetListRequest;
using employee::Id;
using employee::PostAndLead;
using employee::Post_Setter;
using employee::Lead_Setter;





#endif // MAIN_H
