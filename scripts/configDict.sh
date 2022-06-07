#!/bin/bash
cd /dunedaq/run/
source dbt-env.sh
dbt-workarea-env
#source dbt-pyvenv/bin/activate

export FLASK_APP=install/configclient/bin/configDict
python -m flask run --host=0.0.0.0
