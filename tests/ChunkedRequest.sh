#!/bin/bash

curl -v -X POST localhost:8080/php_tests/post.php \
	-T SamplePost \
	-H "Transfer-Encoding: chunked" \
	-H "Content-Type: multipart/form-data; boundary=---------------------------4046729733594385941515630244" \
	;
