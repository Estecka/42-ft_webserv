#!/bin/bash

make all && ./pseudo_cgi.out $1 | ./pseudo_server.out ;
