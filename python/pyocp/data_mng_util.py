"""
Module ``data_mng_util``
========================


"""

import numpy as np

from dataclasses import dataclass, field
import pickle
import zipfile
import json


@dataclass
class DataSet:
    """A dataclass to hold experimental data."""

    #: N-size vector of time instants.
    t : np.array = field(default_factory=lambda: np.empty(0))

    #: (N, M) matrix containing the signals of the data.
    data : np.array = field(default_factory=lambda: np.empty(0))

    #: A dictionary to hold any additional meta data.
    meta : dict = field(default_factory=dict)

    #: Source of the data.
    source : str = field(default_factory=str)


def find_trigger_indices(
    data: np.ndarray, 
    level: float, 
    edge: str = "rising", 
    debounce: int = 0
    ) -> np.ndarray:
    """
    Find indices where the data crosses the trigger level based on the specified
    edge type.

    Parameters
    ----------    
    data
        Input signal vector.

    level
        The level at which to detect crossings.

    edge
        The type of edge to detect ('rising', 'falling', 'both').

    debounce
        Number of samples to ignore after a crossing to prevent multiple
        detections due to noise.

    Returns:
        Indices where the data crosses the trigger level.
    
    """
    if edge not in {"rising", "falling", "both"}:
        raise ValueError("Edge must be 'rising', 'falling', or 'both'")
    
    # Shift the data by one sample to compare adjacent values
    above = data >= level
    below = data < level

    # Transitions
    rising_edges = np.where(above[1:] & below[:-1])[0] + 1
    falling_edges = np.where(below[1:] & above[:-1])[0] + 1
    
    if edge == "rising":
        crossings = rising_edges
    elif edge == "falling":
        crossings = falling_edges
    else:  # "both"
        crossings = np.sort(np.concatenate((rising_edges, falling_edges)))
    
    if debounce > 0 and crossings.size > 0:
        filtered_crossings = [crossings[0]]
        for idx in crossings[1:]:
            if idx > filtered_crossings[-1] + debounce:
                filtered_crossings.append(idx)
        crossings = np.array(filtered_crossings)
    
    return crossings


def load_data(
    file: str
    ) -> DataSet:
    """Loads data from a zipped file.

    Parameters
    ----------
    file
        Name of the file, without ``.zip``. Must include the full or relative
        path if ``file`` is in a different directory.

    Returns
    -------
    data
        Data loaded from the specified file.

    """
    with zipfile.ZipFile(file + '.zip', 'r') as zipf:
        data_bytes = zipf.read('DataSet')

    data = pickle.loads(data_bytes)

    return data


def save_data(
    file: str,
    data: DataSet):
    """Saves data to a zipped file.

    Parameters
    ----------
    file
        Name of the file (without the ``.zip`` extension). The name can contain
        a relative or full path.

    data
        Simulation data.

    """
    with zipfile.ZipFile(file + '.zip', 'w', compression=zipfile.ZIP_LZMA) as zipf:
        zipf.writestr('DataSet', pickle.dumps(data))


def load_json(file: str):
    
    with open(file + r'.json', 'r') as f:
        data = json.load(f)
	
    return data


def save_json(
    file: str,
    data : dict):
    
    with open(file + r'.json', 'w') as f:
        json.dump(
            data, f,
            sort_keys = False, indent = 4,
            ensure_ascii = False
        )
