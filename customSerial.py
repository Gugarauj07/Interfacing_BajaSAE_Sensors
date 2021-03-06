from threading import Event, Thread
import serial
import serial.tools.list_ports
from time import strftime
from pathlib import Path
from PyQt5.QtCore import QObject, pyqtSignal
from pyqtgraph import PlotWidget, plot, mkPen
import csv
import pandas as pd


class customSerial(QObject):
    data_available = pyqtSignal(str)

    def __init__(self):
        super().__init__()

        self.cvtArray = list()
        self.rpmArray = list()
        self.velocidadeArray = list()

        self.serialPort = serial.Serial()
        self.serialPort.timeout = 0.5

        self.arquivo = strftime("%d.%m.%Y_%Hh%M")
        path = Path("Arquivos_CSV")
        path.mkdir(parents=True, exist_ok=True)

        with open(f"Arquivos_CSV/{self.arquivo}.csv", 'w', newline='') as f:
            self.thewriter = csv.writer(f)
            self.thewriter.writerow(['cvt', 'velocidade', 'rpm'])

        self.baudratesDIC = {
            '9600': 9600,
            '19200': 19200,
            '38400': 38400,
            '57600': 57600,
            '115200': 115200
        }
        self.portList = list()

        self.thread = None
        self.alive = Event()

    def update_ports(self):
        self.portList = [port.device for port in serial.tools.list_ports.comports()]
        print(self.portList)

    def read_serial(self):
        while self.alive.isSet() and self.serialPort.is_open:

            self.data = self.serialPort.readline().decode("utf-8").split(',')
            print(self.data)

            if len(self.data) == 3:
                with open(f"Arquivos_CSV/{self.arquivo}.csv", 'a+', newline='') as f:
                    self.thewriter = csv.writer(f)
                    self.thewriter.writerow([self.data[0], self.data[1], self.data[2]])

                df = pd.read_csv(f"Arquivos_CSV/{self.arquivo}.csv").tail(100)

                # self.velocidadeArray = list(df["velocidade"])
                self.rpmArray = list(df["rpm"])
                self.cvtArray = list(df["cvt"])

                try:
                    self.window.displayVeloc.display(self.data[1])
                    self.window.displayRPM.display(self.data[2])
                    self.window.displayCVT.display(self.data[0])

                    self.window.velocimetro.updateValue(float(self.data[1]))

                    self.pen = mkPen(width=2)
                    self.window.graphRPM.clear()
                    self.window.graphRPM.plot(self.rpmArray, pen=self.pen)

                    self.window.graphCVT.clear()
                    self.window.graphCVT.plot(self.cvtArray, pen=self.pen)
                except (ValueError, TypeError):
                    pass

    def start_thread(self):
        self.thread = Thread(target=self.read_serial)
        self.thread.setDaemon(1)
        self.alive.set()
        self.thread.start()

    def update_window(self, window):
        self.window = window
