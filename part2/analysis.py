#!/usr/bin/env python3.11
# coding=utf-8

from matplotlib import pyplot as plt
import pandas as pd
import seaborn as sns
import numpy as np
import zipfile

# muzete pridat libovolnou zakladni knihovnu ci knihovnu predstavenou na prednaskach
# dalsi knihovny pak na dotaz


def _convert_selected_to_float(df: pd.DataFrame) -> pd.DataFrame:
    """Converts selected columns to float64 data type 

    Args:
        df (pd.DataFrame): Dataframe on which the changes will be made.

    Returns:
        pd.DataFrame: Modified dataframe
    """
    to_float = ["a", "b", "d", "e", "f", "g", "o"]
    for col_name in to_float:
        df[col_name] = pd.to_numeric(
            df[col_name].str.replace(',', '.'), errors="coerce")
    return df


def _convert_selected_to_category(df: pd.DataFrame) -> pd.DataFrame:
    """Converts selected columns to category data type.

    Args:
        df (pd.DataFrame): Dataframe to be modified.

    Returns:
        pd.DataFrame Modified dataframe.
    """
    to_category = ["weekday(p2a)", "k", "l", "p", "q", "t", "p47"]
    for col_name in to_category:
        df[col_name] = df[col_name].astype("category")
    return df


def _get_column_list() -> list:
    """Takes selected column names and puts them into a list.

    Returns:
        list: List of column names.
    """
    tmp_list = ["p5a", "p13a", "p13b", "p13c",
                "p45a", "p48a", "p50a", "p50b", "p55a"]
    tmp_list += [f"p{x}" for x in range(6, 25) if x != 13]
    tmp_list += [f"p{x}" for x in range(34, 40) if x != 38]
    tmp_list += [f"p{x}" for x in [27, 28, 44, 51, 52, 53, 57, 58]]
    return tmp_list


def _convert_selected_to_int(df: pd.DataFrame) -> pd.DataFrame:
    """Converts selected dataframe columns to int data type.

    Args:
        df (pd.DataFrame): Dataframe to be modified.

    Returns:
        pd.DataFrame: Modified dataframe.
    """
    to_int = _get_column_list()
    for col_name in to_int:
        df[col_name] = df[col_name].astype("Int64").astype("Int8")

    for col_name in ["p2b", "p12"]:
        df[col_name] = df[col_name].astype("Int64").astype("Int16")

    df["p1"] = df["p1"].astype("Int64")
    return df


def _convert_selected_to_str(df: pd.DataFrame) -> pd.DataFrame:
    """Converts selected columns to string datatype (object in pandas).

    Args:
        df (pd.DataFrame): Dataframe to be modified.

    Returns:
        pd.DataFrame: Modified dataframe.
    """
    for col_name in ["h", "i"]:
        df[col_name] = df[col_name].astype("str")
    return df


def _convert_columns(df: pd.DataFrame) -> pd.DataFrame:
    """Modifies all dataframe columns to specified data type.

    Args:
        df (pd.DataFrame): Dataframe to be modified.

    Returns:
        pd.DataFrame: Modified dataframe.
    """
    df["p2a"] = pd.to_datetime(df["p2a"])
    df["p2a"].rename("date")
    df = _convert_selected_to_float(df)
    df = _convert_selected_to_category(df)
    df = _convert_selected_to_int(df)
    df = _convert_selected_to_str(df)
    return df


def _print_sizes(df_old: pd.DataFrame, df_new: pd.DataFrame):
    """Prints sizes of original and modified dataframe.

    Args:
        df_old (pd.DataFrame): Original dataframe without changed types.
        df_new (pd.DataFrame): Modified dataframe.
    """
    size_bytes_old = df_old.memory_usage(deep=True).sum()
    size_bytes_new = df_new.memory_usage(deep=True).sum()
    print(f"orig_size={round(size_bytes_old / 1e6, 2)} MB")
    print(f"new_size={round(size_bytes_new / 1e6, 2)} MB".format())


def _set_ax(df: pd.DataFrame,
            ax_array: plt.Axes.axes,
            searched_num: int,
            ax_index: int,
            hline_indices: list[int],
            title: str):
    """Sets all graph properties on specified ax.

    Args:
        df (pd.DataFrame): Dataframe with data to be plotted.
        ax_array (plt.Axes.axes): Figure with axes.
        searched_num (int): Column number with data.
        ax_index (int): Index for ax_array.
        hline_indices (list[int]): Y values where black horizontal lines should append.
        title (str): Graph title.
    """
    filtered_df = df[df["p57"] == searched_num]
    filtered_df = filtered_df.sort_values(by="region")
    kwargs = _get_common_kwargs(filtered_df)
    histplot = sns.histplot(ax=ax_array[ax_index], **kwargs)
    histplot.set(xlabel=None)
    for row in hline_indices:
        histplot.axhline(y=row, color="black", zorder=0,
                         alpha=0.2, linewidth=0.5)
    histplot.set_title(title, fontsize=10)


