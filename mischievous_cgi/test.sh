#!/bin/bash

make all && ./pseudo_cgi.out $1 | ./filedesc_server.out ;
