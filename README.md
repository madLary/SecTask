# SecTask

————————————————————————————————————
# Test.py
Test for unary RPCs:
1. InsertEmployee
2. GetPostAndLead
3. SetPost
4. SetLead
5. DeleteEmployee

Requirements for Test.py
Locust must be installed via 
    $ pip3 install locust

Official documentation: https://docs.locust.io/en/2.0.0/extending-locust.html
Launch from ./Test directory via
    $ locust -f Test.py --host=http://localhost:50051

Follow the http://localhost:8089/ in your browser
Start the test 
————————————————————————————————————
#Test.js
Test for Server streaming RPC:
1. Getlist

Requirements for Test.js
k6 must be installed via
    $ sudo gpg -k
    $ sudo gpg --no-default-keyring --keyring /usr/share/keyrings/k6-archive-keyring.gpg --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys C5AD17C747E3415A3642D57D77C6C491D6AC1D69
    $ echo "deb [signed-by=/usr/share/keyrings/k6-archive-keyring.gpg] https://dl.k6.io/deb stable main" | sudo tee /etc/apt/sources.list.d/k6.list
    $ sudo apt-get update
    $ sudo apt-get install k6

Official documentation: https://grafana.com/docs/k6/latest/get-started/installation/

Launch from ./Test directory via
    $ k6 run Test.js 
for 1 iteration
also try 
    $ k6 run --duration 30s Test.js