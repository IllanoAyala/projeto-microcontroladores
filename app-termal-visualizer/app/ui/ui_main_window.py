from PySide6.QtGui import QIcon
from PySide6.QtWidgets import (
    QMainWindow, QVBoxLayout, QHBoxLayout, QPushButton, QFrame
)
from PySide6.QtCore import QThread, Signal

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure

import serial

limit = 3

class UI_MainWindow(QMainWindow):
    def setup_ui(self, parent):
        parent.setObjectName("MainWindow")
        parent.resize(1200, 720)
        parent.setMinimumSize(960, 540)

        self.central_frame = QFrame()
        self.layout = QVBoxLayout(self.central_frame)

        self.layout_graphics = QHBoxLayout()

        self.left_graphic = QFrame()
        self.left_graphic.setStyleSheet("background-color: #44475a;")

        self.right_graphic = QFrame()
        self.right_graphic.setStyleSheet("background-color: #282a36;")

        self.button_start = QPushButton("Start")
        self.button_start.setCheckable(True)
        self.button_start.setStyleSheet("font-size: 10px; font-weight: bold;")
        self.button_start.clicked.connect(self.toggle_process)
        self.button_start.setFixedHeight(30)
        self.button_start.setFixedWidth(100)

        self.layout.addLayout(self.layout_graphics)
        self.layout.addWidget(self.button_start)

        self.layout_graphics.addWidget(self.left_graphic)
        # self.layout_graphics.addWidget(self.right_graphic)

        parent.setCentralWidget(self.central_frame)

        self.data_window_size = 100
        self.index = -1

        self.add_graphics()
        self.setup_process()

    def add_graphics(self):
        self.x_data = []
        self.y = []
        self.y2 = []

        self.left_figure = Figure()
        self.left_canvas = FigureCanvas(self.left_figure)
        self.left_toolbar = NavigationToolbar(self.left_canvas, self)

        self.ax1 = self.left_figure.add_subplot(111)
        self.left_line, = self.ax1.plot([], [], '-o', color='blue', markersize=5)
        self.ax1.set_title("Temperatura")
        self.ax1.set_ylim(0, 50)
        self.ax1.set_xlim(0, 5)
        self.ax1.set_xlabel("Tempo (t)")
        self.ax1.set_ylabel("Temperatura")
        self.ax1.grid(True)

        self.top_line = self.ax1.axhline(y=100, color='black', linestyle='--', label='Top Line')
        self.set_point_line = self.ax1.axhline(y=0, color='r', linestyle='--', label='Set Point')
        self.bottom_line = self.ax1.axhline(y=0, color='black', linestyle='--', label='Bottom Line')
        self.ax1.legend()

        left_layout = QVBoxLayout(self.left_graphic)
        left_layout.addWidget(self.left_toolbar)
        left_layout.addWidget(self.left_canvas)

        self.right_figure = Figure()
        self.right_canvas = FigureCanvas(self.right_figure)
        self.right_toolbar = NavigationToolbar(self.right_canvas, self)

        self.ax2 = self.right_figure.add_subplot(111)
        self.right_line, = self.ax2.plot([], [], '-o', color='blue', markersize=5)
        self.ax2.set_title("Calor")
        self.ax2.set_ylim(0, 1)
        self.ax2.set_xlim(0, 5)
        self.ax2.set_xlabel("Tempo (t)")
        self.ax2.set_ylabel("Calor (°C)")
        self.ax2.grid(True)
        self.ax2.axhline(y=0.5, color='r', linestyle='--', label='Set Point')
        self.ax2.legend()

        right_layout = QVBoxLayout(self.right_graphic)
        right_layout.addWidget(self.right_toolbar)
        right_layout.addWidget(self.right_canvas)

        self.left_canvas.draw()
        self.right_canvas.draw()

    def setup_process(self):
        self.serial_thread = SerialReader(port="COM3", baudrate=115200)
        self.serial_thread.data_received.connect(self.read_serial_data)

    def toggle_process(self):
        if not self.serial_thread.isRunning():
            self.serial_thread.start()
            self.button_start.setText("Stop")
            self.button_start.setChecked(True)
        else:
            self.serial_thread.stop()
            self.button_start.setText("Start")
            self.button_start.setChecked(False)

    def read_serial_data(self, line):
        print("Linha recebida do ESP:", line)  # DEBUG
        if "," in line:
            try:
                val1_str, val2_str, time_attempt = line.split(",")
                val1 = float(val1_str)
                set_point = float(val2_str)
                time_attempt = float(time_attempt)

                if val1 == 0.0 and self.index == -1:
                    return

                self.index += time_attempt
                self.x_data.append(self.index)
                self.y.append(val1)

                # Atualiza a posição do set point sem recriar a linha
                self.set_point_line.set_ydata([set_point, set_point])
                self.bottom_line.set_ydata([set_point - limit, set_point - limit])
                self.top_line.set_ydata([set_point + limit, set_point + limit])

                if len(self.x_data) > 1:
                    if self.x_data[0] == self.x_data[-1]:
                        self.ax1.set_xlim(self.x_data[0] - 0.5, self.x_data[-1] + 0.5)
                    else:
                        self.ax1.set_xlim(self.x_data[0], self.x_data[-1])

                self.left_line.set_data(self.x_data, self.y)

                if self.x_data[0] == self.x_data[-1]:
                    self.ax1.set_xlim(self.x_data[0] - 0.5, self.x_data[-1] + 0.5)
                else:
                    self.ax1.set_xlim(self.x_data[0], self.x_data[-1])

                self.left_canvas.draw()

            except Exception as e:
                print("Erro ao processar linha:", line, e)


class SerialReader(QThread):
    data_received = Signal(str)

    def __init__(self, port="COM3", baudrate=115200):
        super().__init__()
        self.port = port
        self.baudrate = baudrate
        self.running = False

    def run(self):
        try:
            with serial.Serial(self.port, self.baudrate, timeout=1) as ser:
                self.running = True
                while self.running:
                    try:
                        line = ser.readline().decode('utf-8', errors='ignore').strip()
                        if line:
                            self.data_received.emit(line)
                    except Exception as e:
                        print("Erro ao decodificar linha:", e)
        except serial.SerialException as e:
            print(f"Erro na serial: {e}")

    def stop(self):
        self.running = False
        self.quit()
        self.wait()
