#include <QCoreApplication>
#include "main.h"

void InputFilter(int &data);
void InputFilter(bool &data);

Employee MakeEmployee(std::string name, std::string post, int lead)
{
    Employee em;
    em.set_name(name);
    em.set_post(post);
    em.set_lead(lead);
    return em;
}

class EmployeeDbClient
{
public:
    EmployeeDbClient(std::shared_ptr<Channel> channel): stub_(EmployeeDb::NewStub(channel)){}

    void InsertEmployee()
    {
        ClientContext context;
        Employee employee;
        StatusReply statusReply;
        std::string name, post;
        int lead=0;
        std::cout << "Enter name : ";
        std::cin >> name;
        std::cout << "Enter post : ";
        std::cin >> post;
        std::cout << "Enter leadID : ";
        //std::cin >> lead;
        InputFilter(lead);
        std::cout<< "Entered data is : name = " << name <<", post = " << post << ", lead ID = " << lead << std::endl;
        employee = MakeEmployee(name, post, lead);
        Status status = stub_->InsertEmployee(&context, employee, &statusReply);
        if (!status.ok())
        {
            std::cout << "InsertEmployee rpc failed." << std::endl;
        }
        else
        {
            std::cout << statusReply.reply() << std::endl;
        }
    }

    void Getlist()
    {
        ClientContext context;
        EmployeeReply employee;
        GetListRequest getListReq;
        std::unique_ptr<ClientReader<EmployeeReply> > reader(stub_->GetList(&context, getListReq));
        while (reader -> Read(&employee))
        {
            std::cout << employee.id()
                      << std::setw (0) <<" " << employee.name()
                      << std::setw (0) <<" " << employee.post()
                      << std::setw (0) <<" " << employee.lead() << std::endl;
        }
        Status status = reader -> Finish();
        if (status.ok())
        {
            std::cout << "GetList rpc succeeded." << std::endl;
        }
        else
        {
            std::cout << "GetList rpc failed." << std::endl;
        }
    }

    void GetPostAndLead()
    {
        ClientContext context;
        Id id;
        PostAndLead post_and_lead;

        int input;
        std::cout << "Enter ID: ";
        //std::cin >> input;
        InputFilter(input);
        id.set_id(input);
        Status status = stub_->GetPostAndLead(&context, id, &post_and_lead);
        if (!status.ok())
        {
            std::cout << "GetPostAndLead rpc failed." << std::endl;
        }
        else
        {
            std::cout << "Post: " << post_and_lead.post() << std::endl
                      << "Lead: " << post_and_lead.lead() << std::endl;
        }
    }

    void SetEmployeePost()
    {
        ClientContext context;
        Post_Setter post_setter;
        StatusReply statusReply;
        int id;
        std::string post;
        std::cout << "Enter ID: ";
        //std::cin >> id;
        InputFilter(id);
        post_setter.set_id(id);
        std::cout << "Enter post: ";
        std::cin >> post;
        post_setter.set_data(post);
        Status status = stub_->SetEmployeePost(&context, post_setter, &statusReply);
        if (!status.ok())
        {
            std::cout << "SetEmployeePost rpc failed." << std::endl;
        }
        else
        {
            std::cout << statusReply.reply() << std::endl;
        }
    }

    void SetEmployeeLead()
    {
        ClientContext context;
        Lead_Setter lead_setter;
        StatusReply statusReply;
        int id;
        int lead_id;
        std::cout << "Enter ID: ";
        //std::cin >> id;
        InputFilter(id);
        lead_setter.set_id(id);
        std::cout << "Enter lead id: ";
        //std::cin >> lead_id;
        InputFilter(lead_id);
        if(id == lead_id)
        {
            std::cout << "An employee can` t be his own lead" << std::endl;
            return;
        }
        lead_setter.set_data(lead_id);
        Status status = stub_->SetEmployeeLead(&context, lead_setter, &statusReply);
        if (!status.ok())
        {
            std::cout << "SetEmployeeLead rpc failed." << std::endl;
        }
        else
        {
            std::cout << statusReply.reply() << std::endl;
        }
    }

    void DeleteEmployee()
    {
        ClientContext context;
        Id id;
        StatusReply statusReply;
        int input_id;
        std::cout << "Enter ID: ";
        //std::cin >> input_id;
        InputFilter(input_id);
        id.set_id(input_id);
        Status status = stub_->DeleteEmployee(&context, id, &statusReply);
        if (!status.ok())
        {
            std::cout << "DeleteEmployee rpc failed." << std::endl;
        }
        else
        {
            std::cout << statusReply.reply() << std::endl;
        }
    }


private:
    std::unique_ptr<EmployeeDb::Stub> stub_;



};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    EmployeeDbClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    bool flag = 0;
    int option = 0;
    while (flag == 0)
    {
        std::cout << "Options: " << std::endl
                  << "0 - InsertEmployee" << std::endl
                  << "1 - GetList" << std::endl
                  << "2 - GetPostAndLead"<< std::endl
                  << "3 - SetEmployeePost" << std::endl
                  << "4 - SetEmployeeLead" << std::endl
                  << "5 - DeleteEmployee" << std::endl
                  << "6 - Exit" << std::endl;
        std::cout << "Enter option: ";
        //std::cin >> option;
        InputFilter(option);
        switch (option)
        {
        case 0:
            client.InsertEmployee();
            break;
        case 1:
            client.Getlist();
            break;
        case 2:
            client.GetPostAndLead();
            break;
        case 3:
            client.SetEmployeePost();
            break;
        case 4:
            client.SetEmployeeLead();
            break;
        case 5:
            client.DeleteEmployee();
            break;
        case 6:
            break;

        }

        std::cout << "Do you want to exit? 0 - no, 1 - yes" << std::endl;
        //std::cin >> flag;
        InputFilter(flag);
    }


    return 0;
    //return a.exec();
}

void InputFilter(int &data)
{
    std::string buf;
    bool flag=true;
    std::cin >> buf;
    for(int i = 0; i <buf.length(); i++)
    {
        if(!isdigit(buf[i]))
        {
            flag=false;
            break;
        }
    }
    if(flag)
        data = stoi(buf);
    else
    {
        std::cout << "Incorrect input data, try again" << std::endl;
        InputFilter(data);
    }
}

void InputFilter(bool &data)
{
    std::string buf;
    bool flag=true;
    std::cin >> buf;
    for(int i = 0; i <buf.length(); i++)
    {
        if(!isdigit(buf[i]))
        {
            flag=false;
            break;
        }
    }
    if(flag)
        data = stoi(buf);
    else
    {
        std::cout << "Incorrect input data, try again" << std::endl;
        InputFilter(data);
    }
}
