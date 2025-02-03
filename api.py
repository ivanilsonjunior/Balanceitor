from flask import Flask, render_template, send_file, Response, abort, jsonify, request, url_for, redirect, logging
from Balanceitor import Balanceitor
import datetime


app = Flask(__name__, template_folder="templates")
app.config['MAX_CONTENT_LENGTH'] = 1024 * 1024
app.config['UPLOAD_EXTENSIONS'] = ['.sql']
app.config['UPLOAD_PATH'] = 'uploads/'

balanca = ""
uso = 0
valores = {}

def medir() -> float:
    valor = balanca.medir()/1000
    global valores
    valores[datetime.datetime.now().strftime('%H:%M:%S')] = valor
    return valor


@app.route('/')
def hello():
    global uso
    global balanca
    if uso == 0:
        balanca = Balanceitor()
        balanca.calibrar()
        uso+=1
    return render_template("index.html", peso=medir())

@app.route('/peso')
def pesar():
    retorno = {'valor': medir()}
    return jsonify(retorno)

@app.route('/limpar')
def limparGraph():
        global valores
        valores = {}
        return redirect("/")

@app.route('/grafico')
def homepage():
     # Define Plot Data 
    global valores
    labels = list(valores.keys())
 
    data = list(valores.items())
    # Return the components to the HTML template 
    return render_template(
        template_name_or_list='grafico.html',
        data=data,
        labels=labels,
    )

if __name__ == '__main__':
    app.run(host="127.0.0.1", port=22222, debug=True, threaded=False)