def _get_common_kwargs(df: pd.DataFrame) -> dict:
    """Gets common keyword arguments for axes and returns them in a dictionary.

    Args:
        df (pd.DataFrame): Dataframe with used data.

    Returns:
        dict: Kwargs for ax.
    """
    return {"data": df, "stat": "count", "x": "region",
            "legend": False, "hue": "region", "alpha": 1,
            "shrink": 0.9, "palette": "husl"}


def _set_common_params(fig: plt.Figure, ax_array: plt.Axes.axes):
    """Sets params for whole figure.

    Args:
        fig (plt.Figure): Figure to be modified.
        ax_array (plt.Axes.axes): Array of axes in figure.
    """
    fig.suptitle("Počet nehod dle stavu řidiče v nedobrém stavu", fontsize=16)
    for ax in ax_array:
        ax.set_ylabel("Počet nehod", fontsize=10)
        ax.tick_params(axis="both", labelsize=8)
    sns.set_theme(style="dark")


def _filter_df(df: pd.DataFrame) -> pd.DataFrame:
    """Modifies dataframe so it contains only specific columns and values.
    Melts the dataframe for plotting.

    Args:
        df (pd.DataFrame): Dataframe to be modified.

    Returns:
        pd.DataFrame: Modified dataframe.
    """
    df_filtered = df.copy()
    df_filtered["p2b"] = df["p2b"] // 100
    df_filtered = df_filtered[(0 <= df_filtered["p2b"])
                              & (df_filtered["p2b"] <= 24)]
    df_filtered = df_filtered[(df_filtered["region"] == "JHM") | (df_filtered["region"] == "PHA") |
                              (df_filtered["region"] == "STC") | (df_filtered["region"] == "MSK")]

    df_filtered = df_filtered.dropna(subset=["p11"])
    df_filtered["Ano"] = (df_filtered["p11"] >= 3)
    df_filtered["Ne"] = (df_filtered["p11"] == 1) | (df_filtered["p11"] == 2)
    df_grouped = df_filtered.groupby(["region", "p2b"]).agg({"Ano": "sum",
                                                             "Ne": "sum"})

    return pd.melt(df_grouped.reset_index(), id_vars=[
        "region", "p2b"], var_name="Alkohol", value_name="count")


def _set_axs_names(axs: plt.Figure.axes):
    """Sets axs names for all axes in figure.

    Args:
        axs (plt.Figure.axes): Axes in figure.
    """
    names = ["JHM", "MSK", "PHA", "STC"]
    for ax, name in zip(axs, names):
        ax.set_title(f"Kraj: {name}", fontsize=14)
        ax.set_xlabel("Hodina")
        ax.set_ylabel("Počet nehod")


def _filter_df_task5(df: pd.DataFrame) -> pd.DataFrame:
    """Filters and pivots dataframe for plotting.

    Args:
        df (pd.DataFrame): Dataframe to be modified.

    Returns:
        pd.DataFrame: Modified dataframe.
    """
    df_filtered = df.copy()

    # Redukce poctu sloupcu
    df_filtered = df_filtered[(df_filtered["p10"] == 1) | (
        df_filtered["p10"] == 2) | (df_filtered["p10"] == 3) | (df_filtered["p10"] == 4)]
    df_filtered = df_filtered[(df_filtered["region"] == "JHM") | (df_filtered["region"] == "PHA") |
                              (df_filtered["region"] == "STC") | (df_filtered["region"] == "MSK")]
    df_filtered["p10"] = df_filtered["p10"].astype(str).map(
        {"1": "Řidičem motorového vozidla", "2": "Řidičem nemotorového vozidla",
         "3": "Chodcem", "4": "Zvířetem"})
    df_filtered.rename(
        columns={"p2a": "Datum", "p10": "Zavinění"}, inplace=True)
    df_filtered = df_filtered.loc[:, ["region", "Datum", "Zavinění"]]

    # Hybani a pivotovani se sloupci
    df_filtered["aux"] = df_filtered["Zavinění"]
    df_filtered = df_filtered.pivot_table(
        index=["region", "Datum"], values="aux", columns="Zavinění", aggfunc="count", fill_value=0)
    to_melt = ['Chodcem', 'Zvířetem', 'Řidičem motorového vozidla',
               "Řidičem nemotorového vozidla"]
    df_filtered = df_filtered.reset_index().melt(
        id_vars=["region", "Datum"], value_vars=to_melt, var_name="Zavinění", value_name="Počet")
    df_filtered = df_filtered.groupby(
        ["region", "Zavinění"]).resample("MS", on="Datum").sum("Počet")

    return df_filtered


