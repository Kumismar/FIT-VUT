#!/usr/bin/env python3
"""
IZV cast1 projektu
Autor: Ondrej Koumar, xkouma02

Detailni zadani projektu je v samostatnem projektu e-learningu.
Nezapomente na to, ze python soubory maji dane formatovani.

Muzete pouzit libovolnou vestavenou knihovnu a knihovny predstavene na prednasce
"""

from bs4 import BeautifulSoup
import requests
import numpy as np
from numpy.typing import NDArray
import matplotlib.pyplot as plt
from typing import List, Callable, Dict, Any


def integrate(f: Callable[[NDArray], NDArray], a: float, b: float, steps=1000) -> float:
    x_values = np.linspace(a, b, steps)
    return np.sum((x_values[1:] - x_values[:-1])*f((x_values[1:] + x_values[:-1])/2))


def generate_graph(a: List[float], show_figure: bool = False, save_path: str | None = None):
    x = np.linspace(-3, 3, 1000)
    results = np.power(a, 2) * np.power(x.reshape((-1,1)), 3) * np.sin(x.reshape((-1,1)))
    results_f1 = results[:, 0].ravel()
    results_f2 = results[:, 1].ravel()
    results_f3 = results[:, 2].ravel()

    fig = plt.figure(figsize=(10, 5))
    ax1 = fig.add_subplot()
    ax1.plot(x, results)
    ax1.set_xlabel(r"$x$")
    ax1.set_ylabel(r"$f_a(x)$")
    ax1.legend([r"$\gamma_{1.0}(x)$", r"$\gamma_{1.5}(x)$", r"$\gamma_{2.0}$(x)"],
                loc="upper center", bbox_to_anchor=(0.5, 1.15), ncol = 3)
    ax1.set_xmargin(0)
    ax1.set_ylim(0, 40)
    ax1.set_yticks(range(0, 45, 5))
    ax1.set_xlim(-3, 5) 
    ax1.set_xticks(range(-3, 4, 1))
    ax1.fill_between(x, results_f1, alpha=0.1)
    ax1.fill_between(x, results_f2, alpha=0.1)
    ax1.fill_between(x, results_f3, alpha=0.1)

    int1 = np.trapz(results_f1, x)
    int2 = np.trapz(results_f2, x)
    int3 = np.trapz(results_f3, x)

    ax1.text(x[-1], results_f1[-1], r"$\int{f_{1.0}(x)dx} = $" + f"{round(int1, 2)}", ha="left")
    ax1.text(x[-1], results_f2[-1], r"$\int{f_{1.5}(x)dx} = $" + f"{round(int2, 2)}", ha="left")
    ax1.text(x[-1], results_f3[-1], r"$\int{f_{2.0}(x)dx} = $" + f"{round(int3, 2)}", ha="left")

    if show_figure:
        plt.show()
    
    if save_path:
        fig.savefig(save_path)

    plt.close(fig)


def generate_sinus(show_figure: bool = False, save_path: str | None = None):

    def set_properties(ax):
        ax.set_yticks([-0.8, -0.4, 0, 0.4, 0.8])
        ax.set_xmargin(0)

    t = np.linspace(0, 100, 20000)
    f1 = 0.5*np.cos(1/50*np.pi*t)
    f2 = 0.25*(np.sin(np.pi*t) + np.sin(3/2*np.pi*t))

    fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(8, 10))
    plt.tight_layout()

    ax1.plot(t, f1)
    set_properties(ax1)
    ax1.set_xlabel(r"$t$")
    ax1.set_ylabel(r"$f_1(t)$")

    ax2.plot(t, f2)
    set_properties(ax2)
    ax2.set_xlabel(r"$t$")
    ax2.set_ylabel(r"$f_2(t)$")

    f3 = f1+f2
    pos =  f3 >= f1

    # f3[~pos] = np.nan
    # ax3.plot(t, f3, color="green")

    # copy = f3.copy()
    # copy[pos] = np.nan
    # ax3.plot(t, copy, color="red")

    # Komentare jsem zde nechal ciste kvuli tomu, aby bylo jasno, ze jsem se dival na reseni z prednasky.
    # Nicmene tyto dva radky kodu mi prijdou mnohem elegantnejsi (nalezeno na internetech).
    ax3.plot(t, np.ma.masked_where(~pos, f3), color="green")
    ax3.plot(t, np.ma.masked_where(pos, f3), color="red")

    set_properties(ax3)
    ax3.set_ylabel(r"$f_3(t)$")
    ax3.set_xlabel(r"$t$")

    if show_figure:
        plt.show()
    
    if save_path:
        fig.savefig(save_path)

    plt.close(fig)


def download_data() -> List[Dict[str, Any]]:
    response = requests.get("https://ehw.fit.vutbr.cz/izv/st_zemepis_cz.html")

    data_list = []
    if response.status_code == 200:
        response.encoding = "utf-8"
        html = response.text
        soup = BeautifulSoup(html, "html.parser")
        data = soup.find_all("tr")
        for tr in data:
            if "class" in tr.attrs:
                data_dict = {}
                tds = tr.find_all("td")
                data_dict["position"] = tds[0].text
                data_dict["lat"] = float(tds[2].text.replace('°', '').replace(',', '.'))
                data_dict["long"] = float(tds[4].text.replace('°', '').replace(',', '.'))
                data_dict["height"] = float(tds[6].text.strip().replace(',', '.'))
                data_list.append(data_dict)
    return data_list

if __name__ == "main":
    generate_sinus(show_figure=True)
