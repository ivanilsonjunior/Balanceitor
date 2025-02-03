import serial.tools.list_ports as port_list
from flask_serial import serial
import numpy
import time
import math


class Balanceitor:
    def __init__(self):
        self.balanca = self.locateBalanceitor()

    def locateBalanceitor(self) -> serial.Serial:
        print ("Procurando a balança...", end='')
        ports = list(port_list.comports())
        for port,desc,hwid in ports:
            print ("Abrindo ", port, end='')
            porta = serial.Serial(port=port, baudrate=115200)
            if "Laica - Balanceitor" in porta.readline().decode("ascii"):
                print(" Encontrado em ", porta.port)
                return porta
            else:
                print(porta, "não era uma balança",end='')
        print("Balança não encontrada")

    def medir(self) -> float:
        leitura = []
        if "Pressione 0 para ajuda" in self.balanca.readline().decode("ascii"):
            pass
        if "Segue o jogo" in self.balanca.readline().decode("ascii"):
            pass
        for i in range(5):
            leitura.append(float(self.balanca.readline().decode("ascii")))
        
        return numpy.average(leitura)


    def calibrar(self):
        print ("Iniciando Calibração...")
        if "Pressione 0 para ajuda" in self.balanca.readline().decode("ascii"):
            pass
        self.balanca.write(b"3\n")
        terminal = self.balanca.readline().decode("ascii")
        while math.isnan(float(terminal)):
        #while "Retire todo o peso" :
            print(terminal)
            terminal = self.balanca.readline().decode("ascii")
        while "Retire todo o peso" not in terminal:
            terminal = self.balanca.readline().decode("ascii")
        print ("Retira o peso")
        time.sleep(5)
        self.balanca.write(b"\n")
        print(self.balanca.readline().decode("ascii"))

        if "place a weight on the loadcellponha o peso de 5000g" in self.balanca.readline().decode("ascii"):
            print ("Coloque os 5000g na balança")
            time.sleep(5)
        self.balanca.write(b"\n")
    def tara(self):
        print ("Tara Requerida")
        self.balanca.write(b"1\n")