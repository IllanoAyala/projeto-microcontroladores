import sys
import os

from qt_core import *

from app.ui.ui_main_window import UI_MainWindow

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("TOC - Termal Object Control")


        self.ui = UI_MainWindow()
        self.ui.setup_ui(self)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    app.setWindowIcon(QIcon("icon.ico"))
    window = MainWindow()
    window.show()
    sys.exit(app.exec())