def _set_axs_params(fig: sns.FacetGrid):
    """Sets common axis params with values different for each ax. 

    Args:
        fig (sns.FacetGrid): Seaborn figure with axes.
    """
    for ax, name in zip(fig.axes.flat, ["JHM", "MSK", "PHA", "STC"]):
        ax.set_title(f"Kraj: {name}")
        ax.set_xticks([f"20{x}" for x in range(16, 24)])
        ax.set_xticklabels([f"01/{x}" for x in range(16, 24)])
        ax.set_xlim(pd.to_datetime("1.1.2016"), pd.to_datetime("1.1.2023"))

    fig.axes.flat[0].set_ylabel("Počet nehod")
    fig.axes.flat[2].set_ylabel("Počet nehod")
    fig.figure.suptitle(
        "Zavinění dopravních nehod v průběhu šesti let", fontsize=16)

# Ukol 1: nacteni dat ze ZIP souboru


def load_data(filename: str) -> pd.DataFrame:
    """Read csv files in 2-level nested zips and put its data to one dataframe.

    Args:
        filename (str): Path to zip file you want to open.

    Returns:
        pd.DataFrame: Dataframe with data from all the csv files.
    """
    # tyto konstanty nemente, pomuzou vam pri nacitani
    headers = ["p1", "p36", "p37", "p2a", "weekday(p2a)", "p2b", "p6", "p7", "p8", "p9", "p10", "p11", "p12", "p13a",
               "p13b", "p13c", "p14", "p15", "p16", "p17", "p18", "p19", "p20", "p21", "p22", "p23", "p24", "p27", "p28",
               "p34", "p35", "p39", "p44", "p45a", "p47", "p48a", "p49", "p50a", "p50b", "p51", "p52", "p53", "p55a",
               "p57", "p58", "a", "b", "d", "e", "f", "g", "h", "i", "j", "k", "l", "n", "o", "p", "q", "r", "s", "t", "p5a"]

    # def get_dataframe(filename: str, verbose: bool = False) -> pd.DataFrame:
    regions = {
        "PHA": "00",
        "STC": "01",
        "JHC": "02",
        "PLK": "03",
        "ULK": "04",
        "HKK": "05",
        "JHM": "06",
        "MSK": "07",
        "OLK": "14",
        "ZLK": "15",
        "VYS": "16",
        "PAK": "17",
        "LBK": "18",
        "KVK": "19",
    }

    dataframe = pd.DataFrame()

    with zipfile.ZipFile(filename, mode='r') as outer_zip:
        for zip_file in outer_zip.namelist():
            with outer_zip.open(zip_file, mode='r') as inner_zip:
                with zipfile.ZipFile(inner_zip, mode='r') as inner:
                    for csv_file_name in regions.values():
                        with inner.open(f"{csv_file_name}.csv", mode='r') as csv:
                            df = pd.read_csv(
                                csv, encoding="cp1250", delimiter=';', names=headers, low_memory=False
                            )
                            df["region"] = [
                                index for index, value in regions.items() if value == csv_file_name
                            ][0]
                            dataframe = pd.concat([df, dataframe])
    return dataframe


# Ukol 2: zpracovani dat


def parse_data(df: pd.DataFrame, verbose: bool = False) -> pd.DataFrame:
    """Copy dataframe, convert column data types for smaller memory\
          usage and delete duplicate rows, print original and new size.

    Args:
        df (pd.DataFrame): Non memory-optimized dataframe.

        verbose (bool, optional): Printing flag. Prints original \
            dataframe size and new dataframe size if set to True.\
            Defaults to False.

    Returns:
        pd.DataFrame: Memory-optimized dataframe without duplicate rows.
    """
    df_copy = pd.DataFrame.copy(df, deep=True)
    df_copy = _convert_columns(df_copy)
    df_copy = df_copy.drop_duplicates(subset=["p1"], keep="first")

    if verbose:
        _print_sizes(df, df_copy)

    return df_copy


# Ukol 3:


