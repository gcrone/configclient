#!/usr/bin/env python

import os
import json

from flask import Flask, request, abort
from kubernetes import client, config

class configmgr(object):
  def __init__(self, name):
    self.name=name

    if "namespace" in os.environ:
      self.namespace=os.environ["namespace"]
    else:
      self.namespace="default"

    config.load_config()
    self.core_api = client.CoreV1Api()
    try:
      body=self.core_api.read_namespaced_config_map(name,self.namespace)
    except Exception as ex:
      print (f"Creating configMap {name}")
      body=client.V1ConfigMap(api_version="v1",
                              metadata={"name":name},
                              data={"app":""}
                              )
      self.core_api.create_namespaced_config_map(self.namespace,body)
    self.store=body.data
    print(self.store)

  def update(self, newdict):
    body=self.core_api.read_namespaced_config_map(self.name,self.namespace)
    for item in newdict:
      body.data[item]=json.dumps(newdict[item])
    self.core_api.patch_namespaced_config_map(self.name,self.namespace,body)

cmgr=configmgr('test2')
store={'app':{},'resource':{}}
for item in cmgr.store:
  store[item]={}
  if cmgr.store[item]!="":
    appdict=json.loads(cmgr.store[item])
    for a in appdict:
      store[item][a]=appdict[a]
app=Flask('configDict')

@app.route("/")
def dump():
  d=f'<h1>Dump of configuration dictionary</h1>'
  for n in store:
    d=d+f'<h2>{n}</h2> <pre>{store[n]}</pre>'
  print(d)
  return d

@app.route("/publish",methods=['POST'])
def publish():
  print(f"publish() request=[{request.form}]")
  store['app'][request.form['app']]=request.form['conf']
  resources=request.form['resources']
  for resource in resources.split(','):
    store['resource'][resource.strip()]=request.form['app']

  cmgr.update(store)
  return 'OK'

@app.route("/retract",methods=['POST'])
def retract():
  print(f"retract() request=[{request.form}]")
  if request.form['app'] in store['app']:
    store['app'].pop(request.form['app'])
    todelete=[]
    for resource in store['resource']:
      if store['resource'][resource]==request.form['app']:
        todelete.append(resource)
    for resource in todelete:
      store['resource'].pop(resource)
    cmgr.update(store)
    return 'OK'
  else:
    abort(404)

def lookup(map, key):
  if key in map:
    val=map[key]
    print(f"lookup key={key} map[key]={val}")
    return(val)
  else:
    abort(404)

@app.route("/getresource/<resource>")
def get_resource(resource):
  return lookup(store['resource'],resource)
@app.route("/getapp/<app>")
def get_app(app):
  return lookup(store['app'],app)


if __name__ == '__main__':
  app.run()
