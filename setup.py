"""
setup.py — build the blackjack_game pybind11 extension
--------------------------------------------------------
Run from the project root:
    pip install pybind11
    python setup.py build_ext --inplace

This produces  blackjack_game.pyd  (Windows) or  blackjack_game.so  (Linux/Mac)
in the project root, next to flask_app.py.
"""

from setuptools import setup, Extension
import pybind11
import os

src = "src"

sources = [
    os.path.join(src, "Card.cpp"),
    os.path.join(src, "Deck.cpp"),
    os.path.join(src, "Player.cpp"),
    os.path.join(src, "Dealer.cpp"),
    os.path.join(src, "Game.cpp"),
    os.path.join(src, "blackjack_bindings.cpp"),
]

ext = Extension(
    name="blackjack_game",
    sources=sources,
    include_dirs=[src, pybind11.get_include()],
    language="c++",
    extra_compile_args=["/std:c++17"] if os.name == "nt" else ["-std=c++17", "-O2"],
)

setup(
    name="blackjack_game",
    version="1.0",
    ext_modules=[ext],
)