def plot_state(df: pd.DataFrame, fig_location: str = None,
               show_figure: bool = False):
    """Plot six graphs saying in which state the driver was when the \
    crash happened.

    Args:
        df (pd.DataFrame): Data.
        fig_location (str, optional): Filepath. Save figure when\
              present. Defaults to None.
        show_figure (bool, optional): Flag determining whether \
            the graphs will be shown. Defaults to False.
    """
    fig, axes = plt.subplots(nrows=3, ncols=2, figsize=(12, 10))
    ax_array = axes.flatten()
    _set_common_params(fig, ax_array)

    _set_ax(df, ax_array, 8, 0, [25, 50, 75, 100, 125,
            150, 175], "Stav řidiče: řidič při jízdě zemřel")
    _set_ax(df, ax_array, 2, 1, [200, 400, 600, 800, 1000, 1200],
            "Stav řidiče: unaven, usnul, náhlá fyzická indispozice")
    _set_ax(df, ax_array, 3, 2, [50, 100, 150, 200, 250,
            300], "Stav řidiče: pod vlivem léků, narkotik")
    _set_ax(df, ax_array, 4, 3, [200, 400, 600, 800],
            "Stav řidiče: pod vlivem alkoholu do 0,99\u2030")
    _set_ax(df, ax_array, 5, 4, [500, 1000, 1500, 2000, 2500,
            3000], "Stav řidiče: pod vlivem alkoholu 1\u2030 a více")
    _set_ax(df, ax_array, 7, 5, [10, 20, 30, 40], "Stav řidiče: invalida")

    for ax in ax_array[-2:]:
        ax.set_xlabel("Kraj", fontsize=10)

    plt.tight_layout()

    if fig_location:
        plt.savefig(fig_location)
    if show_figure:
        plt.show()


# Ukol4: alkohol v jednotlivých hodinách


def plot_alcohol(df: pd.DataFrame, fig_location: str = None,
                 show_figure: bool = False):
    """Plot histograms showing data of drunk driver crashes per hour in several regions.


    Args:
        df (pd.DataFrame): Dataframe from which the data is read.
        fig_location (str, optional): Where this function should store figure.\
            Defaults to None.
        show_figure (bool, optional): Flag determining whether the\
            plot should be shown. Defaults to False.
    """
    df_filtered = _filter_df(df)
    sns.set_theme("notebook")
    cp = sns.catplot(x="p2b", y="count", hue="Alkohol", col="region", data=df_filtered,
                     kind="bar", col_wrap=2, aspect=1.2, sharey=False, sharex=False)
    axs = cp.axes.flatten()
    _set_axs_names(axs)
    cp.figure.suptitle(
        "Opilí a neopilí řidiči v různých hodinách", fontsize=16)

    cp.tight_layout()
    if fig_location:
        cp.savefig(fig_location)
    if show_figure:
        plt.show()


# Ukol 5: Zavinění nehody v čase


def plot_fault(df: pd.DataFrame, fig_location: str = None,
               show_figure: bool = False):
    """Plot information about what caused how many crashes in a period of time. 

    Args:
        df (pd.DataFrame): Dataframe from which the data will be read.
        fig_location (str, optional): Where this function should store figure.\
            Defaults to None.
        show_figure (bool, optional): Flag determining whether the\
            plot should be shown. Defaults to False.
    """
    df_filtered = _filter_df_task5(df)

    sns.set_theme("notebook")
    rp = sns.relplot(data=df_filtered, x="Datum", y="Počet", col="region", hue="Zavinění",
                     kind="line", height=4, aspect=1.2, col_wrap=2,
                     facet_kws={"sharex": False, "sharey": False})

    _set_axs_params(rp)

    rp.tight_layout()
    if fig_location:
        rp.savefig(fig_location)
    if show_figure:
        plt.show()


if __name__ == "__main__":
    # zde je ukazka pouziti, tuto cast muzete modifikovat podle libosti
    # skript nebude pri testovani pousten primo, ale budou volany konkreni
    # funkce.
    df = load_data("data/data.zip")
    df2 = parse_data(df, True)

    plot_state(df2, show_figure=True)
    plot_alcohol(df2, show_figure=True, fig_location="02_alcohol.png")
    plot_fault(df2, show_figure=True, fig_location="03_fault.png")


# Poznamka:
# pro to, abyste se vyhnuli castemu nacitani muzete vyuzit napr
# VS Code a oznaceni jako bunky (radek #%%% )
# Pak muzete data jednou nacist a dale ladit jednotlive funkce
# Pripadne si muzete vysledny dataframe ulozit nekam na disk (pro ladici
# ucely) a nacitat jej naparsovany z disku
