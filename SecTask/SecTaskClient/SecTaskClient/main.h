#ifndef MAIN_H
#define MAIN_H

#include <iostream>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "employee.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
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
