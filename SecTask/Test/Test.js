import { Client, Stream } from 'k6/net/grpc';
import { sleep } from 'k6';

const COORD_FACTOR = 1e7;

const client = new Client();
client.load([], 'employee.proto');

export default () => {
  if (__ITER == 0) {
    client.connect('localhost:50051', { plaintext: true});
  }

  const stream = new Stream(client, 'employee.EmployeeDb/GetList', null);

  stream.on('data', function (EmployeeReply) {
    console.log(
      EmployeeReply.ID +
      EmployeeReply.name +
      EmployeeReply.post +
      EmployeeReply.lead
    );
  });

  stream.on('end', function () {
    // The server has finished sending
    client.close();
    console.log('All done');
  });

  // send a message to the server
  stream.write({
  });

  sleep(0.5);
};
