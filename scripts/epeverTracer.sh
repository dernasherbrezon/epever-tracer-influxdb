#!/bin/bash

MAILTO=""

set -e

source /etc/epeverTracer.conf

if [ -z "$device" ]
then
	  # device can be empty if app is not configured
	  # return 0 code (success) and no output to logs
      exit 0
fi

if [ -z "$influxdbUrl" ]
then
	  >&2 echo "influxdbUrl is not configured at /etc/epeverTracer.conf"
      exit 1
fi

if [ -z "$influxdbToken" ]
then
	  >&2 echo "influxdbToken is not configured at /etc/epeverTracer.conf"
      exit 1
fi

epeverTracer -d ${device} -t ${modbusTimeout} | curl -XPOST "${influxdbUrl}" --header "Authorization: Token ${influxdbToken}" --data-binary @-