#!/bin/bash
cd /dunedaq/run/
source dbt-env.sh
#dbt-workarea-env
dbt-setup-release dunedaq-v2.11.1-cs8
source dbt-pyvenv/bin/activate
PYLIB=$(ls -l dbt-pyvenv/bin/python|sed -e "s=.* \(.*\)/bin/python=\1/lib=")
export LD_LIBRARY_PATH=${PYLIB}:${LD_LIBRARY_PATH}
export FLASK_APP=install/configclient/bin/configDict
python -m flask run --host=0.0.0.0
