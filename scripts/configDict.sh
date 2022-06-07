#!/bin/bash
cd /dunedaq/run/
source dbt-env.sh
dbt-workarea-env
source dbt-pyvenv/bin/activate
cd ../devel
export FLASK_APP=configDict
python -m flask run --host=0.0.0.0
