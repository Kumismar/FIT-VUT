#!/bin/bash

if [ -d venv ]; then 
    source venv/bin/activate
else
    python3.11 -m venv venv/
    source venv/bin/activate
    pip install matplotlib seaborn pandas numpy ipykernel
fi
