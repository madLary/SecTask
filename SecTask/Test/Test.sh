#!/bin/bash
locust -f Test.py --host=http://localhost:50051
# locust -f SecondTest.py --host=http://localhost:50051

#k6 run Test.js
