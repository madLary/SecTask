from locust import events, task, between

import gevent
import grpc_user
import employee_pb2
import employee_pb2_grpc
  



class EmployeeGrpcUser(grpc_user.GrpcUser):
    host = "localhost:50051"
    stub_class = employee_pb2_grpc.EmployeeDbStub
    wait_time = between(1, 5)
    
    @task(1)
    def InsertEmployee(self):
    	self.stub.InsertEmployee(employee_pb2.Employee(name="TestName"))
    	
#    @task(2)
#    def GetList(self):
#    	self.stub.GetList(employee_pb2.GetListRequest(name="empty"))
    	
    @task(3)
    def GetPostAndLead(self):
    	self.stub.GetPostAndLead(employee_pb2.Id(ID=10))
    
    @task(4)
    def SetEmployeePost(self):
    	self.stub.SetEmployeePost(employee_pb2.Post_Setter(ID=30, data="TestPost"))
    	
    @task(5)
    def SetEmployeeLead(self):
    	self.stub.SetEmployeeLead(employee_pb2.Lead_Setter(ID=65, data=30))
    	
    @task(6)
    def DeleteEmployee(self):
    	self.stub.DeleteEmployee(employee_pb2.Id(ID=10))
