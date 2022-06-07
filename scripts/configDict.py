from flask import Flask, request, abort

app=Flask('configDict')
store={'app':{},'resource':{}}

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
