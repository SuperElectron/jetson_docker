# /video_server/server/setup_logging.py

from pathlib import Path
from os import system
import logging
from datetime import datetime


def setup_logging(folder="/var/log/", filename="python.log", level=logging.DEBUG):

    try:
        system(f"mkdir -p {folder}")
        filepath = folder + filename
        Path(filepath).touch(mode=0o777, exist_ok=True)
        system(f"ln -sf /dev/stdout {filepath}")
        logging.basicConfig(filename=filepath, filemode="w", level=level)
        logging.debug(f"\t\tLOG FILE LOCATION: {filepath}")
        logging.info(f"Python logger started at {datetime.now()}\n")

    except Exception as err:
        print(f"INFO: Must run scripts with 'sudo' for logging to take effect")
        print(f"ERROR (setup_logging): {err}")
